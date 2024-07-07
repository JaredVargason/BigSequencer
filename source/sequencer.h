#pragma once
#include <stdint.h>

namespace vargason::bigsequencer {

	struct NoteData {
		uint8_t velocity;
		uint8_t pitch;
		uint8_t probability; // probability that individual note will hit
	};

	class Sequencer {
	public:
		NoteData* notes;

		Sequencer();
		Sequencer(int width, int height);
		~Sequencer();

		double getCursor();
		void setCursor(double lastTriggeredQuarterNote);
		uint16_t getWidth();
		uint16_t getHeight();
		void SetSize(int width, int height);

		NoteData getNote(int index);
		NoteData getNote(int x, int y);
		void setNotes(int width, int height, NoteData* notes);
		int totalNotes();
	private:
		double cursor;
		uint16_t width;
		uint16_t height;
	};
}