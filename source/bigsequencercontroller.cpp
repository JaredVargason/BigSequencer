//------------------------------------------------------------------------
// Copyright(c) 2024 Vargason.
//------------------------------------------------------------------------

#include "bigsequencercontroller.h"
#include "bigsequencercids.h"
#include "base/source/fstreamer.h"
#include "vstgui/plugin-bindings/vst3editor.h"
#include "util.h"
#include "plugids.h"
#include "scales.h"
#include "notedatagenerator.h"

using namespace Steinberg;

namespace vargason::bigsequencer {

//------------------------------------------------------------------------
// BigSequencerController Implementation
//------------------------------------------------------------------------
tresult PLUGIN_API BigSequencerController::initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated

	//---do not forget to call parent ------
	tresult result = EditControllerEx1::initialize (context);
	if (result != kResultOk)
	{
		return result;
	}

	addParameters();

	// Generate default grid
	RandomNoteDataGenerator randomNoteGenerator;
	NoteData* noteData = randomNoteGenerator.generate(Sequencer::defaultWidth, Sequencer::defaultHeight, NoteDataGenerator::defaultPitch, NoteDataGenerator::defaultScale, NoteDataGenerator::defaultMinNote, NoteDataGenerator::defaultMaxNote);
	sequencer.setNotes(Sequencer::defaultWidth, Sequencer::defaultHeight, noteData);

	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API BigSequencerController::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!
	//---do not forget to call parent ------
	return EditControllerEx1::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API BigSequencerController::setComponentState (IBStream* state)
{
	// Here you get the state of the component (Processor part)
	if (!state) {
		return kResultFalse;
	}

	IBStreamer streamer(state, kLittleEndian);

	// Read sequencer deets
	uint8_t width = 0;
	if (!streamer.readInt8u(width)) {
		return kResultFalse;
	}
	uint8_t height = 0;
	if (!streamer.readInt8u(height)) {
		return kResultFalse;
	}

	NoteData* noteDatas = new NoteData[width * height];
	bool failure = false;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {

			bool active = false;
			char pitch = 0;

			if (!streamer.readBool(active)) {
				failure = true;
				break;
			}
			if (!streamer.readInt8(pitch)) {
				failure = true;
				break;
			}

			noteDatas[y * width + x].active = active;
			noteDatas[y * width + x].pitch = pitch;
		}
		if (failure) {
			break;
		}
	}

	if (failure) {
		delete[] noteDatas;
		return kResultFalse;
	}

	// Write cursors
	Cursor cursors[sequencer.maxNumCursors];
	for (int i = 0; i < sequencer.maxNumCursors; i++) {
		Cursor& cursor = cursors[i];
		if (!streamer.readBool(cursor.active)) {
			failure = true;
			break;
		}

		Steinberg::uint8 interval;
		if (!streamer.readInt8u(interval)) {
			failure = true;
			break;
		}
		cursor.interval = (Interval)interval;

		Steinberg::int8 pitchOffset;
		if (!streamer.readInt8(pitchOffset)) {
			failure = true;
			break;
		}
		cursor.pitchOffset = pitchOffset;

		float noteLength;
		if (!streamer.readFloat(noteLength)) {
			failure = true;
			break;
		}
		cursor.setNoteLength(noteLength);

		if (!streamer.readFloat(cursor.velocity)) {
			failure = true;
			break;
		}

		if (!streamer.readFloat(cursor.probability)) {
			failure = true;
			break;
		}
	}

	if (failure) {
		return kResultFalse;
	}

	uint32_t seed;
	uint8_t scale;
	uint8_t rootNote;
	uint8_t minNote;
	uint8_t maxNote;
	float fillChance;

	if (!streamer.readInt32u(seed)) {
		return kResultFalse;
	}

	if (!streamer.readInt8u(scale)) {
		return kResultFalse;
	}

	if (!streamer.readInt8u(rootNote)) {
		return kResultFalse;
	}

	if (!streamer.readInt8u(minNote)) {
		return kResultFalse;
	}

	if (!streamer.readInt8u(maxNote)) {
		return kResultFalse;
	}

	if (!streamer.readFloat(fillChance)) {
		return kResultFalse;
	}

