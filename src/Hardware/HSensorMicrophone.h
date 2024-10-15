#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../ProtonixDevice.h"

namespace Qybercom {
	namespace Protonix {
        namespace Hardware {
			class HSensorMicrophone : public IProtonixHardware {
                private:


	            public:
					void HardwareInitPre(ProtonixDevice* device);
					void HardwareInitPost(ProtonixDevice* device);
                    void HardwarePipe(ProtonixDevice* device);
			};
        }
	}
}