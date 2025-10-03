#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HPhotoResistor : public IProtonixHardware {
				private:
					unsigned short _pin;
					int _value;
					int _max;
					int _min;

				public:
					HPhotoResistor (unsigned short pin, int max = QYBERCOM_PROTONIX_ANALOG_MAX, int min = 0);

					unsigned short Pin ();
					HPhotoResistor* Pin (unsigned short pin);

					int Max ();
					HPhotoResistor* Max (int max);

					int Min ();
					HPhotoResistor* Min (int min);

					int Value ();
					bool ValueMatch (int min = 0, int max = QYBERCOM_PROTONIX_ANALOG_MAX, bool minEQ = true, bool maxEQ = true);

					int ValueMapped (int min, int max);

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareOnCommand (Protonix* device, String command);
			};
		}
	}
}