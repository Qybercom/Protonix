#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

#include "HBusSerial.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HPlayerMP3 : public IProtonixHardware {
				private:
					HBusSerial* _player;

					bool _cmd (byte a1, byte a2, byte a3, byte a4);

				public:
					HPlayerMP3 (unsigned short pinRX, unsigned short pinTX);

					HBusSerial* Port ();

					bool Play (int file, int dir = 0x00);
					bool Pause ();
					bool Stop ();
					bool VolumeUp ();
					bool VolumeDown ();
					bool Volume (short value);
					bool Reset ();
					bool Wake ();

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareOnReset (Protonix* device);
					void HardwareOnCommand (Protonix* device, String command);
			};
		}
	}
}