#pragma once

#include <Arduino.h>

namespace Qybercom {
	namespace Protonix {
		class Protonix;

		class IProtonixHardware {
			protected:
				String _id;
				bool _allowSignal;
				short _dedicatedCore = -1;
				bool _log (String message, bool ret = true);

			public:
				String HardwareID ();
				IProtonixHardware* HardwareID (String id);
				short HardwareDedicatedCore ();
				bool HardwareAllowSignal ();
				IProtonixHardware* HardwareAllowSignal (bool allow);

				virtual void HardwareInitPre (Protonix* device) { (void)device; }
				virtual bool HardwareI2C () { return false; }
				virtual void HardwareI2CPre (Protonix* device) { (void)device; }
				virtual void HardwareI2CPost (Protonix* device) { (void)device; }
				virtual bool HardwareSPI () { return false; }
				virtual void HardwareSPIPre (Protonix* device) { (void)device; }
				virtual void HardwareSPIPost (Protonix* device) { (void)device; }
				virtual void HardwareInitPost (Protonix* device) { (void)device; }

				virtual void HardwarePipe (Protonix* device, short core) { (void)device; (void)core; }
				virtual void HardwareOnCommand (Protonix* device, String command) { (void)device; (void)command; }

				virtual void HardwarePipeInterrupt (Protonix* device) { (void)device; }

				virtual ~IProtonixHardware () { };
		};

		class IProtonixBus : public IProtonixHardware {
			public:
				virtual bool HardwareBusSend (Protonix* device, String data) { (void)device; (void)data; return false; }
				virtual bool HardwareBusCommand (Protonix* device, String command) { (void)device; (void)command; return false; }
		};
	}
}