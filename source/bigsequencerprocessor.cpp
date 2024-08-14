//------------------------------------------------------------------------
// Copyright(c) 2024 Vargason.
//------------------------------------------------------------------------

#include "bigsequencerprocessor.h"
#include "bigsequencercids.h"
#include "plugids.h"
#include "scales.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstevents.h"


using namespace Steinberg;

namespace vargason::bigsequencer {
	//------------------------------------------------------------------------
	// BigSequencerProcessor
	//------------------------------------------------------------------------
	BigSequencerProcessor::BigSequencerProcessor()
	{
		//--- set the wanted controller for our processor
		setControllerClass(kBigSequencerControllerUID);
	}

	//------------------------------------------------------------------------
	BigSequencerProcessor::~BigSequencerProcessor()
	{}

	//------------------------------------------------------------------------
	tresult PLUGIN_API BigSequencerProcessor::initialize(FUnknown* context)
	{
		// Here the Plug-in will be instantiated

		//---always initialize the parent-------
		tresult result = AudioEffect::initialize(context);
		// if everything Ok, continue
		if (result != kResultOk)
		{
			return result;
		}
		addEventOutput(STR16("Event Out"), 1);
		
		this->sequencer = new Sequencer();
		this->randomNoteGenerator = new RandomNoteDataGenerator();
		regenerateGridNotes();
		return kResultOk;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API BigSequencerProcessor::terminate()
	{
		// Here the Plug-in will be de-instantiated, last possibility to remove some memory!

		delete sequencer;
		delete randomNoteGenerator;

		//---do not forget to call parent ------
		return AudioEffect::terminate();
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API BigSequencerProcessor::setActive(TBool state)
	{
		//--- called when the Plug-in is enable/disable (On/Off) -----

		return AudioEffect::setActive(state);
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API BigSequencerProcessor::process(Vst::ProcessData& data)
	{
		handleParameterChanges(data);

		// if we don't have a process context, the plugin can't do anything
		if (data.processContext == NULL) {
			return kResultOk;
		}

		// handle host being start & stopped
		bool playing = data.processContext->state & data.processContext->kPlaying;
		if (playing && !wasPreviouslyPlaying) {  // started from paused or stopped state.
			if (data.processContext->projectTimeMusic == data.processContext->cycleStartMusic) {  // started from stopped state
				for (int i = 0; i < sequencer->maxNumCursors; i++) {
					Cursor& cursor = sequencer->getCursor(i);

					if (retrigger) {
						cursor.position = 0;
						cursor.lastNoteTime = 0;
					}
					cursor.position = retrigger ? 0 : cursor.position;

					sendCursorUpdate(i, cursor);
					if (cursor.active) {
						NoteData noteData = sequencer->getNote(cursor.position);
						if (noteData.active) {
							cursor.notePlaying = true;
							uint8_t pitch = sequencer->getNote(0).pitch + cursor.pitchOffset;
							cursor.currentlyPlayingNote = pitch;
							sendMidiNoteOn(data.outputEvents, pitch, cursor.velocity);
						}
						cursor.position++;
					}
				}
			}
		}
		else if (!playing && wasPreviouslyPlaying) {  // we stopped or paused playback
			for (int i = 0; i < sequencer->maxNumCursors; i++) {
				Cursor& cursor = sequencer->getCursor(i);
				if (cursor.notePlaying) {
					cursor.notePlaying = false;
					uint8_t pitch = cursor.currentlyPlayingNote;
					sendMidiNoteOff(data.outputEvents, pitch);
				}
			}
		}

		wasPreviouslyPlaying = playing;

		if (hostSynced && playing) {
			updateSequencer(data);
		}

		lastProjectMusicTime = data.processContext->projectTimeMusic;

		return kResultOk;
	}

	void BigSequencerProcessor::handleParameterChanges(Vst::ProcessData& data) {
		// get parameter changes
		if (data.inputParameterChanges) {
			int32 numParamsChanged = data.inputParameterChanges->getParameterCount();
			for (int32 index = 0; index < numParamsChanged; index++) {
				Vst::IParamValueQueue* paramQueue = data.inputParameterChanges->getParameterData(index);
				if (paramQueue) {
					Vst::ParamValue value;
					int32 sampleOffset;
					int32 numPoints = paramQueue->getPointCount();
					switch (paramQueue->getParameterId()) {
					case SequencerParams::kParamSequencerWidthId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							int width = 1 + value * sequencer->maxWidth;
							sequencer->setSize(width, sequencer->getHeight());  // cursor could be out of bounds if we do this wrong
							regenerateGridNotes();
							sendSequencerUpdate();
						}
						break;
					case SequencerParams::kParamSequencerHeightId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							int height = 1 + value * sequencer->maxHeight;
							sequencer->setSize(sequencer->getWidth(), height);
							regenerateGridNotes();
							sendSequencerUpdate();
						}
						break;
					case SequencerParams::kParamHostSyncId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							// hostSynced = value;
						}
						break;
					case SequencerParams::kParamRetriggerId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							retrigger = value;
						}
						break;

						// Cursor 1
					case SequencerParams::kParamCursor1ActiveId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							sequencer->getCursor(0).active = value;
							sendCursorActiveUpdate(0, value);
						}
						break;
					case SequencerParams::kParamCursor1NoteLengthId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							sequencer->getCursor(0).setNoteLength(value);
						}
						break;
					case SequencerParams::kParamCursor1NoteIntervalId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							sequencer->getCursor(0).interval = (Interval)(Interval::thirtySecondNote + (Interval::doubleWholeNote - Interval::thirtySecondNote) * value);
						}
						break;
					case SequencerParams::kParamCursor1PitchOffsetId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							Cursor& cursor = sequencer->getCursor(0);
							cursor.pitchOffset = cursor.pitchMin + (cursor.pitchMax - cursor.pitchMin) * value;
						}
						break;
					case SequencerParams::kParamCursor1VelocityId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							Cursor& cursor = sequencer->getCursor(0);
							cursor.velocity = value;
						}
						break;

						// Cursor 2
					case SequencerParams::kParamCursor2ActiveId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							sequencer->getCursor(1).active = value;
							sendCursorActiveUpdate(1, value);
						}
						break;
					case SequencerParams::kParamCursor2NoteLengthId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							sequencer->getCursor(1).setNoteLength(value);
						}
						break;
					case SequencerParams::kParamCursor2NoteIntervalId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							sequencer->getCursor(1).interval = (Interval)(Interval::thirtySecondNote + (Interval::doubleWholeNote - Interval::thirtySecondNote) * value);
						}
						break;
					case SequencerParams::kParamCursor2PitchOffsetId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							Cursor& cursor = sequencer->getCursor(1);
							cursor.pitchOffset = cursor.pitchMin + (cursor.pitchMax - cursor.pitchMin) * value;
						}
						break;
					case SequencerParams::kParamCursor2VelocityId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							Cursor& cursor = sequencer->getCursor(1);
							cursor.velocity = value;
						}
						break;

						// Cursor 3
					case SequencerParams::kParamCursor3ActiveId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							sequencer->getCursor(2).active = value;
							sendCursorActiveUpdate(2, value);
						}
						break;
					case SequencerParams::kParamCursor3NoteLengthId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							sequencer->getCursor(2).setNoteLength(value);
						}
						break;
					case SequencerParams::kParamCursor3NoteIntervalId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							sequencer->getCursor(2).interval = (Interval)(Interval::thirtySecondNote + (Interval::doubleWholeNote - Interval::thirtySecondNote) * value);
						}
						break;
					case SequencerParams::kParamCursor3PitchOffsetId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							Cursor& cursor = sequencer->getCursor(2);
							cursor.pitchOffset = cursor.pitchMin + (cursor.pitchMax - cursor.pitchMin) * value;
						}
						break;
					case SequencerParams::kParamCursor3VelocityId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							Cursor& cursor = sequencer->getCursor(2);
							cursor.velocity = value;
						}
						break;

						// Cursor 4
					case SequencerParams::kParamCursor4ActiveId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							sequencer->getCursor(3).active = value;
							sendCursorActiveUpdate(3, value);
						}
						break;
					case SequencerParams::kParamCursor4NoteLengthId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							sequencer->getCursor(3).setNoteLength(value);
						}
						break;
					case SequencerParams::kParamCursor4NoteIntervalId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							sequencer->getCursor(3).interval = (Interval)(Interval::thirtySecondNote + (Interval::doubleWholeNote - Interval::thirtySecondNote) * value);
						}
						break;
					case SequencerParams::kParamCursor4PitchOffsetId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							Cursor& cursor = sequencer->getCursor(3);
							cursor.pitchOffset = cursor.pitchMin + (cursor.pitchMax - cursor.pitchMin) * value;
						}
						break;
					case SequencerParams::kParamCursor4VelocityId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							Cursor& cursor = sequencer->getCursor(3);
							cursor.velocity = value;
						}
						break;

						// "fake" parameters
					case SequencerParams::kParamScaleId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							scale = (Scale)(10 * value);
							regenerateGridNotes();
							sendSequencerUpdate();
						}
						break;
					case SequencerParams::kParamRootNoteId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							rootNote = (Pitch)(Pitch::b * value);
							regenerateGridNotes();
							sendSequencerUpdate();
						}
						break;
					case SequencerParams::kParamMinNoteId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							minNote = noteLowerBound + (noteUpperBound - noteLowerBound) * value;
							if (minNote > maxNote) {
								maxNote = minNote;
							}
							regenerateGridNotes();
							sendSequencerUpdate();
						}
						break;
					case SequencerParams::kParamMaxNoteId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							maxNote = noteLowerBound + (noteUpperBound - noteLowerBound) * value;
							if (minNote > maxNote) {
								minNote = maxNote;
							}
							regenerateGridNotes();
							sendSequencerUpdate();
						}
						break;
					case SequencerParams::kParamFillChanceId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							randomNoteGenerator->fillChance = value;
							regenerateGridNotes();
							sendSequencerUpdate();
						}
						break;
					}
				}
			}
		}
	}

	void BigSequencerProcessor::updateSequencer(Vst::ProcessData& data) {
		double cycleLength = data.processContext->cycleEndMusic - data.processContext->cycleStartMusic;

		for (int cursorIndex = 0; cursorIndex < sequencer->maxNumCursors; cursorIndex++) {
			Cursor& cursor = sequencer->getCursor(cursorIndex);
			if (lastProjectMusicTime > data.processContext->projectTimeMusic) {  // the measure/song ended and we are back at 0
				cursor.lastNoteTime -= cycleLength;
			}
			updateCursor(data, cursorIndex, cursor);
		}
	}

	void BigSequencerProcessor::updateCursor(Vst::ProcessData& data, int index, Cursor& cursor) {
		double quarterNotes = data.processContext->projectTimeMusic;
		float numericInterval = cursor.numericInterval();
		if (quarterNotes >= cursor.lastNoteTime + numericInterval) {
			if (cursor.active) {
				NoteData noteData = sequencer->getNote(cursor.position);
				if (noteData.active) {
					uint8_t realPitch = noteData.pitch + cursor.pitchOffset;
					sendMidiNoteOn(data.outputEvents, realPitch, cursor.velocity);

					cursor.notePlaying = true;
					cursor.currentlyPlayingNote = realPitch;
				}
				sendCursorUpdate(index, cursor);
				int newPos = cursor.position + 1;
				int totalNotes = sequencer->totalNotes();
				if (newPos >= totalNotes) {
					newPos = 0;
				}
				cursor.position = newPos;
			}
			cursor.lastNoteTime = quarterNotes;
		}
		if (cursor.notePlaying && quarterNotes >= cursor.lastNoteTime + cursor.realNoteLength()) {
			sendMidiNoteOff(data.outputEvents, cursor.currentlyPlayingNote);
			cursor.notePlaying = false;
		}
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API BigSequencerProcessor::setupProcessing(Vst::ProcessSetup& newSetup)
	{
		//--- called before any processing ----
		return AudioEffect::setupProcessing(newSetup);
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API BigSequencerProcessor::canProcessSampleSize(int32 symbolicSampleSize)
	{
		// by default kSample32 is supported
		if (symbolicSampleSize == Vst::kSample32)
			return kResultTrue;

		// disable the following comment if your processing support kSample64
		/* if (symbolicSampleSize == Vst::kSample64)
			return kResultTrue; */

		return kResultFalse;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API BigSequencerProcessor::setState(IBStream* state)
	{
		if (!state) {
			return kResultFalse;
		}
		/*
		// called when we load a preset, the model has to be reloaded
		IBStreamer streamer (state, kLittleEndian);
		short width = 0;
		if (!streamer.readInt16(width)) {
			return kResultFalse;
		}
		short height = 0;
		if (!streamer.readInt16(height)) {
			return kResultFalse;
		}
		short cursor = 0;
		if (!streamer.readInt16(height)) {
			return kResultFalse;
		}

		NoteData* noteDatas = new NoteData[width * height];
		bool failure = false;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				char pitch = 0;
				char velocity = 0;
				char probability = 0;

				if (!streamer.readInt8(pitch)) {
					 failure = false;
					 break;
				}
				if (!streamer.readInt8(velocity)) {
					failure = false;
					break;
				}
				if (!streamer.readInt8(probability)) {
					failure = false;
					break;
				}
				noteDatas[y * width + x].pitch = pitch;
				noteDatas[y * width + x].velocity = velocity;
				noteDatas[y * width + x].probability = probability;
			}
			if (failure) {
				break;
			}
		}

		if (failure) {
			delete[] noteDatas;
			return kResultFalse;
		}

		sequencer->setNotes(width, height, noteDatas);
		sequencer->setCursor(cursor);
		*/
		return kResultOk;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API BigSequencerProcessor::getState(IBStream* state)
	{
		/*
		// here we need to save the model
		IBStreamer streamer(state, kLittleEndian);
		int width = sequencer->getWidth();
		int height = sequencer->getHeight();
		double cursor = sequencer->getCursor();
		streamer.writeInt16(width);
		streamer.writeInt16(height);
		streamer.writeDouble(cursor);

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				NoteData noteData = sequencer->getNote(x, y);
				streamer.writeInt8(noteData.pitch);
				streamer.writeInt8(noteData.velocity);
				streamer.writeInt8(noteData.probability);
			}
		}
		*/
		return kResultOk;
	}

	//------------------------------------------------------------------------


	void BigSequencerProcessor::sendMidiNoteOn(Vst::IEventList* eventList, uint8_t pitch, float velocity) {
		if (eventList) {
			Vst::Event midiEvent = { 0 };
			midiEvent.type = Vst::Event::kNoteOnEvent;
			midiEvent.sampleOffset = 0;
			midiEvent.noteOn.channel = 0;
			midiEvent.noteOn.pitch = pitch;
			midiEvent.noteOn.velocity = velocity;
			midiEvent.noteOn.noteId = pitch;
			eventList->addEvent(midiEvent);
		}
	}

	void BigSequencerProcessor::sendMidiNoteOff(Vst::IEventList* eventList, uint8_t pitch) {
		if (eventList) {
			Vst::Event midiEvent = { 0 };
			midiEvent.type = Vst::Event::kNoteOffEvent;
			midiEvent.sampleOffset = 0;
			midiEvent.noteOff.channel = 0;
			midiEvent.noteOff.pitch = pitch;
			midiEvent.noteOff.velocity = 0;
			midiEvent.noteOff.noteId = pitch;
			eventList->addEvent(midiEvent);
		}
	}

	void BigSequencerProcessor::regenerateGridNotes() {
		NoteData* noteData = randomNoteGenerator->generate(sequencer->getWidth(), sequencer->getHeight(), rootNote, scale, minNote, maxNote);
		sequencer->setNotes(sequencer->getWidth(), sequencer->getHeight(), noteData);
	}

	void BigSequencerProcessor::sendSequencerUpdate() {
		Vst::IMessage* message = allocateMessage();
		if (!message) {
			return;
		}
		message->setMessageID("SequencerMessage");
		Steinberg::Vst::IAttributeList* attr = message->getAttributes();
		if (attr) {
			std::vector<char> sequencerData;
			getSequencerData(sequencerData);
			attr->setBinary("sequencer", &sequencerData[0], sequencerData.size());
		}
		sendMessage(message);
	}

	void BigSequencerProcessor::getSequencerData(std::vector<char>& sequencerData) {
		sequencerData.push_back(sequencer->getWidth());
		sequencerData.push_back(sequencer->getHeight());
		for (int y = 0; y < sequencer->getHeight(); y++) {
			for (int x = 0; x < sequencer->getWidth(); x++) {
				NoteData& noteData = sequencer->getNote(x, y);
				sequencerData.push_back(noteData.active);
				sequencerData.push_back(noteData.pitch);
			}
		}
	}

	void BigSequencerProcessor::sendCursorActiveUpdate(int index, bool val) {
		Vst::IMessage* message = allocateMessage();
		if (!message) {
			return;
		}
		message->setMessageID("CursorMessage");
		Steinberg::Vst::IAttributeList* attr = message->getAttributes();
		if (attr) {
			attr->setInt("index", index);
			attr->setInt("active", val);
		}
		sendMessage(message);
	}

	void BigSequencerProcessor::sendCursorUpdate(int index, Cursor& cursor) {
		Vst::IMessage* message = allocateMessage();
		if (!message) {
			return;
		}
		message->setMessageID("CursorMessage");
		Steinberg::Vst::IAttributeList* attr = message->getAttributes();
		if (attr) {
			attr->setInt("index", index);
			attr->setInt("position", cursor.position);
		}
		sendMessage(message);
	}
}

// namespace vargason
