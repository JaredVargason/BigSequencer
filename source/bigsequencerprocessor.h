//------------------------------------------------------------------------
// Copyright(c) 2024 Vargason.
//------------------------------------------------------------------------

#pragma once
#include <thread>

#include "public.sdk/source/vst/vstaudioeffect.h"
#include "sequencer.h"
#include "notedatagenerator.h"

namespace vargason::bigsequencer {

//------------------------------------------------------------------------
//  BigSequencerProcessor
//------------------------------------------------------------------------
class BigSequencerProcessor : public Steinberg::Vst::AudioEffect
{
public:
	BigSequencerProcessor ();
	~BigSequencerProcessor () SMTG_OVERRIDE;

    // Create function
	static Steinberg::FUnknown* createInstance (void* /*context*/) 
	{ 
		return (Steinberg::Vst::IAudioProcessor*)new BigSequencerProcessor;
	}

	//--- ---------------------------------------------------------------------
	// AudioEffect overrides:
	//--- ---------------------------------------------------------------------
	/** Called at first after constructor */
	Steinberg::tresult PLUGIN_API initialize (Steinberg::FUnknown* context) SMTG_OVERRIDE;
	
	/** Called at the end before destructor */
	Steinberg::tresult PLUGIN_API terminate () SMTG_OVERRIDE;
	
	/** Switch the Plug-in on/off */
	Steinberg::tresult PLUGIN_API setActive (Steinberg::TBool state) SMTG_OVERRIDE;

	/** Will be called before any process call */
	Steinberg::tresult PLUGIN_API setupProcessing (Steinberg::Vst::ProcessSetup& newSetup) SMTG_OVERRIDE;
	
	/** Asks if a given sample size is supported see SymbolicSampleSizes. */
	Steinberg::tresult PLUGIN_API canProcessSampleSize (Steinberg::int32 symbolicSampleSize) SMTG_OVERRIDE;

	/** Here we go...the process call */
	Steinberg::tresult PLUGIN_API process (Steinberg::Vst::ProcessData& data) SMTG_OVERRIDE;

	/** For persistence */
	Steinberg::tresult PLUGIN_API setState (Steinberg::IBStream* state) SMTG_OVERRIDE;
	Steinberg::tresult PLUGIN_API getState (Steinberg::IBStream* state) SMTG_OVERRIDE;

//------------------------------------------------------------------------
protected:
	vargason::bigsequencer::Sequencer* sequencer = nullptr;  // this could be a not pointer probably.
	vargason::bigsequencer::RandomNoteDataGenerator* randomNoteGenerator = nullptr;  // same.

	bool wasPreviouslyPlaying = false;  // whether the host was playing in the last frame
	float lastProjectMusicTime = 0;  // music time of the last frame

	bool hostSynced = true;  // whether sequencer starts when host playback starts
	bool retrigger = true;  // whether the sequencer starts from 0 when playback is restarted

	const uint8_t noteLowerBound = 24;
	const uint8_t noteUpperBound = 96;

	uint8_t minNote = noteLowerBound;
	uint8_t maxNote = noteUpperBound;
	Pitch rootNote = Pitch::c;
	Scale scale = Scale::chromatic;

	std::thread* messageThread = nullptr;
	std::thread* timerThread = nullptr;

	void sendMidiNoteOn(Steinberg::Vst::IEventList* eventList, uint8_t pitch, float velocity);
	void sendMidiNoteOff(Steinberg::Vst::IEventList* eventList, uint8_t pitch);

private:
	void regenerateGridNotes();
	void handleParameterChanges(Steinberg::Vst::ProcessData& data);
	void updateSequencer(Steinberg::Vst::ProcessData& data);
	void updateCursor(Steinberg::Vst::ProcessData& data, Cursor& cursor);

	// sending GUI updates
	void messageSender();
	void sendDataToController();
	void getSequencerData(std::vector<char>& sequencerData);
};

//------------------------------------------------------------------------
} // namespace vargason
