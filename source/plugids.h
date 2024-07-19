#pragma once

#include "pluginterfaces/vst/vsttypes.h"

namespace vargason::bigsequencer {
	enum SequencerParams : Steinberg::Vst::ParamID {
		kParamSequencerWidthId = 100,
		kParamSequencerHeightId,
		kParamHostSyncId,

		kParamCursor1ActiveId,
		kParamCursor1NoteLengthId,
		kParamCursor1NoteIntervalId,
		kParamCursor1PitchOffsetId,

		kParamCursor2ActiveId,
		kParamCursor2NoteLengthId,
		kParamCursor2NoteIntervalId,
		kParamCursor2PitchOffsetId
	};
}