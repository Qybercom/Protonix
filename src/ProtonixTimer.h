#pragma once

#include <Arduino.h>

namespace Qybercom {
	namespace Protonix {
		class ProtonixTimer {
			public:
				enum class ProtonixTimerUnit {
					MICROSECONDS,
					MILLISECONDS
				};

				ProtonixTimer();
				ProtonixTimer(unsigned int interval);
				ProtonixTimer(unsigned int interval, ProtonixTimerUnit unit);

				unsigned long Previous();

				void Interval(int interval);
				unsigned int Interval();

				void Unit(ProtonixTimerUnit unit);
				ProtonixTimerUnit Unit();

				void Enabled(bool enabled);
				bool Enabled();

				bool Pipe();

				void Reset();

			private:
				bool _enabled;
				unsigned long _previous;
				unsigned int _interval;
				ProtonixTimerUnit _unit;
		};
	}
}