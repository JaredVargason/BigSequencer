#include "sequencer.h"

namespace vargason::bigsequencer {
	Sequencer::Sequencer() : Sequencer(16, 4) {
	}

	Sequencer::Sequencer(int width, int height) {
		setSize(width, height);
		cursors = new Cursor[maxNumCursors];
		cursors[0].active = true;
		cursors[0].interval = quarterNote;
	}

	Sequencer::~Sequencer() {
		delete notes;
		delete cursors;
	}

	NoteData Sequencer::getNote(int index) {
		return notes[index];
	}

	NoteData Sequencer::getNote(int x, int y) {
		return notes[y * width + x];
	}

	int Sequencer::totalNotes() {
		return width * height;
	}

	// right now we are just setting the notes explicity, but we could validate and copy.
	void Sequencer::setNotes(int width, int height, NoteData* notes) {
		this->width = width;
		this->height = height;
		// we should validate probably maybe
		if (notes != NULL) {
			delete this->notes;
		}
		this->notes = notes;
	}

	void Sequencer::setSize(int width, int height) {
		this->width = width;
		this->height = height;
		if (notes != nullptr) {
			delete notes;
		}

		notes = new NoteData[width * height];
	}

	Cursor& Sequencer::getCursor(int index) {
		return this->cursors[index];
	}

	uint16_t Sequencer::getWidth() {
		return width;
	}

	uint16_t Sequencer::getHeight() {
		return height;
	}
}