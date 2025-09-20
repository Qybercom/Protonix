#pragma once

#include <Arduino.h>

#include "../../IProtonixDevice.h"
/*#include "../../IProtonixHardware.h"
#include "../../IProtonixCommand.h"*/
#include "../../Protonix.h"
/*#include "../../ProtonixAction.h"
#include "../../ProtonixSignal.h"*/

namespace Qybercom {
	namespace Protonix {
		namespace Profile {
			namespace Axion {
				class IAxionDevice : public IProtonixDevice {
					public:
						/*virtual unsigned int DeviceTick () = 0;
						virtual void DeviceOnReady (Protonix* device) = 0;
						virtual void DeviceOnTick (Protonix* device) = 0;
						virtual void DeviceOnLoop (Protonix* device) = 0;
						virtual void DeviceOnAction (Protonix* device, ProtonixAction* action) = 0;
						virtual void DeviceOnCommand (Protonix* device, IProtonixCommand* command, IProtonixHardware* hardware) = 0;
						virtual void DeviceOnCommandCustom (Protonix* device, String command) = 0;
						virtual void DeviceOnSignal (Protonix* device, ProtonixSignal* signal) = 0;*/

						virtual String AxionDeviceID ();
						virtual String AxionDevicePassphrase ();

						void AxionDeviceOnNetworkConnect (Protonix* device) { (void)device; }
						void AxionDeviceOnClientStreamConnect (Protonix* device) { (void)device; }
				};
			}
		}
	}
}