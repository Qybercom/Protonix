#pragma once

#include <Arduino.h>

#include "Common/List.hpp"

#include "../../../AppData/Roaming/JetBrains/CLion2025.2/extensions/old/IProtonixBridge.h"

#include "ProtonixHardwareCapability.h"

namespace Qybercom {
	namespace Protonix {
		class Protonix;

		class IProtonixBridge;

		class IProtonixHardware {
			protected:
				String _id;
				bool _allowSignal;
				short _dedicatedCore = -1;
				IProtonixBridge* _bridge;
				List<ProtonixHardwareCapability*> _capabilities;

				ProtonixHardwareCapability* _capability (String kind, String id, String comment);
				bool _capability (String id, String value);
				bool _log (String message, bool ret = true);

			public:
				String HardwareID ();
				IProtonixHardware* HardwareID (String id);
				short HardwareDedicatedCore ();
				bool HardwareAllowSignal ();
				IProtonixHardware* HardwareAllowSignal (bool allow);
				IProtonixBridge* HardwareBridge ();
				IProtonixHardware* HardwareBridge (IProtonixBridge* bridge);

				virtual String HardwareSummary () { return ""; }
				virtual List<ProtonixHardwareCapability*> &HardwareCapabilities () { return this->_capabilities; }
				virtual void HardwareInitPre (Protonix* device) { (void)device; }
				virtual bool HardwareI2C () { return false; }
				virtual void HardwareI2CPre (Protonix* device) { (void)device; }
				virtual void HardwareI2CPost (Protonix* device) { (void)device; }
				virtual bool HardwareSPI () { return false; }
				virtual void HardwareSPIPre (Protonix* device) { (void)device; }
				virtual void HardwareSPIPost (Protonix* device) { (void)device; }
				virtual void HardwareInitPost (Protonix* device) { (void)device; }

				virtual void HardwarePipe (Protonix* device, short core) { (void)device; (void)core; }
				virtual void HardwareOnReset (Protonix* device) { (void)device; }
				virtual void HardwareOnCommand (Protonix* device, String command) { (void)device; (void)command; }

				virtual void HardwarePipeInterrupt (Protonix* device) { (void)device; }

				virtual ~IProtonixHardware () { };
		};

		class IProtonixBus : public IProtonixHardware {
			public:
				virtual bool BusSend (Protonix* device, String data) { (void)device; (void)data; return false; }
				virtual bool BusCommand (Protonix* device, String command) { (void)device; (void)command; return false; }
		};

		class IProtonixBridge : public IProtonixHardware {
			public:
				virtual bool BridgePinMode (unsigned int pin, int mode);

				virtual bool BridgeDigitalRead (unsigned int pin);
				virtual bool BridgeDigitalWrite (unsigned int pin, bool value);

				virtual int BridgeAnalogRead (unsigned int pin);
				virtual bool BridgeAnalogWrite (unsigned int pin, int value);
		};
	}
}