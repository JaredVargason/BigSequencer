#include "scales.h"

namespace vargason::bigsequencer {
	const std::vector<int> chromaticIntegers = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	const std::vector<int> majorIntegers = { 0, 2, 4, 5, 7, 9, 11 };
	const std::vector<int> majorPentatonicIntegers = { 0, 2, 4, 7, 9 };
	const std::vector<int> harmonicMinorIntegers = { 0, 2, 3, 5, 7, 8, 11 };
	const std::vector<int> melodicMinorIntegers = { 0, 2, 3, 5, 7, 9, 11 };  // acsending
	const std::vector<int> dorianIntegers = { 0, 2, 3, 5, 7, 9, 10 };
	const std::vector<int> phrygianIntegers = { 0, 1, 3, 5, 7, 8, 10 };
	const std::vector<int> lydianIntegers = { 0, 2, 4, 6, 7, 9, 11 };
	const std::vector<int> mixolydianIntegers = { 0, 2, 4, 5, 7, 9, 10 };
	const std::vector<int> minorIntegers = { 0, 2, 3, 5, 7, 8, 10 };
	const std::vector<int> locrianIntegers = { 0, 1, 3, 5, 6, 8, 10 };
	const std::vector<int> bluesIntegers = { 0, 3, 5, 6, 7, 10 };
	const std::vector<int> augmentedIntegers = { 0, 3, 4, 7, 8, 11 };
	const std::vector<int> prometheusIntegers = { 0, 2, 4, 6, 9, 10 };
	const std::vector<int> hirajoshiIntegers = { 0, 4, 6, 7, 11 };
	const std::vector<int> enigmaticIntegers = { 0, 1, 4, 6, 8, 10, 11 };
	const std::vector<int> gypsyIntegers = { 0, 2, 3, 6, 7, 9, 10 };
	const std::vector<int> superLocrianIntegers = { 0, 1, 3, 4, 6, 8, 10 };
	
	const std::vector<std::vector<int>> scaleIntegers = {
		chromaticIntegers,
		majorIntegers,
		majorPentatonicIntegers,
		harmonicMinorIntegers,
		melodicMinorIntegers,
		dorianIntegers,
		phrygianIntegers,
		lydianIntegers,
		mixolydianIntegers,
		minorIntegers,
		locrianIntegers,
		bluesIntegers,
		augmentedIntegers,
		prometheusIntegers,
		hirajoshiIntegers,
		enigmaticIntegers,
		gypsyIntegers,
		superLocrianIntegers
	};
	
	const std::vector<int> getScaleIntegers(Scale scale) {
		return scaleIntegers[(int)scale];
	}

	// get the interval in a normalized form so we can pass it into the controller
	const float normalizedInterval(Interval interval) {
		return (interval) / (float)(totalIntervals - 1);
	}
}
