#include "sequencerview.h"

namespace vargason::bigsequencer {
	SequencerView::SequencerView(const VSTGUI::CRect& size, Sequencer& sequencer) : VSTGUI::COpenGLView(size) {
		this->sequencer = &sequencer;
	}

	void SequencerView::drawOpenGL(const VSTGUI::CRect& updateRect) {
		// draw our sequencer
		if (sequencer == nullptr) {
			return;
		}
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


		/*glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f(viewSize.getWidth() / 2, viewSize.getHeight() - 50);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex2f(50, 50);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex2f(viewSize.getWidth() - 50, 50);
		glEnd();*/

		float sqrWidth = viewSize.getWidth() / sequencer->getWidth();
		glBegin(GL_TRIANGLES);
		for (int y = 0; y < sequencer->getHeight(); y++) {
			for (int x = 0; x < sequencer->getWidth(); x++) {

				// get color for note
				// if its active, we draw it, otherwise we dont

				float xPos = x * sqrWidth;
				float yPos = viewSize.getHeight() - y * sqrWidth;
				NoteData& noteData = sequencer->getNote(x, y);
				const Color color = noteData.active ? colors[noteData.pitch % 12] : inactiveColor;

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
		/*
		// draw cursors on top
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < sequencer->maxNumCursors; i++) {
			float xPos = 0;
			float yPos = 0;
			// draw inside the square

		}
		glEnd();
		*/
		

		glFlush();
		this->getPlatformOpenGLView()->swapBuffers();
	}
}
