#pragma once
#include <vector>

namespace vargason::bigsequencer {
	enum Note {
		a,
		aSharp,
		b,
		c,
		cSharp,
		d,
		dSharp,
		e,
		f,
		fSharp,
		g,
		gSharp
	};

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
		aeolian,
		locrian
	};

	enum Interval {
		thirtySecondNote = 0,
		sixteenthNote,
		eighthNote,
		quarterNote,
		halfNote,
		wholeNote,
		doubleWholeNote
	};

	const std::vector<int> getScaleIntegers(Scale scale);
}
