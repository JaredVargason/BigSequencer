#pragma once
#include <stdint.h>
#include "scales.h"

namespace vargason::bigsequencer {

	struct Cursor {
		bool active = false;

		double position;
		bool notePlaying;
		uint8_t currentlyPlayingNote;

		Interval interval;

		float noteLength = 0.4;  // between 0 and 1
		int8_t octaveOffset = 0;

		uint8_t startPosition;  // what index the cursor starts at
		double offset;  // between 0 and 1, offsets from the main beat
		double lastNoteTime;

		const int octaveMin = -2;
		const int octaveMax = 2;

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
			return offset * numericInterval();
		}

		double numericInterval() {
			return numericIntervals[(int)interval];
		}
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

		Cursor& getCursor(int index);

		uint16_t getWidth();
		uint16_t getHeight();
		void setSize(int width, int height);

		NoteData getNote(int index);
		NoteData getNote(int x, int y);
		void setNotes(int width, int height, NoteData* notes);
		int totalNotes();

		uint8_t currentlyPlayingNote;

		const int maxWidth = 32;
		const int maxHeight = 32;
		const int maxNumCursors = 2;

	private:
		Cursor* cursors = new Cursor[maxNumCursors];

		uint16_t width;
		uint16_t height;
	};
}