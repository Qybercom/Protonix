#pragma once

#include <Arduino.h>

#include <QybercomMesonix.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

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
					void HardwareInitPre (Protonix* device);
					void HardwareInitPost (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareCommand (Protonix* device, String command);
			};
		}
	}
}