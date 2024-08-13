#include "bigsequencereditor.h"


namespace vargason::bigsequencer {
	BigSequencerEditor::BigSequencerEditor(Steinberg::Vst::EditController* controller, VSTGUI::UTF8StringPtr templateName, VSTGUI::UTF8StringPtr xmlFile, Sequencer& sequencer) : VSTGUI::VST3Editor(controller, templateName, xmlFile) {
		//this->viewName = xmlFile;
		this->sequencer = &sequencer;
	}

	bool BigSequencerEditor::open(void* parent, const VSTGUI::PlatformType& platformType) {
		if (!VSTGUI::VST3Editor::open(parent, platformType)) {
			return false;
		}

		const VSTGUI::CRect viewSize(695, 100, 835, 240);
		sequencerView = new SequencerView(viewSize, *sequencer);
		frame->addView(sequencerView);

		return true;
	}
}
