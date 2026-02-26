#include <Arduino.h>
#include <EEPROM.h>

#include "Common/index.h"

#include "_platforms.h"
#include "ProtonixMemory.h"

using namespace Qybercom::Protonix;


#if defined(ESP32)
#elif defined(ESP8266)
#elif defined(AVR)
extern int __heap_start, * __brkval;
extern char __data_load_end;
#endif



ProtonixMemory::ProtonixMemory () {
	this->_eepromReady = false;
}



long ProtonixMemory::RAMFree () {
	#if defined(ESP32)
		return (long)ESP.getFreeHeap();
	#elif defined(ESP8266)
		uint32_t mFree = 0;
		uint32_t mMax = 0;
		uint8_t mFragmented = 0;
		ESP.getHeapStats(&mFree, &mMax, &mFragmented);

		return (long)mFree;
	#elif defined(AVR)
		int v;
		// https://docs.arduino.cc/learn/programming/memory-guide
		return (long)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
	#else
		return -1;
	#endif
}

long ProtonixMemory::RAMUsed () {
	long used = this->RAMTotal() - this->RAMFree();

	return used < 0 ? -1 : used;
}

long ProtonixMemory::RAMTotal () {
	#if defined(ESP32)
		return (long)ESP.getHeapSize();
	#elif defined(ESP8266)
		uint32_t mFree = 0;
		uint32_t mMax = 0;
		uint8_t mFragmented = 0;
		ESP.getHeapStats(&mFree, &mMax, &mFragmented);
		//Serial.printf("free: %7u - max: %7u - frag: %3d%% <- ", mFree, mMax, mFragmented);

		return (long)mMax;
	#elif defined(ARDUINO_ARCH_AVR)
		return (long)(RAMEND + 1);
	#else
		return -1;
	#endif
}

long ProtonixMemory::RAMFragmented () {
	#if defined(ESP32)
		return -1;
	#elif defined(ESP8266)
		uint32_t mFree = 0;
		uint32_t mMax = 0;
		uint8_t mFragmented = 0;
		ESP.getHeapStats(&mFree, &mMax, &mFragmented);

		return (long)mFragmented;
	#else
		return -1;
	#endif
}



long ProtonixMemory::FlashFree () {
	#if defined(ESP32) || defined(ESP8266)
		return (long)ESP.getFreeSketchSpace();
	#elif defined(AVR)
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
	#if defined(ESP32) || defined(ESP8266)
		long total = (long)ESP.getFlashChipSize();
		long free  = (long)ESP.getFreeSketchSpace();

		return total - free;
	#elif defined(AVR)
		return (long)&__data_load_end;
	#else
		return -1;
	#endif
}

long ProtonixMemory::FlashTotal () {
	#if defined(ESP32) || defined(ESP8266)
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



Qybercom::Value ProtonixMemory::DTO (bool first) {
	(void)first;

	Value out = Value::Object();

	out["ram_free"] = this->RAMFree();
	//out["ram_used"] = this->RAMUsed();
	out["ram_total"] = this->RAMTotal();
	//out["ram_fragmented"] = this->RAMFragmented();
	out["flash_free"] = this->FlashFree();
	//out["flash_used"] = this->FlashUsed();
	out["flash_total"] = this->FlashTotal();

	return out;
}