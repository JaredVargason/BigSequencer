//------------------------------------------------------------------------
// Copyright(c) 2024 Vargason.
//------------------------------------------------------------------------

#pragma once

#include "public.sdk/source/vst/vsteditcontroller.h"
#include "sequencer.h"
#include "bigsequencereditor.h"
#include <public.sdk/source/vst/vstguieditor.h>


namespace vargason::bigsequencer {

//------------------------------------------------------------------------
//  BigSequencerController
//------------------------------------------------------------------------
class BigSequencerController : public Steinberg::Vst::EditControllerEx1
{
public:
//------------------------------------------------------------------------
	BigSequencerController () = default;
	~BigSequencerController () SMTG_OVERRIDE = default;

    // Create function
	static Steinberg::FUnknown* createInstance (void* /*context*/)
	{
		return (Steinberg::Vst::IEditController*)new BigSequencerController;
	}

	//--- from IPluginBase -----------------------------------------------
	Steinberg::tresult PLUGIN_API initialize (Steinberg::FUnknown* context) SMTG_OVERRIDE;
	Steinberg::tresult PLUGIN_API terminate () SMTG_OVERRIDE;

	//--- from EditController --------------------------------------------
	Steinberg::tresult PLUGIN_API setComponentState (Steinberg::IBStream* state) SMTG_OVERRIDE;
	Steinberg::IPlugView* PLUGIN_API createView (Steinberg::FIDString name) SMTG_OVERRIDE;
	Steinberg::tresult PLUGIN_API setState (Steinberg::IBStream* state) SMTG_OVERRIDE;
	Steinberg::tresult PLUGIN_API getState (Steinberg::IBStream* state) SMTG_OVERRIDE;

	void PLUGIN_API editorAttached(Steinberg::Vst::EditorView* view) SMTG_OVERRIDE;
	void PLUGIN_API editorRemoved(Steinberg::Vst::EditorView* view) SMTG_OVERRIDE;
	Steinberg::tresult PLUGIN_API notify(Steinberg::Vst::IMessage* message) SMTG_OVERRIDE;

 	//---Interface---------
	DEFINE_INTERFACES
		// Here you can add more supported VST3 interfaces
		// DEF_INTERFACE (Vst::IXXX)
	END_DEFINE_INTERFACES (EditController)
    DELEGATE_REFCOUNT (EditController)

//------------------------------------------------------------------------
private:
	Sequencer sequencer;  // kind of a storage thing tbh
	BigSequencerEditor* editor = nullptr;
	void addParameters();
	bool viewAvailable = false;
};

//------------------------------------------------------------------------
} // namespace vargason
