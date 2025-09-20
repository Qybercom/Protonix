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

				public:
					HSelector3P (unsigned short pin1, unsigned short pin2, unsigned int checkInterval = 0);

					HTrigger* Trigger1 ();
					HTrigger* Trigger2 ();

					bool Changed ();
					short Value ();

					bool AllowZero ();
					HSelector3P* AllowZero (bool allow);

					bool HardwareSPI ();
					void HardwareInitPre (Protonix* device);
					void HardwareInitPost (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareCommand (Protonix* device, String command);
			};
		}
	}
}