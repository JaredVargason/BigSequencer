#pragma once
#include "sequencer.h"
#include "scales.h"
#include <random>

namespace vargason::bigsequencer {

	class NoteDataGenerator {
	public:
		virtual NoteData* generateNoteData(Note rootNote, Scale scale, uint16_t noteMin, uint16_t noteMax, int width, int height) = 0;
	};

	class RandomNoteDataGenerator : public NoteDataGenerator {
	public:
		NoteData* generateNoteData(Note rootNote, Scale scale, uint16_t noteMin, uint16_t noteMax, int width, int height) {
			NoteData* noteDatas = new NoteData[width * height];
			std::random_device rd;
			std::mt19937 rnd(rd());
			std::uniform_int_distribution<> uniform_int(noteMin, noteMax);
			std::uniform_real_distribution<> uniform_real(0.0, 1.0);
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					NoteData* noteData = &noteDatas[y * width + x];
					noteData->pitch = uniform_int(rnd);
					noteData->velocity = uniform_real(rnd);
					noteData->probability = 100;
				}
			}
			return noteDatas;
		}
	};
}