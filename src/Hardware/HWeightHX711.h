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
					long _weight;
					long _offset;
					float _scale;
					unsigned short _readings;
					unsigned short _readingsCounter;
					long _readingsValue;

				public:
					HWeightHX711 (unsigned short pinDT, unsigned short pinSCK, float scale = 1.0, unsigned short readings = 1);
					static HWeightHX711* Init (unsigned short pinDT, unsigned short pinSCK, float scale = 1.0, unsigned short readings = 1);

					long Value ();
					long Weight ();

					unsigned short PinDT ();
					HWeightHX711* PinDT (unsigned short pin);

					unsigned short PinSCK ();
					HWeightHX711* PinSCK (unsigned short pin);

					long Offset ();
					HWeightHX711* Offset (long offset);

					float Scale ();
					HWeightHX711* Scale (float scale);

					unsigned short Readings ();
					HWeightHX711* Readings (unsigned short count);

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareOnReset (Protonix* device);
					void HardwareOnCommand (Protonix* device, String command);
			};
		}
	}
}