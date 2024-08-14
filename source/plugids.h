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
		kParamCursor1VelocityId,

		kParamCursor2ActiveId,
		kParamCursor2NoteLengthId,
		kParamCursor2NoteIntervalId,
		kParamCursor2PitchOffsetId,
		kParamCursor2VelocityId,

		kParamCursor3ActiveId,
		kParamCursor3NoteLengthId,
		kParamCursor3NoteIntervalId,
		kParamCursor3PitchOffsetId,
		kParamCursor3VelocityId,

		kParamCursor4ActiveId,
		kParamCursor4NoteLengthId,
		kParamCursor4NoteIntervalId,
		kParamCursor4PitchOffsetId,
		kParamCursor4VelocityId,

		// "fake" parameters
		kParamScaleId,
		kParamRootNoteId,
		kParamMinNoteId,
		kParamMaxNoteId,
		kParamFillChanceId,

		kParamCursorTab
	};
}
