#include "sequencerview.h"

namespace vargason::bigsequencer {
	SequencerView::SequencerView(const VSTGUI::CRect& size, Sequencer& sequencer) : VSTGUI::COpenGLView(size) {
		this->sequencer = &sequencer;
		const float lightnessModifier = 0.2f;
		const float baseNoteLightnessModifier = 0.10f;
		for (int i = 0; i < 71; i++) {
			Color baseNoteColor = noteColors[i % 12];
			HSLColor color = rgbToHsl(baseNoteColor);
			float adjustedLightnessModifier = baseNoteLightnessModifier + (i / 12) * lightnessModifier;
			color.l = adjustedLightnessModifier;
			allColors[i] = hslToRgb(color);
		}
	}

	void SequencerView::drawOpenGL(const VSTGUI::CRect& updateRect) {
		// draw our sequencer
		VSTGUI::CRect viewSize = getViewSize();
		glViewport(0, 0, viewSize.getWidth(), viewSize.getHeight());

		glDisable(GL_DEPTH_TEST);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, viewSize.getWidth(), 0, viewSize.getHeight(), -1, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		float sqrWidth = viewSize.getWidth() / sequencer->getWidth();
		glBegin(GL_TRIANGLES);
		for (int y = 0; y < sequencer->getHeight(); y++) {
			for (int x = 0; x < sequencer->getWidth(); x++) {

				// get color for note
				// if its active, we draw it, otherwise we dont

				float xPos = x * sqrWidth;
				float yPos = viewSize.getHeight() - y * sqrWidth;
				NoteData& noteData = sequencer->getNote(x, y);
				const Color color = noteData.active ? allColors[noteData.pitch - 24] : inactiveNoteColor;

				glColor3ub(color.r, color.g, color.b);
				glVertex2f(xPos, yPos);  // top left
				glVertex2f(xPos + sqrWidth, yPos);  // top right
				glVertex2f(xPos + sqrWidth, yPos - sqrWidth);  // bottom right

				glVertex2f(xPos, yPos);  // top left
				glVertex2f(xPos + sqrWidth, yPos - sqrWidth);  // bottom right
				glVertex2f(xPos, yPos - sqrWidth); // bottom left
			}
		}
		glEnd();

		// draw cursors on top
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < sequencer->maxNumCursors; i++) {

			// draw inside the square
			Cursor& cursor = sequencer->getCursor(i);
			if (!cursor.active) {
				continue;
			}
			float xIndex = (cursor.position % sequencer->getWidth());
			float yIndex = cursor.position / sequencer->getWidth();

			float xPos = xIndex * sqrWidth;
			float yPos = viewSize.getHeight() - yIndex * sqrWidth;

			const Color color = cursorColors[i];
			glColor3ub(color.r, color.g, color.b);
			glVertex2f(xPos, yPos);  // top left
			glVertex2f(xPos + sqrWidth, yPos);  // top right
			glVertex2f(xPos + sqrWidth, yPos - sqrWidth);  // bottom right

			glVertex2f(xPos, yPos);  // top left
			glVertex2f(xPos + sqrWidth, yPos - sqrWidth);  // bottom right
			glVertex2f(xPos, yPos - sqrWidth); // bottom left
		}
		glEnd();

		glFlush();
		this->getPlatformOpenGLView()->swapBuffers();
	}
}
