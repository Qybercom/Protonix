#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HWeightHX711 : public IProtonixHardware {
				private:
					unsigned short _pinDT;
					unsigned short _pinSCK;
					long _value;

				public:
					HWeightHX711 (unsigned short pinDT, unsigned short pinSCK);
					static HWeightHX711* Init (unsigned short pinDT, unsigned short pinSCK);

					long Value ();

					unsigned short PinDT ();
					HWeightHX711* PinDT (unsigned short pin);

					unsigned short PinSCK ();
					HWeightHX711* PinSCK (unsigned short pin);

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareOnReset (Protonix* device);
					void HardwareOnCommand (Protonix* device, String command);
			};
		}
	}
}