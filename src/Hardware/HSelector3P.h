#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

#include "HTrigger.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HSelector3P : public IProtonixHardware, public IValueListener {
				private:
					HTrigger* _trigger1;
					HTrigger* _trigger2;
					bool _changed;
					short _value;

					bool _changedPipe ();
					void _signal (Protonix* device, short value);

				public:
					HSelector3P (unsigned short pin1, unsigned short pin2);

					HTrigger* Trigger1 ();
					HTrigger* Trigger2 ();

					bool Changed ();
					short Value ();

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					bool HardwareI2C ();
					bool HardwareSPI ();
					void HardwareInitPost (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwarePipeInterrupt (Protonix* device);
					void HardwareOnReset (Protonix* device);
					void HardwareOnCommand (Protonix* device, ProtonixCommand &command);

					void ValueListenerSet (Qybercom::Value &value);
			};
		}
	}
}