#pragma once

#include <Arduino.h>
#include <EEPROM.h>

namespace Qybercom {
	namespace Protonix {
		class ProtonixMemory {
			private:
				bool _eepromReady;

			public:
				ProtonixMemory ();


				long RAMFree ();
				long RAMUsed ();
				long RAMTotal ();
				long RAMFragmented ();


				long FlashFree ();
				long FlashUsed ();
				long FlashTotal ();


				void EEPROMBegin ();
				bool EEPROMCommit ();
				void EEPROMEnd ();

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
		};
	}
}