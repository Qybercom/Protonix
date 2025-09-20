#include <Arduino.h>
#include <EEPROM.h>

#include "_platforms.h"
#include "ProtonixMemory.h"

using namespace Qybercom::Protonix;



#if defined(AVR)
extern int __heap_start, * __brkval;
#endif



ProtonixMemory::ProtonixMemory () {
	this->_eepromReady = false;
}



long ProtonixMemory::RAMFree () {
	#if defined(ESP8266) || defined(ESP32)
		return (long)ESP.getFreeHeap();
	#elif defined(AVR)
		int v;
		// https://docs.arduino.cc/learn/programming/memory-guide
		return (long)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
	#else
		return -1;
	#endif
}

long ProtonixMemory::RAMUsed () {
	#if defined(ESP8266) || defined(ESP32)
		long total = (long)ESP.getHeapSize();
		long free = (long)ESP.getFreeHeap();

		return total - free;
	#elif defined(ARDUINO_ARCH_AVR)
		long total = (long)(RAMEND + 1);
		long free = this->RAMFree();

		return total - free;
	#else
		return -1;
	#endif
}

long ProtonixMemory::RAMTotal () {
	#if defined(ESP8266) || defined(ESP32)
		return (long)ESP.getHeapSize();
	#elif defined(ARDUINO_ARCH_AVR)
		return (long)(RAMEND + 1);
	#else
		return -1;
	#endif
}



long ProtonixMemory::FlashFree () {
	#if defined(ESP8266) || defined(ESP32)
		return (long)ESP.getFreeSketchSpace();
	#elif defined(ARDUINO_ARCH_AVR)
		extern char __data_load_end;

		long used  = (long)&__data_load_end;
		long total = (long)(FLASHEND + 1);

		if (used > total)
			used = total;

		return total - used;
	#else
		return -1;
	#endif
}

long ProtonixMemory::FlashUsed () {
	#if defined(ESP8266) || defined(ESP32)
		long total = (long)ESP.getFlashChipSize();
		long free  = (long)ESP.getFreeSketchSpace();

		return total - free;
	#elif defined(ARDUINO_ARCH_AVR)
		extern char __data_load_end;

		return (long)&__data_load_end;
	#else
		return -1;
	#endif
}

long ProtonixMemory::FlashTotal () {
	#if defined(ESP8266) || defined(ESP32)
		return (long)ESP.getFlashChipSize();
	#elif defined(ARDUINO_ARCH_AVR)
		return (long)(FLASHEND + 1);
	#else
		return -2;
	#endif
}



void ProtonixMemory::EEPROMBegin () {
	#if defined(ESP32) || defined(ESP8266)
	EEPROM.begin(QYBERCOM_PROTONIX_MEMORY_EEPROM_SIZE);
	#endif

	this->_eepromReady = true;
}

bool ProtonixMemory::EEPROMCommit () {
	#if defined(ESP32) || defined(ESP8266)
	return this->_eepromReady ? EEPROM.commit() : false;
	#else
	return this->_eepromReady;
	#endif
}

void ProtonixMemory::EEPROMEnd () {
	#if defined(ESP32) || defined(ESP8266)
	EEPROM.end();
	#endif

	this->_eepromReady = false;
}