#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

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
					void HardwareInitPre (Protonix* device);
					void HardwareInitPost (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareCommand (Protonix* device, String command);
			};
		}
	}
}