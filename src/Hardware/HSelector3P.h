#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

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
					bool _allowZero;

					bool _changedPipe ();
					void _signal (Protonix* device, short value);

				public:
					HSelector3P (unsigned short pin1, unsigned short pin2, unsigned int checkInterval = 0);

					HTrigger* Trigger1 ();
					HTrigger* Trigger2 ();

					bool Changed ();
					short Value ();

					bool AllowZero ();
					HSelector3P* AllowZero (bool allow);

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					bool HardwareI2C ();
					bool HardwareSPI ();
					void HardwareInitPost (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareOnReset (Protonix* device);
					void HardwareOnCommand (Protonix* device, String command);
			};
		}
	}
}