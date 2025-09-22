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

				virtual bool HardwareSPI () = 0;
				virtual void HardwareInitPre (Protonix* device) = 0;
				virtual void HardwareInitPost (Protonix* device) = 0;
				virtual void HardwarePipe (Protonix* device, short core) = 0;
				virtual void HardwareOnCommand (Protonix* device, String command) = 0;

				virtual void HardwarePipeInterrupt (Protonix* device) { (void)device; }

				virtual ~IProtonixHardware () { };
		};

		class IProtonixBus : public IProtonixHardware {
			public:
				virtual bool HardwareBusSend (Protonix* device, String data) = 0;
				virtual bool HardwareBusCommand (Protonix* device, String command) = 0;
		};
	}
}