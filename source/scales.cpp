#include "scales.h"

namespace vargason::bigsequencer {
	const std::vector<int> chromaticIntegers = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	const std::vector<int> majorIntegers = { 0, 2, 4, 5, 7, 9, 11 };
	const std::vector<int> majorPentatonicIntegers = { 0, 2, 4, 7, 9 };
	const std::vector<int> harmonicMinorIntegers = { 0, 2, 3, 5, 7, 8, 11 };
	const std::vector<int> melodicMinorIntegers = { 0, 2, 3, 5, 7, 9, 11 };  // acsending
	const std::vector<int> dorianIntegers = { 0, 2, 3, 5, 7, 9, 10 };
	const std::vector<int> phyrgianIntegers = { 0, 1, 3, 5, 7, 8, 10 };
	const std::vector<int> lydianIntegers = { 0, 2, 4, 6, 7, 9, 11 };
	const std::vector<int> mixoLydianIntegers = { 0, 2, 4, 5, 7, 9, 10 };
	const std::vector<int> aeolianIntegers = { 0, 2, 3, 5, 7, 8, 10 };  // natural minor scale
	const std::vector<int> locrianIntegers = { 0, 1, 3, 5, 6, 8, 10 };
	
	const std::vector<std::vector<int>> scaleIntegers = {
		chromaticIntegers,
		majorIntegers,
		majorPentatonicIntegers,
		harmonicMinorIntegers,
		melodicMinorIntegers,
		dorianIntegers,
		phrygianIntegers,
		lydianIntegers,
		mixoLydianIntegers,
		aeolianIntegers,
		locrianIntegers
	};

	
	const std::vector<int> getScaleIntegers(Scale scale) {
		return scaleIntegers[(int)scale];
	}
}
