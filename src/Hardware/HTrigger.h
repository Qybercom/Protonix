#pragma once

#include <Arduino.h>

#include "../Common/index.h"

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HTrigger : public IProtonixHardware {
				private:
					bool _init;
					bool _inputChanged;
					bool _inputChangedSignal;
					bool _inputValue;

					Qybercom::Pipes::Debouncer<bool>* _debouncer;

					bool _pipe ();
					bool _inputChangedHandler ();
					void _signal (Protonix* device);
					HTrigger (String mode, unsigned short pin);

				public:
					static HTrigger* Input (unsigned short pin);
					static HTrigger* Output (unsigned short pin);

					Qybercom::Pipes::Debouncer<bool>* Debouncer ();

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
					void HardwareOnCommand (Protonix* device, const ProtonixCommand &command);
			};
		}
	}
}