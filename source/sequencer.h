#pragma once
#include <stdint.h>

namespace vargason::bigsequencer {

	struct Cursor {
		bool active;

		double position;
		bool notePlaying;
		uint8_t currentlyPlayingNote;

		float interval;  // thirty-second notes, sixteenth notes, eighth notes, quarter notes, half notes, whole notes, double whole notes, quad whole notes
		float noteLength = 0.4;  // between 0 and 1
		int8_t octaveOffset = 0; // by default go to middle c

		uint8_t startPosition;  // what index the cursor starts at
		float offset;  // between 0 and 1, offsets from the main beat
		double lastNoteTime;
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