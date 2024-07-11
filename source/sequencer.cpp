#include "sequencer.h"

namespace vargason::bigsequencer {
	Sequencer::Sequencer() : Sequencer(16, 4) {
	}

	Sequencer::Sequencer(int width, int height) {
		this->cursor = 0;
		this->currentlyPlayingNote = 60;
		this->notePlaying = false;
		this->noteLength = 0.5f;
		setSize(width, height);
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

	void Sequencer::setNoteLength(float noteLength) {
		this->noteLength = noteLength;
	}

	float Sequencer::getNoteLength() {
		return this->noteLength;
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

	double Sequencer::getCursor() {
		return cursor;
	}

	void Sequencer::setCursor(double cursor) {
		this->cursor = cursor;
	}

	bool Sequencer::isNotePlaying()
	{
		return notePlaying;
	}

	void Sequencer::setNotePlaying(bool state)
	{
		notePlaying = state;
	}

	uint16_t Sequencer::getWidth() {
		return width;
	}

	uint16_t Sequencer::getHeight() {
		return height;
	}
}