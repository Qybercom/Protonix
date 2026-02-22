#pragma once

#include <Arduino.h>
#include <MFRC522.h>
#include "MFRC522_I2C_Library.h"

#include "../Common/index.h"

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HReaderNFC : public IProtonixHardware {
				private:
					bool _init;
					MFRC522* _spiReader;
					MFRC522_I2C* _i2cReader;
					String _uuid;
					String _uuidActual;
					bool _uuidChanged;
					Qybercom::Pipes::Debouncer<String>* _debouncer;

					HReaderNFC (String mode, unsigned short pinRST, short dedicatedCore = -1);
					void _signal (Protonix* device, String value);

					static void _channel (unsigned short channel);

				public:
					static HReaderNFC* InitI2C (unsigned short pinRST, int address = 0x28, short dedicatedCore = -1);
					static HReaderNFC* InitSPI (unsigned short pinSS, unsigned short pinRST, short dedicatedCore = -1);

					String UUID ();
					String UUIDActual ();
					bool UUIDChanged ();

					Qybercom::Pipes::Debouncer<String>* Debouncer ();

					String HardwareSummary ();

					void HardwareInitPre (Protonix* device);

					bool HardwareI2C ();
					void HardwareI2CPre (Protonix* device);
					void HardwareI2CPost (Protonix* device);

					bool HardwareSPI ();
					void HardwareSPIPre (Protonix* device);
					void HardwareSPIPost (Protonix* device);

					void HardwareInitPost (Protonix* device);

					void HardwarePipe (Protonix* device, short core);
					void HardwareOnReset (Protonix* device);
					void HardwareOnCommand (Protonix* device, ProtonixCommand &command);
			};
		}
	}
}