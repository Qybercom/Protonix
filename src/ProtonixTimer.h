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

				//ProtonixTimer (bool enabled = true);
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

				void Interval (int interval);
				unsigned int Interval ();

				void Unit (ProtonixTimerUnit unit);
				ProtonixTimerUnit Unit ();

				void Enabled (bool enabled);
				bool Enabled ();

				bool Pipe ();

				void Reset ();

			private:
				bool _enabled;
				unsigned long _previous;
				unsigned int _interval;
				ProtonixTimerUnit _unit;
				volatile unsigned long _overflows;
				unsigned long long _totalMS;
				unsigned long long _totalS;
		};
	}
}