	// copy over all data on read success
	for (int i = 0; i < sequencer.maxNumCursors; i++) {
		Cursor& cursor = sequencer.getCursor(i);
		cursor.active = cursors[i].active;
		cursor.interval = cursors[i].interval;
		cursor.pitchOffset = cursors[i].pitchOffset;
		cursor.setNoteLength(cursors[i].getNoteLength());
		cursor.velocity = cursors[i].velocity;
		cursor.probability = cursors[i].probability;
	}
	sequencer.setNotes(width, height, noteDatas);

	setParamNormalized(kParamSequencerWidthId, ilerp<uint8_t>(Sequencer::minWidth, Sequencer::maxWidth, width));
	setParamNormalized(kParamSequencerHeightId, ilerp<uint8_t>(Sequencer::minHeight, Sequencer::maxHeight, height));

	setParamNormalized(kParamCursor1ActiveId, cursors[0].active);
	setParamNormalized(kParamCursor1NoteIntervalId, ilerp<uint8_t>(0, totalIntervals - 1, cursors[0].interval));
	setParamNormalized(kParamCursor1PitchOffsetId, ilerp(Cursor::pitchMin, Cursor::pitchMax, cursors[0].pitchOffset));
	setParamNormalized(kParamCursor1NoteLengthId, cursors[0].getNoteLength());
	setParamNormalized(kParamCursor1VelocityId, cursors[0].velocity);
	setParamNormalized(kParamCursor1ProbabilityId, cursors[0].probability);

	setParamNormalized(kParamCursor2ActiveId, cursors[1].active);
	setParamNormalized(kParamCursor2NoteIntervalId, ilerp<uint8_t>(0, totalIntervals - 1, cursors[1].interval));
	setParamNormalized(kParamCursor2PitchOffsetId, ilerp(Cursor::pitchMin, Cursor::pitchMax, cursors[1].pitchOffset));
	setParamNormalized(kParamCursor2NoteLengthId, cursors[1].getNoteLength());
	setParamNormalized(kParamCursor2VelocityId, cursors[1].velocity);
	setParamNormalized(kParamCursor2ProbabilityId, cursors[1].probability);

	setParamNormalized(kParamCursor3ActiveId, cursors[2].active);
	setParamNormalized(kParamCursor3NoteIntervalId, ilerp<uint8_t>(0, totalIntervals - 1, cursors[2].interval));
	setParamNormalized(kParamCursor3PitchOffsetId, ilerp(Cursor::pitchMin, Cursor::pitchMax, cursors[2].pitchOffset));
	setParamNormalized(kParamCursor3NoteLengthId, cursors[2].getNoteLength());
	setParamNormalized(kParamCursor3VelocityId, cursors[2].velocity);
	setParamNormalized(kParamCursor3ProbabilityId, cursors[2].probability);

	setParamNormalized(kParamCursor4ActiveId, cursors[3].active);
	setParamNormalized(kParamCursor4NoteIntervalId, ilerp<uint8_t>(0, totalIntervals - 1, cursors[3].interval));
	setParamNormalized(kParamCursor4PitchOffsetId, ilerp(Cursor::pitchMin, Cursor::pitchMax, cursors[3].pitchOffset));
	setParamNormalized(kParamCursor4NoteLengthId, cursors[3].getNoteLength());
	setParamNormalized(kParamCursor4VelocityId, cursors[3].velocity);
	setParamNormalized(kParamCursor4ProbabilityId, cursors[3].probability);

	setParamNormalized(kParamSeedId, (double)seed / INT32_MAX);
	setParamNormalized(kParamScaleId, scale / (double)(totalScales - 1));
	setParamNormalized(kParamRootNoteId, rootNote / (double)(totalPitches - 1));
	setParamNormalized(kParamMinNoteId, ilerp<uint8_t>(NoteDataGenerator::noteLowerBound, NoteDataGenerator::noteUpperBound, minNote));
	setParamNormalized(kParamMaxNoteId, ilerp<uint8_t>(NoteDataGenerator::noteLowerBound, NoteDataGenerator::noteUpperBound, maxNote));
	setParamNormalized(kParamFillChanceId, fillChance);

