#pragma once

#include <Arduino.h>

#include "Common/index.h"

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
				Value _config;

				ProtonixHardwareCapability* _capability (String kind, String id, String comment) {
					for (ProtonixHardwareCapability* capability : this->_capabilities)
						if (capability->ID() == id) return capability;

					ProtonixHardwareCapability* capability = new ProtonixHardwareCapability(kind, id, comment);

					this->_capabilities.Add(capability);

					return capability;
				}

				bool _capability (String id, String value) {
					for (ProtonixHardwareCapability* capability : this->_capabilities) {
						if (capability->ID() != id) continue;

						capability->Value(value);

						return true;
					}

					return false;
				}

				bool _log (String message, bool ret = true) {
					Serial.println("[hardware:" + this->_id + "] " + message);

					return ret;
				}

			public:
				String HardwareID () {
					return this->_id;
				}

				IProtonixHardware* HardwareID (String id) {
					this->_id = id;

					return this;
				}

				short HardwareDedicatedCore () {
					return this->_dedicatedCore;
				}

				bool HardwareAllowSignal () {
					return this->_allowSignal;
				}

				IProtonixHardware* HardwareAllowSignal (bool allow) {
					this->_allowSignal = allow;

					return this;
				}

				IProtonixBridge* HardwareBridge () {
					return this->_bridge;
				}

				IProtonixHardware* HardwareBridge (IProtonixBridge* bridge) {
					this->_bridge = bridge;

					return this;
				}

				List<ProtonixHardwareCapability*> &HardwareCapabilities () {
					return this->_capabilities;
				}

				Value &HardwareConfig () {
					return this->_config;
				}

				virtual String HardwareSummary () { return ""; }

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