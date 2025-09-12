#pragma once

#include <Arduino.h>

#include "ProtonixDevice.h"

namespace Qybercom {
	namespace Protonix {
		class ProtonixDevice;

		class IProtonixHardware {
			protected:
				String _id;
				short _dedicatedCore = -1;

			public:
				IProtonixHardware* HardwareID (String id) {
					this->_id = id;

					return this;
				}

				String HardwareID () {
					return this->_id;
				}

				short HardwareDedicatedCore () {
					return this->_dedicatedCore;
				}

				virtual bool HardwareSPI () = 0;

				virtual void HardwareInitPre (ProtonixDevice* device) = 0;
				virtual void HardwareInitPost (ProtonixDevice* device) = 0;

				virtual void HardwarePipe (ProtonixDevice* device, short core) = 0;
				virtual void HardwarePipeInterrupt (ProtonixDevice* device) {}

				virtual void HardwareCommand (ProtonixDevice* device, String command) = 0;

				virtual ~IProtonixHardware () {};
		};
	}
}