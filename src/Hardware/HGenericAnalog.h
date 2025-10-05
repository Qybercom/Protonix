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

					unsigned short Pin ();
					bool Init ();
					int Value ();

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareOnReset (Protonix* device);
					void HardwareOnCommand (Protonix* device, String command);
			};
		}
	}
}