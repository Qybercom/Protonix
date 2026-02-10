#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HWeightHX711 : public IProtonixHardware {
				private:
					long _value;
					long _weight;
					unsigned short _readingsCounter;
					long _readingsValue;

				public:
					HWeightHX711 (unsigned short pinDT, unsigned short pinSCK);

					long Value ();
					long Weight ();

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareOnReset (Protonix* device);
					void HardwareOnCommand (Protonix* device, String command);
			};
		}
	}
}