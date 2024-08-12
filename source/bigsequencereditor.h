#pragma once

#include "vstgui4/vstgui/plugin-bindings/vst3editor.h"
#include "sequencer.h"
#include "sequencerview.h"

namespace vargason::bigsequencer {
	class BigSequencerEditor : public VSTGUI::VST3Editor {
	public:
		BigSequencerEditor(Steinberg::Vst::EditController* controller, VSTGUI::UTF8StringPtr templateName, VSTGUI::UTF8StringPtr xmlFile, Sequencer& sequencer);
		void setSequencerViewInvalid() {
			sequencerView->invalidate();
		}
	protected:
		bool open(void* parent, const VSTGUI::PlatformType& platformType) SMTG_OVERRIDE;
	private:
		
		Sequencer* sequencer = nullptr;
		SequencerView* sequencerView = nullptr;
	};
}