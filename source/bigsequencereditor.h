#pragma once

#include "vstgui4/vstgui/plugin-bindings/vst3editor.h"

namespace vargason::bigsequencer {
	class BigSequencerEditor : public VSTGUI::VST3Editor {
	public:
		BigSequencerEditor(Steinberg::Vst::EditController* controller, VSTGUI::UTF8StringPtr templateName, VSTGUI::UTF8StringPtr xmlFile);
	protected:
		bool open(void* parent, const VSTGUI::PlatformType& platformType) override;
	};
}