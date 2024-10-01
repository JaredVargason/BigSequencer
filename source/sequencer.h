#pragma once
#include <stdint.h>
#include "scales.h"

namespace vargason::bigsequencer {

	struct Cursor {
		bool active = false;
		Interval interval = Interval::quarterNote;
		int8_t pitchOffset = 0;
		float velocity = 0.6f;
		float probability = 1.0f;

		int position = 0;
		bool notePlaying = false;
		uint8_t currentlyPlayingNote = 60;
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

		void setNoteLength(float noteLength) {  // we do this because note playing bugs when length is 0 or 1
			if (noteLength >= 1) {
				noteLength = .998f;
			}
			else if(noteLength <= 0) {
				noteLength = 0.002f;
			}
			this->noteLength = noteLength;
		};

		float getNoteLength() {
			return this->noteLength;
		}

		double realNoteLength() {
			return noteLength * numericInterval();
		}

		double numericInterval() {
			return numericIntervals[(int)interval];
		}

	private:
		float noteLength = 0.4;  // between 0 and 1
	};

	struct NoteData {
		uint8_t pitch = 60;
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

		NoteData& getNote(int index);
		NoteData& getNote(int x, int y);
		void setNotes(int width, int height, NoteData* notes);
		int totalNotes();

		static const int minWidth = 1;
		static const int minHeight = 1;

		static const int maxWidth = 32;
		static const int maxHeight = 32;

		static const int maxNumCursors = 4;

		static const int defaultWidth = 16;
		static const int defaultHeight = 4;

	private:
		Cursor* cursors = nullptr;  // these don't need to be pointers either if we have a constant size

		uint16_t width = defaultWidth;
		uint16_t height = defaultHeight;
	};
}
