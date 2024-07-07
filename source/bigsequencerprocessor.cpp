//------------------------------------------------------------------------
// Copyright(c) 2024 Vargason.
//------------------------------------------------------------------------

#include "bigsequencerprocessor.h"
#include "bigsequencercids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstevents.h"

using namespace Steinberg;

namespace vargason::bigsequencer {
//------------------------------------------------------------------------
// BigSequencerProcessor
//------------------------------------------------------------------------
BigSequencerProcessor::BigSequencerProcessor ()
{
	//--- set the wanted controller for our processor
	setControllerClass (kBigSequencerControllerUID);

	this->sequencer = new Sequencer();
}

//------------------------------------------------------------------------
BigSequencerProcessor::~BigSequencerProcessor ()
{}

//------------------------------------------------------------------------
tresult PLUGIN_API BigSequencerProcessor::initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated
	
	//---always initialize the parent-------
	tresult result = AudioEffect::initialize (context);
	// if everything Ok, continue
	if (result != kResultOk)
	{
		return result;
	}

	//--- create Audio IO ------
	// addAudioInput (STR16 ("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
	addAudioOutput (STR16 ("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);

	/* If you don't need an event bus, you can remove the next line */
	addEventInput (STR16 ("Event In"), 1);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API BigSequencerProcessor::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!
	delete sequencer;
	//---do not forget to call parent ------
	return AudioEffect::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API BigSequencerProcessor::setActive (TBool state)
{
	//--- called when the Plug-in is enable/disable (On/Off) -----
	return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API BigSequencerProcessor::process (Vst::ProcessData& data)
{
	if (data.processContext == nullptr) {
		return kResultOk;
	}

	double quarterNotes = data.processContext->projectTimeMusic;

	if (quarterNotes >= sequencer->getCursor() + 1.0) {
		Vst::IEventList* eventList = data.outputEvents;
		if (eventList) {
			Vst::Event midiEvent = { 0 };
			midiEvent.busIndex = 0;
			midiEvent.type = Vst::Event::kNoteOnEvent;
			midiEvent.sampleOffset = 0;
			midiEvent.noteOn.channel = 0;
			midiEvent.noteOn.pitch = 60;
			midiEvent.noteOn.velocity = 0.40;
			midiEvent.noteOn.noteId = 60;
			eventList->addEvent(midiEvent);
		}
		sequencer->setCursor(quarterNotes);
	}
	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API BigSequencerProcessor::setupProcessing (Vst::ProcessSetup& newSetup)
{
	//--- called before any processing ----
	return AudioEffect::setupProcessing (newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API BigSequencerProcessor::canProcessSampleSize (int32 symbolicSampleSize)
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
tresult PLUGIN_API BigSequencerProcessor::setState (IBStream* state)
{
	if (!state) {
		return kResultFalse;
	}
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

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API BigSequencerProcessor::getState (IBStream* state)
{
	// here we need to save the model
	IBStreamer streamer (state, kLittleEndian);

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

	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace vargason
