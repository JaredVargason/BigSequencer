#pragma once
#include "sequencer.h"
#include "scales.h"
#include <algorithm>
#include <random>
#include <math.h>

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
			if (availableNotes.size() == 0) {
				availableNotes.push_back(noteMin);
			}
			return generateNoteData(width, height, availableNotes);
		}
	protected:
		virtual NoteData* generateNoteData(int width, int height, std::vector<int> &availableNotes) = 0;
	};

	enum NoteDataGeneratorType {
		valueNoise = 0,
		perlinNoise = 1
	};

	class ValueNoiseNoteDataGenerator : public NoteDataGenerator {
	public:

		float fillChance;
		std::mt19937 rnd;

		ValueNoiseNoteDataGenerator(float fillChance = .5f): rnd(std::random_device()()) {
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
				}
			}
			return noteDatas;
		}
	};

	class PerlinNoiseNoteDataGenerator : public NoteDataGenerator {
	public:
		float fillChance;
		std::mt19937 rnd;
		float scale;
		float xOffset;
		float yOffset;

		PerlinNoiseNoteDataGenerator(float fillChance = .5f): rnd(std::random_device()()) {
			this->fillChance = fillChance;
			this->scale = 1;
			this->xOffset = 0;
			this->yOffset = 0;
		}

		NoteData* generateNoteData(int width, int height, std::vector<int>& availableNotes) {
			NoteData* noteDatas = new NoteData[width * height];
			int numAvailableNotes = availableNotes.size();
			std::uniform_real_distribution<> uniform_real(0.0, 1.0);
			for (int y = 0; y < height; y++) {
				for (int x = 0; x < width; x++) {
					NoteData* noteData = &noteDatas[y * width + x];
					noteData->active = uniform_real(rnd) <= fillChance;
					float val = perlin(x, y);
					if (val == 1) {
						val = .995;
					}
					noteData->pitch = availableNotes[val * numAvailableNotes];
				}
			}
			return noteDatas;
		}

		float interpolate(float a, float b, float t) {
			return a + (b - a) * t;
		}

		typedef struct {
			float x, y;
		} vector2;

		vector2 randomGradient(int ix, int iy) {
			const unsigned w = 8 * sizeof(unsigned);
			const unsigned s = w / 2;
			unsigned a = ix, b = iy;
			a *= 3284157443; b ^= a << s | a >> w - s;
			b *= 1911520717; a ^= b << s | b >> w - s;
			a *= 2048419325;
			float random = a * (3.14159265 / ~(~0u >> 1));
			vector2 v;
			v.x = cos(random); v.y = sin(random);
			return v;
		}

		float dotGridGradient(int ix, int iy, float x, float y) {
			vector2 gradient = randomGradient(ix, iy);
			float dx = x - float(ix);
			float dy = y - float(iy);
			return dx * gradient.x + dy * gradient.y;
		}

		float perlin(float x, float y) {
			x += 0.5f;
			y += 0.5f;
			int x0 = (int)floor(x);
			int x1 = x0 + 1;
			int y0 = (int)floor(y);
			int y1 = y0 + 1;

			float sx = x - (float)x0;
			float sy = y - (float)y0;

			float n0, n1, ix0, ix1, value;

			n0 = dotGridGradient(x0, y0, x, y);
			n1 = dotGridGradient(x1, y0, x, y);
			ix0 = interpolate(n0, n1, sx);

			n0 = dotGridGradient(x0, y1, x, y);
			n1 = dotGridGradient(x1, y1, x, y);
			ix1 = interpolate(n0, n1, sx);

			value = interpolate(ix0, ix1, sy);
			return value * 0.5 + 0.5;
		}
	};
}
