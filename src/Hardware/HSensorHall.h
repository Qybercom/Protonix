#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HSensorHall : public IProtonixHardware {
				private:
					unsigned short _pin;
					int _value;
					int _max;
					int _min;

				public:
					HSensorHall (unsigned short pin, int max = QYBERCOM_PROTONIX_ANALOG_MAX, int min = 0);

					unsigned short Pin ();
					HSensorHall* Pin (unsigned short pin);

					int Max ();
					HSensorHall* Max (int max);

					int Min ();
					HSensorHall* Min (int min);

					int Value ();
					bool ValueMatch (int min = 0, int max = QYBERCOM_PROTONIX_ANALOG_MAX, bool minEQ = true, bool maxEQ = true);

					int ValueMapped (int min, int max);

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareOnReset (Protonix* device);
					void HardwareOnCommand (Protonix* device, String command);
			};
		}
	}
}