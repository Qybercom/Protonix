#pragma once

#include <Arduino.h>

#include <QybercomPhotonix.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			using Qybercom::Photonix::Photonix;

			class HPhotonix : public IProtonixHardware {
				private:


				public:
					bool HardwareSPI ();
					void HardwareInitPre (Protonix* device);
					void HardwareInitPost (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareCommand (Protonix* device, String command);
			};
		}
	}
}