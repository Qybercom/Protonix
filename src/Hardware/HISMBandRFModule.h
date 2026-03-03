#pragma once

#include <Arduino.h>
#include <RCSwitch.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HISMBandRFModule : public IProtonixHardware {
				private:
					bool _init;
					RCSwitch* _remote;
					String _value;
					String _valueActual;
					bool _valueChanged;

					void _signal (Protonix* device, String value);

				public:
					HISMBandRFModule (short pinRX = -1, short pinTX = -1);

					static HISMBandRFModule* RX (unsigned short pin);
					static HISMBandRFModule* TX (unsigned short pin);

					String Value ();
					String ValueActual ();
					bool ValueChanged ();

					bool Send (String value);

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					void HardwareInitPost (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareOnReset (Protonix* device);
					void HardwareOnCommand (Protonix* device, ProtonixCommand &command);
			};
		}
	}
}