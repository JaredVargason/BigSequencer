//------------------------------------------------------------------------
// Copyright(c) 2024 Vargason.
//------------------------------------------------------------------------

#include "bigsequencerprocessor.h"
#include "bigsequencercids.h"
#include "plugids.h"

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
		this->wasPreviouslyPlaying = false;
		return kResultOk;
	}

	//------------------------------------------------------------------------
	tresult PLUGIN_API BigSequencerProcessor::terminate()
	{
		// Here the Plug-in will be de-instantiated, last possibility to remove some memory!
		delete sequencer;
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
					case SequencerParams::kParamNoteLengthId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue)
							sequencer->setNoteLength(value);
						break;
					}
				}
			}
		}

		// if we don't have a process context, the plugin can't do anything
		if (data.processContext == NULL) {
			return kResultOk;
		}

		// handle host being start & stopped
		bool playing = data.processContext->state & data.processContext->kPlaying;
		if (playing && !wasPreviouslyPlaying) {  // started
			sendMidiNoteOn(data.outputEvents, 60, 0.40);
			sequencer->setCursor(0);
			sequencer->setNotePlaying(true);
		}
		else if (!playing && wasPreviouslyPlaying) {  // stopped
			if (sequencer->isNotePlaying()) {
				sendMidiNoteOff(data.outputEvents, 60, 0);
				sequencer->setNotePlaying(false);
			}
		}
		wasPreviouslyPlaying = playing;

		double quarterNotes = data.processContext->projectTimeMusic;  // can also get this by dividing projectTimeSamples by sampleRate
		double cursor = sequencer->getCursor();

		if (quarterNotes >= cursor + 1.0) {
			sendMidiNoteOn(data.outputEvents, 60, 0.40);
			sequencer->setNotePlaying(true);
			sequencer->setCursor(quarterNotes);
		}
		else if (sequencer->isNotePlaying() && quarterNotes >= cursor + sequencer->getNoteLength()) {
			sendMidiNoteOff(data.outputEvents, 60, 0);
			sequencer->setNotePlaying(false);
		}
		return kResultOk;
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


	void BigSequencerProcessor::sendMidiNoteOn(Vst::IEventList* eventList, uint16_t pitch, float velocity) {
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

	void BigSequencerProcessor::sendMidiNoteOff(Vst::IEventList* eventList, uint16_t pitch, float velocity) {
		if (eventList) {
			Vst::Event midiEvent = { 0 };
			midiEvent.type = Vst::Event::kNoteOffEvent;
			midiEvent.noteOff.channel = 0;
			midiEvent.noteOff.pitch = pitch;
			midiEvent.noteOff.velocity = velocity;
			midiEvent.noteOff.noteId = pitch;
			eventList->addEvent(midiEvent);
		}
	}
}

// namespace vargason
