#pragma once

#include <Arduino.h>

#include <QybercomMesonix.h>

#include "../IProtonixHardware.h"
#include "../ProtonixDevice.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			using Qybercom::Mesonix::Mesonix;

			class HMesonix : public IProtonixHardware {
				private:
					Mesonix* _reader;

				public:
					HMesonix (unsigned short pinSS, unsigned short pinRST, unsigned short uuidReadDebounce = 0, short dedicatedCore = -1);

					Mesonix* Reader ();

					bool HardwareSPI ();
					void HardwareInitPre (ProtonixDevice* device);
					void HardwareInitPost (ProtonixDevice* device);
					void HardwarePipe (ProtonixDevice* device, short core);
					void HardwareCommand (ProtonixDevice* device, String command);
			};
		}
	}
}