	if (viewAvailable) {
		this->editor->setSequencerViewInvalid();
	}

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API BigSequencerController::setState (IBStream* state)
{
	// Here you get the state of the controller

	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API BigSequencerController::getState (IBStream* state)
{
	// Here you are asked to deliver the state of the controller (if needed)
	// Note: the real state of your plug-in is saved in the processor

	return kResultTrue;
}

//------------------------------------------------------------------------
IPlugView* PLUGIN_API BigSequencerController::createView (FIDString name)
{
	// Here the Host wants to open your editor (if you have one)
	if (FIDStringsEqual (name, Vst::ViewType::kEditor))
	{
		// create your editor here and return a IPlugView ptr of it
		editor = new BigSequencerEditor(this, "view", "bigsequencereditor.uidesc", sequencer);
		return (IPlugView*)editor;
	}
	return nullptr;
}

void BigSequencerController::addParameters()
{
	Vst::RangeParameter* sequencerWidthParameter = new Vst::RangeParameter(STR16("Sequencer Width"), kParamSequencerWidthId, nullptr, Sequencer::minWidth, Sequencer::maxWidth, Sequencer::defaultWidth, Sequencer::maxWidth - Sequencer::minWidth, 0);
	parameters.addParameter(sequencerWidthParameter);

	Vst::RangeParameter* sequencerHeightParameter = new Vst::RangeParameter(STR16("Sequencer Height"), kParamSequencerHeightId, nullptr, Sequencer::minHeight, Sequencer::maxHeight, Sequencer::defaultHeight, Sequencer::maxHeight - Sequencer::minHeight, 0);
	parameters.addParameter(sequencerHeightParameter);

	parameters.addParameter(STR16("Retrigger"), nullptr, 0, true, 0, kParamRetriggerId);

	// Cursor 1
	parameters.addParameter(STR16("Cursor 1 Active"), nullptr, 0, defaultCursors[0].active, Vst::ParameterInfo::kCanAutomate, kParamCursor1ActiveId);
	parameters.addParameter(STR16("Cursor 1 Note Length"), nullptr, 0, defaultCursors[0].getNoteLength(), Vst::ParameterInfo::kCanAutomate, kParamCursor1NoteLengthId);

	Vst::RangeParameter* cursor1PitchOffsetParameter = new Vst::RangeParameter(STR16("Cursor 1 Pitch Offset"), kParamCursor1PitchOffsetId, nullptr, Cursor::pitchMin, Cursor::pitchMax, 0, Cursor::pitchMax - Cursor::pitchMin, 0);
	parameters.addParameter(cursor1PitchOffsetParameter);

	Vst::StringListParameter* cursor1NoteIntervalParameter = new Vst::StringListParameter(STR16("Cursor 1 Note Interval"), kParamCursor1NoteIntervalId, nullptr, 0);
	cursor1NoteIntervalParameter->appendString(STR16("1/32"));
	cursor1NoteIntervalParameter->appendString(STR16("1/16"));
	cursor1NoteIntervalParameter->appendString(STR16("1/8"));
	cursor1NoteIntervalParameter->appendString(STR16("1/4"));
	cursor1NoteIntervalParameter->appendString(STR16("1/2"));
	cursor1NoteIntervalParameter->appendString(STR16("1/1"));
	cursor1NoteIntervalParameter->appendString(STR16("2/1"));
	cursor1NoteIntervalParameter->setNormalized(normalizedInterval(defaultCursors[0].interval));
	parameters.addParameter(cursor1NoteIntervalParameter);

	parameters.addParameter(STR16("Cursor 1 Velocity"), nullptr, 0, defaultCursors[0].velocity, Vst::ParameterInfo::kCanAutomate, kParamCursor1VelocityId);
	parameters.addParameter(STR16("Cursor 1 Probability"), nullptr, 0, defaultCursors[0].probability, Vst::ParameterInfo::kCanAutomate, kParamCursor1ProbabilityId);

	Vst::RangeParameter* cursor1StartPosition = new Vst::RangeParameter(STR16("Cursor 1 Start Position"), kParamCursor1StartPositionId, nullptr, 0, Sequencer::maxWidth * Sequencer::maxHeight - 1, 0, Sequencer::maxWidth * Sequencer::maxHeight - 2);
	parameters.addParameter(cursor1StartPosition);

	// Cursor 2
	parameters.addParameter(STR16("Cursor 2 Active"), nullptr, 0, defaultCursors[1].active, Vst::ParameterInfo::kCanAutomate, kParamCursor2ActiveId);
	parameters.addParameter(STR16("Cursor 2 Note Length"), nullptr, 0, defaultCursors[1].getNoteLength(), Vst::ParameterInfo::kCanAutomate, kParamCursor2NoteLengthId);

	Vst::RangeParameter* cursor2OctaveOffsetParameter = new Vst::RangeParameter(STR16("Cursor 2 Pitch Offset"), kParamCursor2PitchOffsetId, nullptr, Cursor::pitchMin, Cursor::pitchMax, -12, Cursor::pitchMax - Cursor::pitchMin, 0);
	parameters.addParameter(cursor2OctaveOffsetParameter);

	Vst::StringListParameter* cursor2NoteIntervalParameter = new Vst::StringListParameter(STR16("Cursor 2 Note Interval"), kParamCursor2NoteIntervalId, nullptr, 0);
	cursor2NoteIntervalParameter->appendString(STR16("1/32"));
	cursor2NoteIntervalParameter->appendString(STR16("1/16"));
	cursor2NoteIntervalParameter->appendString(STR16("1/8"));
	cursor2NoteIntervalParameter->appendString(STR16("1/4"));
	cursor2NoteIntervalParameter->appendString(STR16("1/2"));
	cursor2NoteIntervalParameter->appendString(STR16("1/1"));
	cursor2NoteIntervalParameter->appendString(STR16("2/1"));
	cursor2NoteIntervalParameter->setNormalized(normalizedInterval(defaultCursors[1].interval));
	parameters.addParameter(cursor2NoteIntervalParameter);

	parameters.addParameter(STR16("Cursor 2 Velocity"), nullptr, 0, defaultCursors[1].velocity, Vst::ParameterInfo::kCanAutomate, kParamCursor2VelocityId);
	parameters.addParameter(STR16("Cursor 2 Probability"), nullptr, 0, defaultCursors[1].probability, Vst::ParameterInfo::kCanAutomate, kParamCursor2ProbabilityId);

	Vst::RangeParameter* cursor2StartPosition = new Vst::RangeParameter(STR16("Cursor 2 Start Position"), kParamCursor2StartPositionId, nullptr, 0, Sequencer::maxWidth * Sequencer::maxHeight - 1, 0, Sequencer::maxWidth * Sequencer::maxHeight - 2);
	parameters.addParameter(cursor2StartPosition);

	// Cursor 3, inactive by default
	parameters.addParameter(STR16("Cursor 3 Active"), nullptr, 0, defaultCursors[2].active, Vst::ParameterInfo::kCanAutomate, kParamCursor3ActiveId);
	parameters.addParameter(STR16("Cursor 3 Note Length"), nullptr, 0, defaultCursors[2].getNoteLength(), Vst::ParameterInfo::kCanAutomate, kParamCursor3NoteLengthId);

	Vst::RangeParameter* cursor3OctaveOffsetParameter = new Vst::RangeParameter(STR16("Cursor 3 Pitch Offset"), kParamCursor3PitchOffsetId, nullptr, Cursor::pitchMin, Cursor::pitchMax, 12, Cursor::pitchMax - Cursor::pitchMin, 0);
	parameters.addParameter(cursor3OctaveOffsetParameter);

	Vst::StringListParameter* cursor3NoteIntervalParameter = new Vst::StringListParameter(STR16("Cursor 3 Note Interval"), kParamCursor3NoteIntervalId, nullptr, 0);
	cursor3NoteIntervalParameter->appendString(STR16("1/32"));
	cursor3NoteIntervalParameter->appendString(STR16("1/16"));
	cursor3NoteIntervalParameter->appendString(STR16("1/8"));
	cursor3NoteIntervalParameter->appendString(STR16("1/4"));
	cursor3NoteIntervalParameter->appendString(STR16("1/2"));
	cursor3NoteIntervalParameter->appendString(STR16("1/1"));
	cursor3NoteIntervalParameter->appendString(STR16("2/1"));
	cursor3NoteIntervalParameter->setNormalized(normalizedInterval(defaultCursors[2].interval));
	parameters.addParameter(cursor3NoteIntervalParameter);

	parameters.addParameter(STR16("Cursor 3 Velocity"), nullptr, 0, defaultCursors[2].velocity, Vst::ParameterInfo::kCanAutomate, kParamCursor3VelocityId);
	parameters.addParameter(STR16("Cursor 3 Probability"), nullptr, 0, defaultCursors[2].probability, Vst::ParameterInfo::kCanAutomate, kParamCursor3ProbabilityId);

	Vst::RangeParameter* cursor3StartPosition = new Vst::RangeParameter(STR16("Cursor 3 Start Position"), kParamCursor3StartPositionId, nullptr, 0, Sequencer::maxWidth * Sequencer::maxHeight - 1, 0, Sequencer::maxWidth * Sequencer::maxHeight - 2);
	parameters.addParameter(cursor3StartPosition);

	// Cursor 4, inactive by default
	parameters.addParameter(STR16("Cursor 4 Active"), nullptr, 0, defaultCursors[3].active, Vst::ParameterInfo::kCanAutomate, kParamCursor4ActiveId);
	parameters.addParameter(STR16("Cursor 4 Note Length"), nullptr, 0, defaultCursors[3].getNoteLength(), Vst::ParameterInfo::kCanAutomate, kParamCursor4NoteLengthId);

	Vst::RangeParameter* cursor4OctaveOffsetParameter = new Vst::RangeParameter(STR16("Cursor 4 Pitch Offset"), kParamCursor4PitchOffsetId, nullptr, Cursor::pitchMin, Cursor::pitchMax, -24, Cursor::pitchMax - Cursor::pitchMin, 0);
	parameters.addParameter(cursor4OctaveOffsetParameter);

	Vst::StringListParameter* cursor4NoteIntervalParameter = new Vst::StringListParameter(STR16("Cursor 4 Note Interval"), kParamCursor4NoteIntervalId, nullptr, 0);
	cursor4NoteIntervalParameter->appendString(STR16("1/32"));
	cursor4NoteIntervalParameter->appendString(STR16("1/16"));
	cursor4NoteIntervalParameter->appendString(STR16("1/8"));
	cursor4NoteIntervalParameter->appendString(STR16("1/4"));
	cursor4NoteIntervalParameter->appendString(STR16("1/2"));
	cursor4NoteIntervalParameter->appendString(STR16("1/1"));
	cursor4NoteIntervalParameter->appendString(STR16("2/1"));
	cursor4NoteIntervalParameter->setNormalized(normalizedInterval(defaultCursors[3].interval));
	parameters.addParameter(cursor4NoteIntervalParameter);

	parameters.addParameter(STR16("Cursor 4 Velocity"), nullptr, 0, defaultCursors[3].velocity, Vst::ParameterInfo::kCanAutomate, kParamCursor4VelocityId);
	parameters.addParameter(STR16("Cursor 4 Probability"), nullptr, 0, defaultCursors[3].probability, Vst::ParameterInfo::kCanAutomate, kParamCursor4ProbabilityId);

	Vst::RangeParameter* cursor4StartPosition = new Vst::RangeParameter(STR16("Cursor 4 Start Position"), kParamCursor4StartPositionId, nullptr, 0, Sequencer::maxWidth * Sequencer::maxHeight - 1, 0, Sequencer::maxWidth * Sequencer::maxHeight - 2);
	parameters.addParameter(cursor4StartPosition);

	// Note generation

	Vst::StringListParameter* rootNoteParameter = new Vst::StringListParameter(STR16("Root Note"), kParamRootNoteId, nullptr, 0);
	rootNoteParameter->appendString(STR16("C"));
	rootNoteParameter->appendString(STR16("C#"));
	rootNoteParameter->appendString(STR16("D"));
	rootNoteParameter->appendString(STR16("D#"));
	rootNoteParameter->appendString(STR16("E"));
	rootNoteParameter->appendString(STR16("F"));
	rootNoteParameter->appendString(STR16("F#"));
	rootNoteParameter->appendString(STR16("G"));
	rootNoteParameter->appendString(STR16("G#"));
	rootNoteParameter->appendString(STR16("A"));
	rootNoteParameter->appendString(STR16("A#"));
	rootNoteParameter->appendString(STR16("B"));
	parameters.addParameter(rootNoteParameter);

	Vst::StringListParameter* scaleParameter = new Vst::StringListParameter(STR16("Scale"), kParamScaleId, nullptr, 0);
	scaleParameter->appendString(STR16("Chromatic"));
	scaleParameter->appendString(STR16("Major"));
	scaleParameter->appendString(STR16("Major Pentatonic"));
	scaleParameter->appendString(STR16("Harmonic Minor"));
	scaleParameter->appendString(STR16("Melodic Minor"));
	scaleParameter->appendString(STR16("Dorian"));
	scaleParameter->appendString(STR16("Phrygian"));
	scaleParameter->appendString(STR16("Lydian"));
	scaleParameter->appendString(STR16("Mixolydian"));
	scaleParameter->appendString(STR16("Minor"));
	scaleParameter->appendString(STR16("Locrian"));
	scaleParameter->appendString(STR16("Blues"));
	scaleParameter->appendString(STR16("Augmented"));
	scaleParameter->appendString(STR16("Prometheus"));
	scaleParameter->appendString(STR16("Hirajoshi"));
	scaleParameter->appendString(STR16("Enigmatic"));
	scaleParameter->appendString(STR16("Gypsy"));
	scaleParameter->appendString(STR16("Super Locrian"));
	parameters.addParameter(scaleParameter);

	Vst::RangeParameter* minNoteParameter = new Vst::RangeParameter(STR16("Min Note"), kParamMinNoteId, nullptr, NoteDataGenerator::noteLowerBound, NoteDataGenerator::noteUpperBound, NoteDataGenerator::defaultMinNote, NoteDataGenerator::noteUpperBound - NoteDataGenerator::noteLowerBound, 0);
	parameters.addParameter(minNoteParameter);

	Vst::RangeParameter* maxNoteParameter = new Vst::RangeParameter(STR16("Max Note"), kParamMaxNoteId, nullptr, NoteDataGenerator::noteLowerBound, NoteDataGenerator::noteUpperBound, NoteDataGenerator::defaultMaxNote, NoteDataGenerator::noteUpperBound - NoteDataGenerator::noteLowerBound, 0);
	parameters.addParameter(maxNoteParameter);

	parameters.addParameter(STR16("Fill Chance"), nullptr, 0, NoteDataGenerator::defaultFillChance, 0, kParamFillChanceId);
	Vst::RangeParameter* seedParameter = new Vst::RangeParameter(STR16("Seed"), kParamSeedId, nullptr, 0, INT32_MAX, 0, INT32_MAX - 1, 0);
	parameters.addParameter(seedParameter);
	parameters.addParameter(STR16("Use Random Seed"), nullptr, 1, true, 0, kParamUseRandomSeedId);

	parameters.addParameter(STR16("Cursor Tab"), nullptr, 0, 0, 0, kParamCursorTab);
}

void PLUGIN_API BigSequencerController::editorAttached(Steinberg::Vst::EditorView* view) {
	Steinberg::Vst::EditControllerEx1::editorAttached(view);
	viewAvailable = true;
	this->editor->setSequencerViewInvalid();
}

void PLUGIN_API BigSequencerController::editorRemoved(Steinberg::Vst::EditorView* view) {
	Steinberg::Vst::EditControllerEx1::editorRemoved(view);
	viewAvailable = false;
}

tresult PLUGIN_API BigSequencerController::notify(Steinberg::Vst::IMessage* message) {
	if (!viewAvailable) {
		return kResultOk;
	}
	std::string mID = message->getMessageID();
	if (mID == "SequencerMessage") {
		const void* data = nullptr;
		uint32_t size = 0;
		message->getAttributes()->getBinary("sequencer", data, size);
		const char* byteData = static_cast<const char*>(data);
		std::vector<char> serializedData(byteData, byteData + size);

		int index = 0;
		uint8_t width = serializedData[index++];
		uint8_t height = serializedData[index++];
		if (width != sequencer.getWidth() || height != sequencer.getHeight()) {
			sequencer.setSize(width, height);
		}

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				NoteData& noteData = sequencer.getNote(x, y);
				noteData.active = serializedData[index++];
				noteData.pitch = serializedData[index++];
			}
		}

		this->editor->setSequencerViewInvalid();
	}
	else if (mID == "CursorMessage") {
		Steinberg::int64 cursorIndex, cursorPosition;
		message->getAttributes()->getInt("index", cursorIndex);
		message->getAttributes()->getInt("position", cursorPosition);
		
		sequencer.getCursor(cursorIndex).position = cursorPosition;
		this->editor->setSequencerViewInvalid();
	}
	else if (mID == "CursorActiveMessage") {
		Steinberg::int64 cursorIndex, cursorActive, cursorPosition;
		message->getAttributes()->getInt("index", cursorIndex);
		message->getAttributes()->getInt("active", cursorActive);
		message->getAttributes()->getInt("position", cursorPosition);
		Cursor& cursor = sequencer.getCursor(cursorIndex);
		cursor.active = cursorActive;
		cursor.position = cursorPosition;
		this->editor->setSequencerViewInvalid();
	}
	message->release();
	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace vargason
