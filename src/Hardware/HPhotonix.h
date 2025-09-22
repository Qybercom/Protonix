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
					void HardwareSPIPre (Protonix* device);
					void HardwareSPIPost (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareOnCommand (Protonix* device, String command);
			};
		}
	}
}