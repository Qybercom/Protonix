#pragma once

#include <Arduino.h>
#include <Wire.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"
#include "../ProtonixTimer.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HBridgeADS1115 : public IProtonixBridge {
				public:
					enum FLAGS {
						OS_SINGLE   = 0x8000,
						PGA_4_096V  = 0x0200,
						MODE_SINGLE = 0x0100,
						DR_128SPS   = 0x0080,
						COMP_OFF    = 0x0003
					};

					static unsigned int CMDReadPin (unsigned short pin);

				private:
					byte _address;
					bool _init;
					ProtonixTimer* _readTimer;
					unsigned short _readPin;
					int _values[4];

					unsigned int _read (byte reg);
					bool _write (byte reg, unsigned int value);

				public:
					HBridgeADS1115 (byte address);
					static HBridgeADS1115* Init (byte address);

					ProtonixTimer* ReadTimer ();

					byte Address ();
					HBridgeADS1115* Address (byte address);

					bool HardwareI2C ();
					void HardwareI2CPre (Protonix* device);
					void HardwareI2CPost (Protonix* device);
					void HardwarePipe (Protonix* device, short core);

					bool BridgePinInitInput (unsigned int pin, int initial = 0);
					bool BridgePinInitInputUp (unsigned int pin, int initial = 0);
					bool BridgePinInitInputDown (unsigned int pin, int initial = 0);
					bool BridgePinInitOutput (unsigned int pin);
					bool BridgePinMode (unsigned int pin, int mode);
					bool BridgePinAvailable (unsigned int pin);
					bool BridgeDigitalRead (unsigned int pin);
					bool BridgeDigitalWrite (unsigned int pin, bool value);
					int BridgeAnalogRead (unsigned int pin);
					bool BridgeAnalogWrite (unsigned int pin, int value);
			};
		}
	}
}