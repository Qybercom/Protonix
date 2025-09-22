#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.h"
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

					bool Init ();
					int Value ();

					bool HardwareSPI ();
					void HardwareInitPre (Protonix* device);
					void HardwareInitPost (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareOnCommand (Protonix* device, String command);
			};
		}
	}
}