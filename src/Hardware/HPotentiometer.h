#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../ProtonixDevice.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HPotentiometer : public IProtonixHardware {
				private:
					unsigned short _pin;
					int _value;
					int _max;
					int _min;

				public:
					HPotentiometer (unsigned short pin, int max = ANALOG_MAX, int min = 0);

					unsigned short Pin ();
					HPotentiometer* Pin (unsigned short pin);

					int Max ();
					HPotentiometer* Max (int max);

					int Min ();
					HPotentiometer* Min (int min);

					int Value ();
					bool ValueMatch (int min = 0, int max = ANALOG_MAX, bool minEQ = true, bool maxEQ = true);

					int ValueMapped (int min, int max);

					bool HardwareSPI ();
					void HardwareInitPre (ProtonixDevice* device);
					void HardwareInitPost (ProtonixDevice* device);
					void HardwarePipe (ProtonixDevice* device, short core);
					void HardwareCommand (ProtonixDevice* device, String command);
			};
		}
	}
}