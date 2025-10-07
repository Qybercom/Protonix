#pragma once

#include <Arduino.h>

#include "Common/Debouncer.hpp"

#include "../IProtonixHardware.h"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HTrigger : public IProtonixHardware {
				private:
					unsigned short _pin;
					bool _interrupt;
					bool _input;
					unsigned short _inputMode;
					bool _inputChanged;
					bool _inputChangedSignal;
					bool _inputValue;
					Qybercom::Debouncer<unsigned short> _debouncer;
					String _signalInputChanged;
					IProtonixBridge* _bridgePtr;

					bool _pipe ();
					bool _inputChangedHandler ();
					void _signal (Protonix* device);
					HTrigger (bool input, unsigned short pin, unsigned int checkInterval, unsigned short mode, bool interrupt);

				public:
					static HTrigger* Input (unsigned short pin, unsigned short mode = LOW, unsigned int checkInterval = 0, bool interrupt = false);
					static HTrigger* Output (unsigned short pin);

					unsigned short Pin ();
					unsigned short InputMode ();
					Qybercom::Debouncer<unsigned short> &Debouncer ();

					bool Interrupt ();
					HTrigger* Interrupt (bool interrupt);

					String SignalInputChanged ();
					HTrigger* SignalInputChanged (String signal);

					bool InputChanged ();
					bool InputValue ();

					bool OutputValue (unsigned short value);

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