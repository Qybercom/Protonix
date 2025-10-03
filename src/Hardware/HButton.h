#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.h"
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

				public:
					HButton (unsigned short pin, unsigned int checkInterval = 0);
					static HButton* Init (unsigned short pin, unsigned int checkInterval = 0);

					HTrigger* Trigger ();

					String SignalChanged ();
					HButton* SignalChanged (String signal);

					String SignalPressed ();
					HButton* SignalPressed (String signal);

					String SignalReleased ();
					HButton* SignalReleased (String signal);

					bool Changed ();
					bool Active ();
					bool Pressed (bool changed = true);
					bool Released (bool changed = true);

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwarePipeInterrupt (Protonix* device);
					void HardwareOnCommand (Protonix* device, String command);
			};
		}
	}
}