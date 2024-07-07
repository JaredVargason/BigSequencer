#include "sequencer.h"

namespace vargason::bigsequencer {
	Sequencer::Sequencer() {
		this->cursor = 0;
		this->SetSize(16, 4);
	}

	Sequencer::Sequencer(int width, int height) {
		this->cursor = 0;
		SetSize(width, height);
	}

	Sequencer::~Sequencer() {
		delete notes;
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
		if (notes != nullptr) {
			delete this->notes;
		}
		this->notes = notes;
	}

	void Sequencer::SetSize(int width, int height) {
		this->width = width;
		this->height = height;
		if (notes != nullptr) {
			delete notes;
		}
		notes = new NoteData[width, height];
	}

	double Sequencer::getCursor() {
		return cursor;
	}

	void Sequencer::setCursor(double lastTriggeredQuarterNote) {
		cursor = lastTriggeredQuarterNote;
	}

	uint16_t Sequencer::getWidth() {
		return width;
	}

	uint16_t Sequencer::getHeight() {
		return height;
	}
}