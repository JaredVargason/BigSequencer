#pragma once
#include "sequencer.h"
#include "scales.h"
#include <algorithm>
#include <random>

namespace vargason::bigsequencer {

	class NoteDataGenerator {
	public:

		static const Scale defaultScale = Scale::chromatic;
		static const Pitch defaultPitch = Pitch::c;
		static const uint8_t noteLowerBound = 24;
		static const uint8_t noteUpperBound = 95;
		static const uint8_t defaultMinNote = 60;
		static const uint8_t defaultMaxNote = 71;
		static constexpr float defaultFillChance = 0.5f;
		
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
			if (availableNotes.size() == 0) {
				availableNotes.push_back(noteMin);
			}
			return generateNoteData(width, height, availableNotes);
		}
	protected:
		virtual NoteData* generateNoteData(int width, int height, std::vector<int> &availableNotes) = 0;
	};

	class RandomNoteDataGenerator : public NoteDataGenerator {
	public:

		float fillChance;
		int seed;
		bool useRandomSeed;

		RandomNoteDataGenerator(float fillChance = .5f) {
			seed = 0;
			useRandomSeed = true;
			this->fillChance = fillChance;
		}

		NoteData* generateNoteData(int width, int height, std::vector<int> &availableNotes) {
			std::mt19937 rnd(seed);

			NoteData* noteDatas = new NoteData[width * height];
			int numAvailableNotes = (int)availableNotes.size();
			std::uniform_real_distribution<> uniform_real(0.0, 1.0);
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					NoteData* noteData = &noteDatas[y * width + x];
					noteData->active = uniform_real(rnd) <= fillChance;
					noteData->pitch = availableNotes[uniform_real(rnd) * numAvailableNotes];
				}
			}
			return noteDatas;
		}
	};
}
