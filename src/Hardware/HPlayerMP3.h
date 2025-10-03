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

					void _cmd (byte a1, byte a2, byte a3, byte a4);

				public:
					HPlayerMP3 (unsigned short pinRX, unsigned short pinTX);

					HBusSerial* Port ();

					void Play (int file, int dir = 0x00);
					void Pause ();
					void Stop ();
					void Reset ();
					void Wake ();

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareOnCommand (Protonix* device, String command);
			};
		}
	}
}