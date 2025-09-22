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
					bool _input;
					unsigned short _inputMode;
					bool _inputChanged;
					bool _inputValue;
					Qybercom::Debouncer<unsigned short> _debouncer;

					HTrigger (bool input, unsigned short pin, unsigned int checkInterval, unsigned short mode);

				public:
					static HTrigger* Input (unsigned short pin, unsigned short mode = LOW, unsigned int checkInterval = 0);
					static HTrigger* Output (unsigned short pin);

					unsigned short Pin ();

					unsigned short InputMode ();
					bool InputChanged ();
					bool InputValue ();

					bool OutputValue (unsigned short value);

					void HardwareInitPre (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareOnCommand (Protonix* device, String command);
			};
		}
	}
}