#pragma once

#include <Arduino.h>

#include "../../IProtonixDevice.h"
#include "../../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Profile {
			namespace Axion {
				class IAxionDevice : public IProtonixDevice {
					public:
						virtual String AxionDeviceID ();
						virtual String AxionDevicePassphrase ();

						void AxionDeviceOnNetworkConnect (Protonix* device) { (void)device; }
						void AxionDeviceOnClientStreamConnect (Protonix* device) { (void)device; }
				};
			}
		}
	}
}