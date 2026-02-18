#pragma once

#include <Arduino.h>

#include <QybercomPhotonix.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			using Qybercom::Photonix::Photonix;

			class HPhotonix : public IProtonixHardware {
				private:
					Photonix* _photonix;

				public:
					HPhotonix (unsigned short pin, unsigned int length);

					Photonix* Driver ();

					String HardwareSummary ();
					void HardwareInitPost (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareOnReset (Protonix* device);
					void HardwareOnCommand (Protonix* device, const ProtonixCommand &command);
			};
		}
	}
}