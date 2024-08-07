#include "sequencerview.h"

namespace vargason::bigsequencer {
	SequencerView::SequencerView(const VSTGUI::CRect& size) : VSTGUI::COpenGLView(size) {}

	void SequencerView::drawOpenGL(const VSTGUI::CRect& updateRect) {
		// draw our sequencer
		if (sequencer == nullptr) {
			return;
		}
	}

	void SequencerView::setSequencer(Sequencer* sequencer) {
		this->sequencer = sequencer;
	}
}
