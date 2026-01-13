#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HGenericPWM : public IProtonixHardware {
				private:
					unsigned short  _pin;
					bool _inverted;
					unsigned int _frequency;
					unsigned short _resolution;
					unsigned int _max;
					bool _started;
					short _channel;

					static bool _channels[16];
					static bool _init;

					static short _allocate ();

				public:
					HGenericPWM (unsigned short pin, bool inverted = false, unsigned int frequency = 1000, unsigned resolution = 8);

					HGenericPWM* Max (unsigned int max);
					unsigned int Max ();

					HGenericPWM* Frequency (unsigned int frequency);
					unsigned int Frequency ();

					HGenericPWM* Resolution (unsigned short resolution);
					unsigned short Resolution ();

					bool Start ();
					bool Write (unsigned int value);
					bool WritePercent (unsigned short percent);
					bool Stop ();
			};
		}
	}
}