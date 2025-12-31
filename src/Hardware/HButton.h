#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

#include "HTrigger.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HButton : public IProtonixHardware {
				private:
					HTrigger* _trigger;
					String _signalChanged;
					String _signalPressed;
					String _signalReleased;

					void _signal (Protonix* device);

				public:
					HButton (unsigned short pin);
					static HButton* Init (unsigned short pin);

					HTrigger* Trigger ();

					/*
					String SignalChanged ();
					HButton* SignalChanged (String signal);

					String SignalPressed ();
					HButton* SignalPressed (String signal);

					String SignalReleased ();
					HButton* SignalReleased (String signal);
					*/

					bool Changed ();
					bool Active ();
					bool Pressed (bool changed = true);
					bool Released (bool changed = true);

					void HardwareConfigSet (String key, Any value);
					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					bool HardwareI2C ();
					bool HardwareSPI ();
					void HardwareInitPost (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwarePipeInterrupt (Protonix* device);
					void HardwareOnReset (Protonix* device);
					void HardwareOnCommand (Protonix* device, String command);
			};
		}
	}
}