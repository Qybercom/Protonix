#pragma once

#include <Arduino.h>

#include "IProtonixHardware.hpp"
#include "ProtonixSignal.h"
#include "ProtonixAction.h"

namespace Qybercom {
	namespace Protonix {
		class Protonix;

		class IProtonixDevice {
			public:
				void DeviceHandleCommand (Protonix* device, ProtonixCommand &command);

				virtual unsigned int DeviceTick ();
				virtual void DeviceOnReady (Protonix* device);
				virtual void DeviceOnTick (Protonix* device);
				virtual void DeviceOnLoop (Protonix* device);
				virtual void DeviceOnAction (Protonix* device, ProtonixAction* action);
				virtual void DeviceOnCommand (Protonix* device, ProtonixCommand &command);
				virtual void DeviceOnSignal (Protonix* device, ProtonixSignal* signal);
				#if defined(ESP32)
				virtual void DeviceOnDedicatedTask (Protonix* device, unsigned short core);
				#endif

				virtual ~IProtonixDevice () { }
		};
	}
}