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
					unsigned short _pin;
					unsigned int _length;
					Photonix* _photonix;

				public:
					HPhotonix (unsigned short pin, unsigned int length);

					Photonix* Driver ();

					String HardwareSummary ();
					void HardwareInitPost (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareOnCommand (Protonix* device, String command);
			};
		}
	}
}