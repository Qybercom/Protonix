#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../ProtonixDevice.h"

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
					HSensorHall (unsigned short pin, int max = ANALOG_MAX, int min = 0);

					unsigned short Pin ();
					HSensorHall* Pin (unsigned short pin);

					int Max ();
					HSensorHall* Max (int max);

					int Min ();
					HSensorHall* Min (int min);

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