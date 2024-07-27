#pragma once

#include "pluginterfaces/vst/vsttypes.h"

namespace vargason::bigsequencer {
	enum SequencerParams : Steinberg::Vst::ParamID {
		kParamSequencerWidthId = 100,
		kParamSequencerHeightId,
		kParamHostSyncId,
		kParamRetriggerId,

		kParamCursor1ActiveId,
		kParamCursor1NoteLengthId,
		kParamCursor1NoteIntervalId,
		kParamCursor1PitchOffsetId,

		kParamCursor2ActiveId,
		kParamCursor2NoteLengthId,
		kParamCursor2NoteIntervalId,
		kParamCursor2PitchOffsetId,

		kParamCursor3ActiveId,
		kParamCursor3NoteLengthId,
		kParamCursor3NoteIntervalId,
		kParamCursor3PitchOffsetId,

		kParamCursor4ActiveId,
		kParamCursor4NoteLengthId,
		kParamCursor4NoteIntervalId,
		kParamCursor4PitchOffsetId,

		// "fake" parameters
		kParamScaleId,
		kParamRootNoteId,
		kParamMinNoteId,
		kParamMaxNoteId,
		kParamFillChanceId
	};
}
