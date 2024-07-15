#pragma once

#include "pluginterfaces/vst/vsttypes.h"

namespace vargason::bigsequencer {
	enum SequencerParams : Steinberg::Vst::ParamID {
		kParamWidthId = 100,
		kParamHeightId = 101,
		kParamHostSyncId = 102,
		kParamNoteLengthId = 103
	};
}