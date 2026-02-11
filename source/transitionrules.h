#pragma once

#include <random>>

namespace vargason::bigsequencer {
	
	enum TransitionRules {
		forward,
		backwards,
		forwardsBackwards,
		random
	};

	class TransitionRule {
	public:
		uint16_t startPosition;

		virtual void reset(uint16_t sequencerLength) {}
		virtual uint16_t getStartPosition(uint16_t sequencerLength) = 0;
		virtual uint16_t getNextPosition(uint16_t position, uint16_t sequencerLength) = 0;
	};

	class ForwardTransitionRule : public TransitionRule {
		uint16_t getStartPosition(uint16_t sequencerLength) {
			return 0;
		}

		uint16_t getNextPosition(uint16_t position, uint16_t sequencerLength) {
			position++;
			if (position > sequencerLength - 1) {
				return 0;
			}
			return position;
		}
	};

	class BackwardTransitionRule : public TransitionRule {
		uint16_t getStartPosition(uint16_t sequencerLength) {
			return sequencerLength - 1;
		}

		uint16_t getNextPosition(uint16_t position, uint16_t sequencerLength) {
			position--;
			if (position < 0) {
				return sequencerLength - 1;
			}
			return position;
		}
	};

	class ForwardsBackwardsTransitionRule : public TransitionRule {
	public:
		ForwardsBackwardsTransitionRule() {
			goingForward = true;
		}

		void reset() {
			goingForward = true;
		}

		uint16_t getStartPosition(uint16_t sequencerLength) {
			return 0;
		}

		uint16_t getNextPosition(uint16_t position, uint16_t sequencerLength) {
			if (goingForward) {
				position++;
				if (position > sequencerLength - 1) {
					goingForward = false;
					position--;
				}
			}
			else {
				position--;
				if (position < 0) {
					position++;
					goingForward = true;
				}
			}
			return position;
		}

	private:
		bool goingForward;
	};

	class RandomTransitionRule : public TransitionRule {
		RandomTransitionRule(std::random_device device) : rnd(device()), dst(0, 10) {
		}

		uint16_t getStartPosition(uint16_t sequencerLength) {
			return dst(rnd, std::uniform_int_distribution<uint16_t>::param_type(0, sequencerLength - 1));
		}

		uint16_t getNextPosition(uint16_t position, uint16_t sequencerLength) {
			return dst(rnd, std::uniform_int_distribution<uint16_t>::param_type(0, sequencerLength - 1));
		}

	private:
		std::mt19937 rnd;
		std::uniform_int_distribution<uint16_t> dst;
	};
}
