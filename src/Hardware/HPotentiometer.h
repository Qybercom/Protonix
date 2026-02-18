#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HPotentiometer : public IProtonixHardware {
				private:
					int _value;

				public:
					HPotentiometer (unsigned short pin, int max = QYBERCOM_PROTONIX_ANALOG_MAX, int min = 0);

					int Value ();
					bool ValueMatch (int min = 0, int max = QYBERCOM_PROTONIX_ANALOG_MAX, bool minEQ = true, bool maxEQ = true);
					int ValueMapped (int min, int max);

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareOnReset (Protonix* device);
					void HardwareOnCommand (Protonix* device, const ProtonixCommand &command);
			};
		}
	}
}