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
		const Color inactiveNoteColor = { 128, 128, 128 };
		const Color noteColors[12] = {
			{255, 0, 0},    // red
			{255, 128, 0},  // orange
			{255, 255, 0},  // yellow
			{128, 255, 0},  // light green
			{0, 255, 0},  // green
			{0, 255, 128},  // teal
			{0, 255, 255},  // light blue
			{0, 128, 255},  // sky blue
			{0, 0, 255},  // dark blue
			{127, 0, 255},  // purple
			{255, 0, 255},  // pink
			{102, 0, 51}  // maroon or something
		};

		const Color cursorColors[4] = {
			{102, 0, 0},
			{0, 102, 102},
			{102, 102, 0},
			{0, 102, 0}
		};
	};
}
