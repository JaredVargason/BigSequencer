#pragma once

#include  "vstgui4/vstgui/lib/copenglview.h"

#include "sequencer.h"
#include <Windows.h>
#include <gl/GL.h>

namespace vargason::bigsequencer {

	struct Color {
		int r;
		int g;
		int b;
	};

	class SequencerView : public VSTGUI::COpenGLView {
	public:
		SequencerView(const VSTGUI::CRect& size, Sequencer& sequencer);
		~SequencerView() override = default;

		bool shouldUpdate = false;
		void drawOpenGL(const VSTGUI::CRect& updateRect) override;
		void invalidate() {
			invalid();
		}
	private:
		Sequencer* sequencer;
		const Color inactiveColor = { 128, 128, 128 };
		const Color colors[12] = {
			{255, 0, 0},    // red
			{255, 128, 0},  // orange
			{255, 255, 0},  // yellow
			{128, 255, 0},  // light green
			{0, 255, 0}, // green
			{0, 255, 128},
			{0, 255, 255},
			{0, 128, 255},
			{0, 0, 255},
			{127, 0, 255},
			{255, 0, 255},
			{102, 0, 51}
		};
	};
}