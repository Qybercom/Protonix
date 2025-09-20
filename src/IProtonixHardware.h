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
				virtual void HardwareCommand (Protonix* device, String command) = 0;

				virtual void HardwarePipeInterrupt (Protonix* device) { }
				virtual ~IProtonixHardware () { };
		};

		class IProtonixBus : public IProtonixHardware {
		};
	}
}