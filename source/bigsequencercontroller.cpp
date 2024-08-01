//------------------------------------------------------------------------
// Copyright(c) 2024 Vargason.
//------------------------------------------------------------------------

#include "bigsequencercontroller.h"
#include "bigsequencercids.h"
#include "vstgui/plugin-bindings/vst3editor.h"
#include "plugids.h"
#include "scales.h"

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

	// Here you could register some parameters

	Vst::RangeParameter* sequencerWidthParameter = new Vst::RangeParameter(STR16("Sequencer Width"), kParamSequencerWidthId, nullptr, 1, 32, 8, 0, 0);
	parameters.addParameter(sequencerWidthParameter);

	Vst::RangeParameter* sequencerHeightParameter = new Vst::RangeParameter(STR16("Sequencer Height"), kParamSequencerHeightId, nullptr, 1, 32, 4, 0, 0);
	parameters.addParameter(sequencerHeightParameter);

	parameters.addParameter(STR16("Host Sync"), nullptr, 0, 1.0, 0, kParamHostSyncId);
	parameters.addParameter(STR16("Retrigger"), nullptr, 0, 1.0, 0, kParamRetriggerId);

	// Cursor 1
	parameters.addParameter(STR16("Cursor 1 Active"), nullptr, 0, 1.0, Vst::ParameterInfo::kCanAutomate, kParamCursor1ActiveId);
	parameters.addParameter(STR16("Cursor 1 Note Length"), nullptr, 0, 0.4, Vst::ParameterInfo::kCanAutomate, kParamCursor1NoteLengthId);

	Vst::RangeParameter* cursor1PitchOffsetParameter = new Vst::RangeParameter(STR16("Cursor 1 Pitch Offset"), kParamCursor1PitchOffsetId, nullptr, -24, 24, 0, 0, 0);
	parameters.addParameter(cursor1PitchOffsetParameter);

	Vst::RangeParameter* cursor1NoteIntervalParameter = new Vst::RangeParameter(STR16("Cursor 1 Note Interval"), kParamCursor1NoteIntervalId, nullptr, 0, Interval::doubleWholeNote, Interval::quarterNote, 0, 0);
	parameters.addParameter(cursor1NoteIntervalParameter);

	parameters.addParameter(STR16("Cursor 1 Velocity"), nullptr, 0, 0.6, Vst::ParameterInfo::kCanAutomate, kParamCursor1VelocityId);

	// Cursor 2
	parameters.addParameter(STR16("Cursor 2 Active"), nullptr, 0, 1.0, Vst::ParameterInfo::kCanAutomate, kParamCursor2ActiveId);
	parameters.addParameter(STR16("Cursor 2 Note Length"), nullptr, 0, 0.6, Vst::ParameterInfo::kCanAutomate, kParamCursor2NoteLengthId);

	Vst::RangeParameter* cursor2OctaveOffsetParameter = new Vst::RangeParameter(STR16("Cursor 2 Pitch Offset"), kParamCursor2PitchOffsetId, nullptr, -24, 24, -12, 0, 0);
	parameters.addParameter(cursor2OctaveOffsetParameter);

	Vst::RangeParameter* cursor2NoteIntervalParameter = new Vst::RangeParameter(STR16("Cursor 2 Note Interval"), kParamCursor2NoteIntervalId, nullptr, 0, Interval::doubleWholeNote, Interval::halfNote, 0, 0);
	parameters.addParameter(cursor2NoteIntervalParameter);

	parameters.addParameter(STR16("Cursor 2 Velocity"), nullptr, 0, 0.6, Vst::ParameterInfo::kCanAutomate, kParamCursor2VelocityId);

	// Cursor 3, inactive by default
	parameters.addParameter(STR16("Cursor 3 Active"), nullptr, 0, 0, Vst::ParameterInfo::kCanAutomate, kParamCursor3ActiveId);
	parameters.addParameter(STR16("Cursor 3 Note Length"), nullptr, 0, 0.6, Vst::ParameterInfo::kCanAutomate, kParamCursor3NoteLengthId);

	Vst::RangeParameter* cursor3OctaveOffsetParameter = new Vst::RangeParameter(STR16("Cursor 3 Pitch Offset"), kParamCursor3PitchOffsetId, nullptr, -24, 24, 12, 0, 0);
	parameters.addParameter(cursor3OctaveOffsetParameter);

	Vst::RangeParameter* cursor3NoteIntervalParameter = new Vst::RangeParameter(STR16("Cursor 3 Note Interval"), kParamCursor3NoteIntervalId, nullptr, 0, Interval::doubleWholeNote, Interval::eighthNote, 0, 0);
	parameters.addParameter(cursor3NoteIntervalParameter);

	parameters.addParameter(STR16("Cursor 3 Velocity"), nullptr, 0, 0.6, Vst::ParameterInfo::kCanAutomate, kParamCursor3VelocityId);

	// Cursor 4, inactive by default
	parameters.addParameter(STR16("Cursor 4 Active"), nullptr, 0, 0, Vst::ParameterInfo::kCanAutomate, kParamCursor4ActiveId);
	parameters.addParameter(STR16("Cursor 4 Note Length"), nullptr, 0, 0.4, Vst::ParameterInfo::kCanAutomate, kParamCursor4NoteLengthId);

	Vst::RangeParameter* cursor4OctaveOffsetParameter = new Vst::RangeParameter(STR16("Cursor 4 Pitch Offset"), kParamCursor4PitchOffsetId, nullptr, -24, 24, -24, 0, 0);
	parameters.addParameter(cursor4OctaveOffsetParameter);

	Vst::RangeParameter* cursor4NoteIntervalParameter = new Vst::RangeParameter(STR16("Cursor 4 Note Interval"), kParamCursor4NoteIntervalId, nullptr, 0, Interval::doubleWholeNote, Interval::wholeNote, 0, 0);
	parameters.addParameter(cursor4NoteIntervalParameter);

	parameters.addParameter(STR16("Cursor 4 Velocity"), nullptr, 0, 0.6, Vst::ParameterInfo::kCanAutomate, kParamCursor4VelocityId);

	// "fake" parameters
	Vst::RangeParameter* rootNoteParameter = new Vst::RangeParameter(STR16("Root Note"), kParamRootNoteId, nullptr, Pitch::c, Pitch::b, Pitch::c, 0, 0);
	parameters.addParameter(rootNoteParameter);

	Vst::StringListParameter* stringListParameter = new Vst::StringListParameter(STR16("Scale"), kParamScaleId, nullptr, 0);
	stringListParameter->appendString(STR16("Chromatic"));
	stringListParameter->appendString(STR16("Major"));
	stringListParameter->appendString(STR16("Major Pentatonic"));
	stringListParameter->appendString(STR16("Harmonic Minor"));
	stringListParameter->appendString(STR16("Melodic Minor"));
	stringListParameter->appendString(STR16("Dorian"));
	stringListParameter->appendString(STR16("Phrygian"));
	stringListParameter->appendString(STR16("Lydian"));
	stringListParameter->appendString(STR16("Mixolydian"));
	stringListParameter->appendString(STR16("Aeolian"));
	stringListParameter->appendString(STR16("Locrian"));
	parameters.addParameter(stringListParameter);

	Vst::RangeParameter* minNoteParameter = new Vst::RangeParameter(STR16("Min Note"), kParamMinNoteId, nullptr, 24, 96, 56, 0, 0);
	parameters.addParameter(minNoteParameter);

	Vst::RangeParameter* maxNoteParameter = new Vst::RangeParameter(STR16("Max Note"), kParamMaxNoteId, nullptr, 24, 96, 76, 0, 0);
	parameters.addParameter(maxNoteParameter);

	parameters.addParameter(STR16("Fill Chance"), nullptr, 0, .5f, 0, kParamFillChanceId);

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
	if (!state)
		return kResultFalse;

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
		auto* view = new VSTGUI::VST3Editor (this, "view", "bigsequencereditor.uidesc");
		return view;
	}
	return nullptr;
}

//------------------------------------------------------------------------
} // namespace vargason
