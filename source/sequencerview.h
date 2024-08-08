#pragma once

#include  "vstgui4/vstgui/lib/copenglview.h"

#include "sequencer.h"

namespace vargason::bigsequencer {
	class SequencerView : public VSTGUI::COpenGLView {
	public:
		SequencerView(const VSTGUI::CRect& size);
		~SequencerView() override = default;

		void drawOpenGL(const VSTGUI::CRect& updateRect) override;

		void setSequencer(Sequencer* sequencer);

	private:
		Sequencer* sequencer = nullptr;
	};
}