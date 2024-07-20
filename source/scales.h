#pragma once

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
		minor,
		dorian,
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
}