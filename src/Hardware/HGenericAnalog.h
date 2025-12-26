#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HGenericAnalog : public IProtonixHardware {
				private:
					unsigned short _pin;
					bool _init;
					int _value;

				public:
					HGenericAnalog (unsigned short pin, bool init = false);
					static HGenericAnalog* Init (unsigned short pin, bool init = false);

					bool Init ();

					int Value ();

					unsigned short Pin ();
					HGenericAnalog* Pin (unsigned short pin);

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareOnReset (Protonix* device);
					void HardwareOnCommand (Protonix* device, String command);
			};
		}
	}
}