#pragma once
#include "sequencer.h"
#include "scales.h"
#include <algorithm>
#include <random>

namespace vargason::bigsequencer {

	class NoteDataGenerator {
	public:
		
		NoteData* generate(int width, int height, Pitch rootNote, Scale scale, uint8_t noteMin, uint8_t noteMax) {
			// get all available notes to pass to the generate function
			std::vector<int> scaleIntegers = getScaleIntegers(scale);
			std::vector<int> availableNotes;
			// iterate through every note min & max- see if it is in the scale
			// a note is in scale if we modulo by 12, add the pitch rootnote, and that value is in the availableNotes
			for (uint8_t currentNote = noteMin; currentNote <= noteMax; currentNote++) {
				uint8_t adjusted = (currentNote + rootNote) % 12;
				if (std::find(scaleIntegers.begin(), scaleIntegers.end(), adjusted) != scaleIntegers.end()) {
					availableNotes.push_back(currentNote);
				}
			}
			return generateNoteData(width, height, availableNotes);
		}
	protected:
		virtual NoteData* generateNoteData(int width, int height, std::vector<int> &availableNotes) = 0;
	};

	class RandomNoteDataGenerator : public NoteDataGenerator {
	public:

		float fillChance;
		std::mt19937 rnd;

		RandomNoteDataGenerator(float fillChance = .5f): rnd((std::random_device())()) {
			this->fillChance = fillChance;
		}

		NoteData* generateNoteData(int width, int height, std::vector<int> &availableNotes) {
			NoteData* noteDatas = new NoteData[width * height];
			int numAvailableNotes = availableNotes.size();
			std::uniform_real_distribution<> uniform_real(0.0, 1.0);
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					NoteData* noteData = &noteDatas[y * width + x];
					noteData->active = uniform_real(rnd) <= fillChance;
					noteData->pitch = availableNotes[uniform_real(rnd) * numAvailableNotes];
					noteData->probability = 100;
				}
			}
			return noteDatas;
		}
	};
}