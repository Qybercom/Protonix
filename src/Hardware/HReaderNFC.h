#pragma once

#include <Arduino.h>
#include <MFRC522.h>
#include "MFRC522_I2C_Library.h"

#include "Common/Debouncer.hpp"

#include "../IProtonixHardware.h"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HReaderNFC : public IProtonixHardware {
				private:
					bool _init;
					unsigned short _pinSS;
					unsigned short _pinRST;
					bool _spi;
					MFRC522* _spiReader;
					bool _i2c;
					MFRC522_I2C* _i2cReader;
					int _i2cAddress;
					String _uuid;
					String _uuidActual;
					bool _uuidChanged;
					Qybercom::Debouncer<String> _debouncer; // use >= 1000

					HReaderNFC (unsigned short pinSS, unsigned short pinRST, unsigned int uuidReadDebounce = 0, short dedicatedCore = -1);

					static void _channel (unsigned short channel);

				public:
					static HReaderNFC* InitI2C (unsigned short pinSS, unsigned short pinRST, int address = 0x28, unsigned int uuidReadDebounce = 0, short dedicatedCore = -1);
					static HReaderNFC* InitSPI (unsigned short pinSS, unsigned short pinRST, unsigned int uuidReadDebounce = 0, short dedicatedCore = -1);

					String UUID ();
					String UUIDActual ();
					bool UUIDChanged ();

					Qybercom::Debouncer<String> &Debouncer ();

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
					void HardwareOnCommand (Protonix* device, String command);
			};
		}
	}
}