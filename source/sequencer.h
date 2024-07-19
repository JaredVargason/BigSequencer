#pragma once
#include <stdint.h>
#include "scales.h"

namespace vargason::bigsequencer {

	struct Cursor {
		bool active = false;

		bool notePlaying = false;
		uint8_t currentlyPlayingNote = 60;
		int position = 0;

		Interval interval = Interval::quarterNote;
		float noteLength = 0.4;  // between 0 and 1
		int8_t pitchOffset = 0;

		uint8_t startPosition = 0;  // what index the cursor starts at
		double offset = 0;  // between 0 and 1, offsets from the main beat, maybe allow -1 to 1
		double lastNoteTime = 0;

		const int pitchMin = -24;
		const int pitchMax = 24;

		const double* numericIntervals = new double[7] {
			0.125,
			0.25,
			0.5,
			1.0,
			2.0,
			4.0,
			8.0
		};

		double realNoteLength() {
			return noteLength * numericInterval();
		}

		double numericInterval() {
			return numericIntervals[(int)interval];
		}
	};

	struct NoteData {
		float velocity = 0.40;
		uint8_t pitch = 60;
		uint8_t probability = 100; // probability that individual note will hit
		bool active = true;
	};

	class Sequencer {
	public:
		NoteData* notes = nullptr;

		Sequencer();
		Sequencer(int width, int height);
		~Sequencer();

		Cursor& getCursor(int index);

		uint16_t getWidth();
		uint16_t getHeight();
		void setSize(int width, int height);

		NoteData getNote(int index);
		NoteData getNote(int x, int y);
		void setNotes(int width, int height, NoteData* notes);
		int totalNotes();

		const int maxWidth = 32;
		const int maxHeight = 32;
		const int maxNumCursors = 1;

	private:
		Cursor* cursors = nullptr;

		uint16_t width;
		uint16_t height;
	};
}