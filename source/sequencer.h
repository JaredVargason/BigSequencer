#pragma once
#include <stdint.h>

namespace vargason::bigsequencer {

	struct NoteData {
		uint8_t velocity;
		uint8_t pitch;
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
	};
}