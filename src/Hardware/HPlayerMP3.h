#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

#include "HBusSerial.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HPlayerMP3 : public IProtonixHardware {
				private:
					HBusSerial* _player;

					bool _cmd (char a1, char a2, char a3, char a4);

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
					void HardwareOnCommand (Protonix* device, ProtonixCommand &command);
			};
		}
	}
}