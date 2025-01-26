#pragma once

#include <Arduino.h>

#include "ProtonixDevice.h"

namespace Qybercom {
	namespace Protonix {
		class ProtonixDevice;

		class IProtonixHardware {
			protected:
				String _id;

			public:
				IProtonixHardware* HardwareID(String id) {
					this->_id = id;

					return this;
				}

				String HardwareID() {
					return this->_id;
				}

				virtual void HardwareInitPre(ProtonixDevice* device) = 0;
				virtual void HardwareInitPost(ProtonixDevice* device) = 0;

				virtual void HardwarePipe(ProtonixDevice* device) = 0;
		};
	}
}