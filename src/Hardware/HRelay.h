#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HRelay : public IProtonixHardware {
				public:
					HRelay (unsigned short pin, bool inverted = false);

					void Open ();
					void Close ();

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					void HardwareOnReset (Protonix* device);
					void HardwareOnCommand (Protonix* device, ProtonixCommand &command);
			};
		}
	}
}