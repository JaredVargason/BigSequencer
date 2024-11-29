#pragma once

#include "pluginterfaces/vst/vsttypes.h"

namespace vargason::bigsequencer {
	enum SequencerParams : Steinberg::Vst::ParamID {
		kParamSequencerWidthId = 100,
		kParamSequencerHeightId,
		kParamRetriggerId,

		kParamCursor1ActiveId,
		kParamCursor1NoteLengthId,
		kParamCursor1NoteIntervalId,
		kParamCursor1PitchOffsetId,
		kParamCursor1VelocityId,
		kParamCursor1ProbabilityId,
		kParamCursor1StartPositionId,
		kParamCursor1TransitionRuleId,

		kParamCursor2ActiveId,
		kParamCursor2NoteLengthId,
		kParamCursor2NoteIntervalId,
		kParamCursor2PitchOffsetId,
		kParamCursor2VelocityId,
		kParamCursor2ProbabilityId,
		kParamCursor2StartPositionId,
		kParamCursor2TransitionRuleId,

		kParamCursor3ActiveId,
		kParamCursor3NoteLengthId,
		kParamCursor3NoteIntervalId,
		kParamCursor3PitchOffsetId,
		kParamCursor3VelocityId,
		kParamCursor3ProbabilityId,
		kParamCursor3StartPositionId,
		kParamCursor3TransitionRuleId,

		kParamCursor4ActiveId,
		kParamCursor4NoteLengthId,
		kParamCursor4NoteIntervalId,
		kParamCursor4PitchOffsetId,
		kParamCursor4VelocityId,
		kParamCursor4ProbabilityId,
		kParamCursor4StartPositionId,
		kParamCursor4TransitionRuleId,

		kParamScaleId,
		kParamRootNoteId,
		kParamMinNoteId,
		kParamMaxNoteId,
		kParamFillChanceId,
		kParamSeedId,
		kParamUseRandomSeedId,

		kParamCursorTab
	};
}
