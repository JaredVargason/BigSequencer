#include "bigsequencereditor.h"
#include "sequencerview.h"

namespace vargason::bigsequencer {
	BigSequencerEditor::BigSequencerEditor(Steinberg::Vst::EditController* controller, VSTGUI::UTF8StringPtr templateName, VSTGUI::UTF8StringPtr xmlFile) : VSTGUI::VST3Editor(controller, templateName, xmlFile) {}

	bool BigSequencerEditor::open(void* parent, const VSTGUI::PlatformType& platformType) {
		if (!VSTGUI::VST3Editor::open(parent, platformType)) {
			return false;
		}

		VSTGUI::CRect sequencerSize(10, 100, 110, 140);
		SequencerView* sequencerView = new SequencerView(sequencerSize);
		sequencerView->setSequencer(nullptr);

		return true;
	}
}
