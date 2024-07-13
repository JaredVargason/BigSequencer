#pragma once
#include <stdint.h>

namespace vargason::bigsequencer {

	struct Cursor {
		double position;
		bool notePlaying;
		uint8_t currentlyPlayingNote;

		float interval;  // thirty-second notes, sixteenth notes, eighth notes, quarter notes, half notes, whole notes, double whole notes, quad whole notes
		float noteLength;  // between 0 and 1
		uint8_t startPosition;  // what index the cursor starts at
		float offset;  // between 0 and 1, offsets from the main beat
		uint8_t octave;
	};

	struct NoteData {
		float velocity = 0.40;
		uint8_t pitch = 60;
		uint8_t probability; // probability that individual note will hit
		bool active;
	};

	class Sequencer {
	public:
		NoteData* notes = nullptr;

		Sequencer();
		Sequencer(int width, int height);
		~Sequencer();

		double getCursor();
		void setCursor(double cursor);
		bool isNotePlaying();
		void setNotePlaying(bool state);

		uint16_t getWidth();
		uint16_t getHeight();
		void setSize(int width, int height);

		NoteData getNote(int index);
		NoteData getNote(int x, int y);
		NoteData getCurrentNote();
		void setNotes(int width, int height, NoteData* notes);
		int totalNotes();

		void setNoteLength(float noteLength);
		float getNoteLength();

	private:
		double cursor;
		bool notePlaying;
		uint8_t currentlyPlayingNote;

		uint16_t width;
		uint16_t height;
		float noteLength;

		// scale
		uint8_t rootNote;
	};
}