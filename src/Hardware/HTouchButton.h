#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HTouchButton : public IProtonixHardware {
				private:
					unsigned short _pin;
					bool _active;

					void _signal (Protonix* device);

				public:
					HTouchButton (unsigned short pin);

					unsigned short Pin ();
					HTouchButton* Pin (unsigned short pin);

					bool Active ();

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareOnReset (Protonix* device);
					void HardwareOnCommand (Protonix* device, String command);
			};
		}
	}
}