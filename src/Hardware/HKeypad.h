#pragma once

#include <Arduino.h>

#include "../Common/index.h"

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HKeypad : public IProtonixHardware, public IValueListener {
				public:
					struct Key {
						String Label;
						unsigned short Row;
						unsigned short Column;
						bool Active;

						Key (String label, unsigned short row, unsigned short column);
					};

				private:
					bool _init;
					List<unsigned short> _columns;
					List<unsigned short> _rows;
					List<Key*> _keys;

					void _signal (Protonix* device, String key, bool active);

				public:
					HKeypad ();

					List<Key*> &keys ();

					String HardwareSummary ();
					void HardwareInitPost (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwarePipeInterrupt (Protonix* device);
					void HardwareOnReset (Protonix* device);
					void HardwareOnCommand (Protonix* device, ProtonixCommand &command);

					void ValueListenerSet (Value &value);

					static HKeypad* C4R1 ();
					static HKeypad* C3R4 ();
					static HKeypad* C4R4 ();
			};
		}
	}
}