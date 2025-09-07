#pragma once

#include <Arduino.h>

#include "Common/Debouncer.hpp"

#include "../IProtonixHardware.h"
#include "../ProtonixDevice.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HSensorEncoder : public IProtonixHardware {
				private:
					unsigned short _pinA;
					unsigned short _pinB;
					volatile bool _valA;
					volatile bool _valB;
					volatile short _dir;
					volatile bool _changed;
					volatile unsigned long _checkLast;
					volatile unsigned int _checkInterval;
					Qybercom::Debouncer<short> _debouncer;
					//volatile unsigned long _debounceValues[3];

				public:
					HSensorEncoder (unsigned short pinA, unsigned short pinB, unsigned int checkInterval = 0);

					unsigned short PinA ();
					unsigned short PinB ();
					bool ValA ();
					bool ValB ();
					short Dir ();
					bool Changed (bool allowZero = false);
					unsigned long CheckLast ();
					unsigned int CheckInterval ();
					Qybercom::Debouncer<short> &Debouncer ();

					bool HardwareSPI ();
					void HardwareInitPre (ProtonixDevice* device);
					void HardwareInitPost (ProtonixDevice* device);
					void HardwarePipe (ProtonixDevice* device, short core);
					void HardwarePipeInterrupt (ProtonixDevice* device);
			};
		}
	}
}