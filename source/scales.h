#pragma once
#include <vector>

namespace vargason::bigsequencer {
	enum Pitch {
		c,
		cSharp,
		d,
		dSharp,
		e,
		f,
		fSharp,
		g,
		gSharp,
		a,
		aSharp,
		b
	};
	const int totalPitches = 12;

	enum Scale {
		chromatic,
		major,
		majorPentatonic,
		harmonicMinor,
		melodicMinor,
		dorian,
		phrygian,
		lydian,
		mixolydian,
		minor,
		locrian,
		blues,
		augmented,
		prometheseus,
		hirajoshi,
		enigmatic,
		gypsy,
		superLocrian
	};
	const int totalScales = 18;

	enum Interval {
		thirtySecondNote = 0,
		sixteenthNote,
		eighthNote,
		quarterNote,
		halfNote,
		wholeNote,
		doubleWholeNote
	};
	const int totalIntervals = 7;

	const std::vector<int> getScaleIntegers(Scale scale);
	const float normalizedInterval(Interval interval);
}
