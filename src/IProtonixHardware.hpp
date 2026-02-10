#pragma once

#include <Arduino.h>

#include "Common/index.h"

#include "ProtonixHardwareCapability.h"

namespace Qybercom {
	namespace Protonix {
		class Protonix;

		class IProtonixBridge;
		class IProtonixHardwareContext;

		class IProtonixHardware {
			protected:
				String _id;
				short _dedicatedCore = -1;
				IProtonixBridge* _bridge;
				IProtonixHardwareContext* _context;
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

				IProtonixBridge* HardwareBridge () {
					return this->_bridge;
				}

				IProtonixHardware* HardwareBridge (IProtonixBridge* bridge) {
					this->_bridge = bridge;

					return this;
				}

				IProtonixHardwareContext* HardwareContext () {
					return this->_context;
				}

				IProtonixHardware* HardwareContext (IProtonixHardwareContext* context) {
					this->_context = context;

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
				virtual bool BridgePinDualPolarity (unsigned int pin) { (void)pin; return false; }

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

		class IProtonixHardwareContext : public IProtonixHardware {
			protected:
				Qybercom::Value _contextData;

			public:
				void HardwareContextData (String hardwareID, const Qybercom::Value &data) {
					this->_contextData[hardwareID] = data;
				}

				Qybercom::Value HardwareContextData (String hardwareID) {
					return this->_contextData[hardwareID];
				}

				virtual void HardwareContextApply (IProtonixHardware* hardware) { (void)hardware; }
		};
	}
}