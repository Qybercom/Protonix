#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../ProtonixDevice.h"
#include "../ProtonixDevicePort.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HPlayerMP3 : public IProtonixHardware {
				private:
					ProtonixDevicePort* _player;
					String _port;

					void _cmd (byte a1, byte a2, byte a3, byte a4);

				public:
					HPlayerMP3 (String port);

					void Play (int file, int dir = 0x00);
					void Pause ();
					void Stop ();
					void Reset ();
					void Wake ();

					bool HardwareSPI ();
					void HardwareInitPre (ProtonixDevice* device);
					void HardwareInitPost (ProtonixDevice* device);
					void HardwarePipe (ProtonixDevice* device, short core);
			};
		}
	}
}