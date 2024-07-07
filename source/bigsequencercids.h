//------------------------------------------------------------------------
// Copyright(c) 2024 Vargason.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace vargason::bigsequencer {
//------------------------------------------------------------------------
static const Steinberg::FUID kBigSequencerProcessorUID (0x28B960F2, 0xB70D5AE2, 0xBD663D5E, 0xEBAE6B26);
static const Steinberg::FUID kBigSequencerControllerUID (0x8547FB53, 0x33825F79, 0xBED7E081, 0xDC803592);

#define BigSequencerVST3Category "Instrument"

//------------------------------------------------------------------------
} // namespace vargason::bigsequencer
