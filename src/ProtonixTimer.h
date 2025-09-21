#pragma once

#include <Arduino.h>

namespace Qybercom {
	namespace Protonix {
		#define TIMER_MAX 4294967296ULL

		class ProtonixTimer {
			public:
				enum class ProtonixTimerUnit {
					MICROSECONDS,
					MILLISECONDS
				};

				ProtonixTimer (unsigned int interval, bool enabled = true);
				ProtonixTimer (unsigned int interval, ProtonixTimerUnit unit, bool enabled = true);

				unsigned long Previous ();

				unsigned long long TotalMilliseconds ();
				unsigned long long TotalSeconds ();
				unsigned int RunDays ();
				unsigned int RunHours ();
				unsigned int RunMinutes ();
				unsigned int RunSeconds ();
				unsigned int RunMilliseconds ();
				String RunTime ();

				unsigned int Interval ();
				ProtonixTimer* Interval (int interval);

				ProtonixTimerUnit Unit ();
				ProtonixTimer* Unit (ProtonixTimerUnit unit);

				bool Enabled ();
				ProtonixTimer* Enabled (bool enabled);

				bool Delayed ();
				ProtonixTimer* Delayed (bool delayed);

				bool Pipe ();

				ProtonixTimer* Reset ();

			private:
				bool _enabled;
				bool _delayed;
				bool _delayedConsumed;
				unsigned long _previous;
				unsigned int _interval;
				ProtonixTimerUnit _unit;
				volatile unsigned long _overflows;
				unsigned long long _totalMS;
				unsigned long long _totalS;
		};
	}
}