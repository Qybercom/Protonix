#pragma once

#include <Arduino.h>
#include <EEPROM.h>

#if defined(AVR)
#define PROTONIX_MEMORY_EEPROM_SIZE 1024
#endif

#if defined(ESP8266)
#include "eboot_command.h"

extern "C" {
	#include "c_types.h"
	#include "spi_flash.h"
	#include "user_interface.h"
}

#include <flash_hal.h> // not "flash_hal.h": can use hijacked MOCK version

#define PROTONIX_MEMORY_EEPROM_SIZE 512
#define PROTONIX_MEMORY_BUFFER_SIZE 256
//#define ADDRESS_END_FLASH 0x00000
//#define ADDRESS_END_FILESYSTEM FS_start - 0x40200000
#endif

#if defined(ESP32)
#define PROTONIX_MEMORY_EEPROM_SIZE 4096
#define PROTONIX_MEMORY_BUFFER_SIZE 256
#endif

namespace Qybercom {
	namespace Protonix {
		class ProtonixMemory {
			private:
				static int _ramFree ();

				bool _eepromReady;

				bool _flashBegin;
				bool _flashVerified;
				uint8_t* _flashBuffer;
				unsigned int _flashBufferSize;
				unsigned int _flashFirmwareSize;
				unsigned int _flashAddressStart;
				unsigned int _flashAddressCurrent;

			public:
				ProtonixMemory ();


				int RAMFree ();


				void EEPROMBegin ();

				template<typename T>
				T &EEPROMGet (int const address, T &data) {
					if (!this->_eepromReady)
						this->EEPROMBegin();

					return EEPROM.get(address, data);
				}

				template<typename T>
				const T &EEPROMSet (int const address, const T &data) {
					if (!this->_eepromReady)
						this->EEPROMBegin();

					return EEPROM.put(address, data);
				}

				template<typename T>
				const T &EEPROMSet (int const address, const T &data, bool commit) {
					if (!this->_eepromReady)
						this->EEPROMBegin();

					T out = EEPROM.put(address, data);

					if (commit)
						this->EEPROMCommit();

					return out;
				}

				bool EEPROMCommit ();

				void EEPROMEnd ();


				int FlashFree ();

				bool FlashFirmwareBegin (size_t size);
				bool FlashFileSystemBegin (size_t size);
				bool FlashWrite (String batch);
				unsigned int FlashSize ();
				unsigned int FlashBufferSize ();
				unsigned int FlashFirmwareSize ();
				#if defined(ESP8266)
				static unsigned int FlashSectorSize (size_t size);
				static unsigned int FlashBufferSizeExpected ();
				#endif
				bool FlashEnd ();

				#if defined(ESP8266)
				static void EBootCommand (uint32_t addressStart, uint32_t addressEnd, size_t size);
				static void EBootCommandRead ();
				#endif
		};
	}
}