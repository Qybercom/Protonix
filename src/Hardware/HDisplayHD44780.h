#pragma once

#include <Arduino.h>

#include "../Common/index.h"

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"
#include "../ProtonixCommand.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HDisplayHD44780 : public IProtonixHardware, public IValueListener {
				private:
					bool _init;

					bool _pulseEnable (char data);
					bool _write4 (char nibble, bool rs);
					bool _send (char value, bool rs);

				public:
					HDisplayHD44780 (
						unsigned short pinRS = 0,
						unsigned short pinRW = 1,
						unsigned short pinEN = 2,
						unsigned short pinBL = 3,
						unsigned short pinD4 = 4,
						unsigned short pinD5 = 5,
						unsigned short pinD6 = 6,
						unsigned short pinD7 = 7
					);

					bool Command (char cmd);
					bool WriteChar (char value);
					bool Write (String value);
					bool Clear ();
					bool Cursor (int x = -1, int y = -1);
					bool Char (unsigned short code, char data[8]);

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					void HardwareInitPost (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareOnCommand (Protonix* device, ProtonixCommand &command);

					void ValueListenerSet (Value &value);

					static HDisplayHD44780* Legacy ();
					static HDisplayHD44780* FC113 ();
					static HDisplayHD44780* FC113Simplified ();
			};
		}
	}
}