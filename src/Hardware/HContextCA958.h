#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HContextCA958 : public IProtonixHardwareContext {
				public:
					HContextCA958 (char address = 0x70);

					String HardwareSummary ();
					bool HardwareI2C ();
					void HardwareI2CPre (Protonix* device);
					void HardwareI2CPost (Protonix* device);

					void HardwareContextApply (IProtonixHardware* hardware);
			};
		}
	}
}