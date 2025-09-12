#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../ProtonixDevice.h"

#include "HTrigger.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HButton : public IProtonixHardware {
				private:
					HTrigger* _trigger;

				public:
					HButton (unsigned short pin, unsigned int checkInterval = 0);

					HTrigger* Trigger ();

					bool Changed ();
					bool Active ();
					bool Pressed (bool changed = true);
					bool Released (bool changed = true);

					bool HardwareSPI ();
					void HardwareInitPre (ProtonixDevice* device);
					void HardwareInitPost (ProtonixDevice* device);
					void HardwarePipe (ProtonixDevice* device, short core);
					void HardwareCommand (ProtonixDevice* device, String command);
			};
		}
	}
}