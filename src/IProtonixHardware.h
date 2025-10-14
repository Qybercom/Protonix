#pragma once

#include <Arduino.h>

#include "Common/List.hpp"

#include "ProtonixHardwareCapability.h"

namespace Qybercom {
	namespace Protonix {
		class Protonix;

		class IProtonixHardware {
			protected:
				String _id;
				bool _allowSignal;
				short _dedicatedCore = -1;
				String _bridge;
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
				String HardwareBridge ();
				IProtonixHardware* HardwareBridge (String bridge);

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
				virtual bool BridgePinInitInput (unsigned int pin, int initial = 0) { (void)pin; (void)initial; return false; }
				virtual bool BridgePinInitInputUp (unsigned int pin, int initial = 0) { (void)pin; (void)initial; return false; }
				virtual bool BridgePinInitInputDown (unsigned int pin, int initial = 0) { (void)pin; (void)initial; return false; }
				virtual bool BridgePinInitOutput (unsigned int pin) { (void)pin; return false; }

				virtual bool BridgePinMode (unsigned int pin, int mode) { (void)pin; (void)mode; return false; }

				virtual bool BridgePinAvailable (unsigned int pin) { (void)pin; return false; }

				virtual bool BridgeDigitalRead (unsigned int pin) { (void)pin; return false; }
				virtual bool BridgeDigitalWrite (unsigned int pin, bool value) { (void)pin; (void)value; return false; }

				virtual int BridgeAnalogRead (unsigned int pin) { (void)pin; return -1; }
				virtual bool BridgeAnalogWrite (unsigned int pin, int value) { (void)pin; (void)value; return false; }
		};
	}
}