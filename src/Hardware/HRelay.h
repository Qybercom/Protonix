#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../ProtonixDevice.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HRelay : public IProtonixHardware {
				private:
					unsigned short _pin;
					bool _inverted;

				public:
					HRelay (unsigned short pin, bool inverted = false);

					unsigned short Pin ();
					HRelay* Pin (unsigned short pin);

					void Open ();
					void Close ();

					bool HardwareSPI ();
					void HardwareInitPre (ProtonixDevice* device);
					void HardwareInitPost (ProtonixDevice* device);
					void HardwarePipe (ProtonixDevice* device, short core);
			};
		}
	}
}