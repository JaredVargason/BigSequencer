#pragma once

#include  "vstgui4/vstgui/lib/copenglview.h"

#include "sequencer.h"
#include <Windows.h>
#include <gl/GL.h>
#include <math.h>

namespace vargason::bigsequencer {

	struct Color {
		int r;
		int g;
		int b;
	};

	struct HSLColor {
		float h;
		float s;
		float l;
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
		const Color inactiveNoteColor = { 200, 200, 200 };
		const Color noteColors[12] = {
			{255, 0, 0},  // red
			{255, 72, 0},  // red-orange
			{255, 128, 0},  // orange
			{255, 183, 0},  // yellow-orange
			{255, 255, 0},  // yellow
			{183, 255, 0},  // yellow-green
			{0, 255, 0},  // green
			{0, 183, 255},  // cyan
			{0, 100, 255},  // blue green
			{0, 0, 255},  // blue
			{115, 0, 255},  // blue-violet
			{183, 0, 255},  // violet
		};

		Color allColors[71];

		const Color cursorColors[4] = {
			{150, 150, 150},
			{100, 100, 100},
			{50, 50, 50},
			{0, 0, 0}
		};

		Color hslToRgb(HSLColor color) {
			float r, g, b;
			if (color.s == 0) {
				r = g = b = color.l;
			}
			else {
				const float q = color.l < 0.5f ? color.l * (1 + color.s) : color.l + color.s - color.l * color.s;
				const float p = 2 * color.l - q;
				r = hueToRgb(p, q, color.h + 1.0f / 3.0f);
				g = hueToRgb(p, q, color.h);
				b = hueToRgb(p, q, color.h - 1.0f / 3.0f);
			}

			Color rgbColor;
			rgbColor.r = (int)round(r * 255);
			rgbColor.g = (int)round(g * 255);
			rgbColor.b = (int)round(b * 255);
			return rgbColor;
		}

		float hueToRgb(float p, float q, float t) {
			if (t < 0) t += 1;
			if (t > 1) t -= 1;
			if (t < 1.0f / 6.0f) return p + (q - p) * 6 * t;
			if (t < 0.5f) return q;
			if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6;
			return p;
		}

		HSLColor rgbToHsl(Color color) {
			float r = (float)color.r / 255;
			float g = (float)color.g / 255;
			float b = (float)color.b / 255;

			const float vmax = max(max(r, g), b);
			const float vmin = min(min(r, g), b);
			float h, s, l;
			h = s = l = (vmax + vmin) / 2;

			HSLColor hslColor;
			if (vmax == vmin) {
				hslColor.h = 0;
				hslColor.s = 0;
				hslColor.l = l;
				return hslColor;
			}

			const float d = vmax - vmin;
			s = l > 0.5 ? d / (2 - vmax - vmin) : d / (vmax + vmin);

			if (vmax == r) h = (g - b) / d + (g < b ? 6 : 0);
			if (vmax == g) h = (b - r) / d + 2;
			if (vmax == b) h = (r - g) / d + 4;
			h /= 6;

			hslColor.h = h;
			hslColor.s = s;
			hslColor.l = l;
			return hslColor;
		}
	};
}
