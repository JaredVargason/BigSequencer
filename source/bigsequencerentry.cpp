//------------------------------------------------------------------------
// Copyright(c) 2024 Vargason.
//------------------------------------------------------------------------

#include "bigsequencerprocessor.h"
#include "bigsequencercontroller.h"
#include "bigsequencercids.h"
#include "version.h"

#include "public.sdk/source/main/pluginfactory.h"

#define stringPluginName "BigSequencer"

using namespace Steinberg::Vst;
using namespace vargason::bigsequencer;

//------------------------------------------------------------------------
//  VST Plug-in Entry
//------------------------------------------------------------------------

BEGIN_FACTORY_DEF ("Vargason", 
			       "www.jaredvargason.com", 
			       "mailto:jaredvargason@gmail.com")

	//---First Plug-in included in this factory-------
	// its kVstAudioEffectClass component
	DEF_CLASS2 (INLINE_UID_FROM_FUID(kBigSequencerProcessorUID),
				PClassInfo::kManyInstances,	// cardinality
				kVstAudioEffectClass,	// the component category (do not changed this)
				stringPluginName,		// here the Plug-in name (to be changed)
				Vst::kDistributable,	// means that component and controller could be distributed on different computers
				BigSequencerVST3Category, // Subcategory for this Plug-in (to be changed)
				FULL_VERSION_STR,		// Plug-in version (to be changed)
				kVstVersionString,		// the VST 3 SDK version (do not changed this, use always this define)
				BigSequencerProcessor::createInstance)	// function pointer called when this component should be instantiated

	// its kVstComponentControllerClass component
	DEF_CLASS2 (INLINE_UID_FROM_FUID (kBigSequencerControllerUID),
				PClassInfo::kManyInstances, // cardinality
				kVstComponentControllerClass,// the Controller category (do not changed this)
				stringPluginName "Controller",	// controller name (could be the same than component name)
				0,						// not used here
				"",						// not used here
				FULL_VERSION_STR,		// Plug-in version (to be changed)
				kVstVersionString,		// the VST 3 SDK version (do not changed this, use always this define)
				BigSequencerController::createInstance)// function pointer called when this component should be instantiated

	//----for others Plug-ins contained in this factory, put like for the first Plug-in different DEF_CLASS2---

END_FACTORY
