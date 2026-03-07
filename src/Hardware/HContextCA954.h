#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HContextCA954 : public IProtonixHardwareContext {
				public:
					HContextCA954 (char address = 0x70);

					String HardwareSummary ();
					bool HardwareI2C ();
					void HardwareI2CPre (Protonix* device);
					void HardwareI2CPost (Protonix* device);

					void HardwareContextApply (IProtonixHardware* hardware);
			};
		}
	}
}