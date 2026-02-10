#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

#include "HTrigger.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HTouchButton : public IProtonixHardware, public IValueListener {
				private:
					HTrigger* _trigger;

					void _signal (Protonix* device);

				public:
					HTouchButton (unsigned short pin);

					HTrigger* Trigger ();

					bool Changed ();
					bool Active ();
					bool Pressed (bool changed = true);
					bool Released (bool changed = true);

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					bool HardwareI2C ();
					bool HardwareSPI ();
					void HardwarePipe (Protonix* device, short core);
					void HardwarePipeInterrupt (Protonix* device);
					void HardwareOnReset (Protonix* device);
					void HardwareOnCommand (Protonix* device, String command);

					void ValueListenerSet (Value &value);
			};
		}
	}
}