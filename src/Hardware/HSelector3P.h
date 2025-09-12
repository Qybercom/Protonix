#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../ProtonixDevice.h"

#include "HTrigger.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HSelector3P : public IProtonixHardware {
				private:
					HTrigger* _trigger1;
					HTrigger* _trigger2;
					short _value;
					bool _changed;

				public:
					HSelector3P (unsigned short pin1, unsigned short pin2, unsigned int checkInterval = 0);

					HTrigger* Trigger1 ();
					HTrigger* Trigger2 ();

					bool Changed (bool allowZero = true);
					short Value ();

					bool HardwareSPI ();
					void HardwareInitPre (ProtonixDevice* device);
					void HardwareInitPost (ProtonixDevice* device);
					void HardwarePipe (ProtonixDevice* device, short core);
					void HardwareCommand (ProtonixDevice* device, String command);
			};
		}
	}
}