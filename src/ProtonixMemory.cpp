#include <Arduino.h>

#include "ProtonixMemory.h"

using namespace Qybercom::Protonix;



ProtonixMemory::ProtonixMemory() {
	this->_eepromReady = false;

    this->_flashBegin = false;
    this->_flashVerified = false;
}



extern int __heap_start, * __brkval;

int ProtonixMemory::_ramFree() {
	#if defined(ESP32)
		return esp_get_free_heap_size();
	#elif defined(ESP8266)
		return ESP.getFreeHeap();
	#elif defined(AVR)
		// https://docs.arduino.cc/learn/programming/memory-guide
		int v;
		//int __heap_start, * __brkval;

		return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
	#else
		return 0;
	#endif
}

int ProtonixMemory::RAMFree() {
	return ProtonixMemory::_ramFree();
}



void ProtonixMemory::EEPROMBegin() {
    EEPROM.begin(PROTONIX_MEMORY_EEPROM_SIZE);
    this->_eepromReady = true;
}

bool ProtonixMemory::EEPROMCommit() {
    return this->_eepromReady ? EEPROM.commit() : false;
}

void ProtonixMemory::EEPROMEnd() {
    EEPROM.end();
    this->_eepromReady = false;
}



int ProtonixMemory::FlashFree() {
	#if defined(ESP32) || defined(ESP8266)
		return ESP.getFreeSketchSpace();
	#elif defined(AVR)
		return 0;
	#else
		return 0;
	#endif
}

bool ProtonixMemory::FlashFirmwareBegin(size_t size) {
	#if defined(ESP8266)
	/**
	 * Check boot mode; if boot mode is 1 (UART download mode),
	 * we will not be able to reset into normal mode once update is done.
	 * Fail early to avoid frustration.
	 * https://github.com/esp8266/Arduino/issues/1017#issuecomment-200605576
	 */
	int boot_mode = (GPI >> 16) & 0xf;
	if (boot_mode == 1) {
        return false;
    }

	if (size == 0) return false;
	if (!ESP.checkFlashConfig(false)) return false;

	//size of current sketch rounded to a sector
	size_t currentSketchSize = ProtonixMemory::FlashSectorSize(this->FlashFirmwareSize());
	//size of the update rounded to a sector
	size_t roundedSize = ProtonixMemory::FlashSectorSize(size);

    //address of the end of the space available for sketch and update
    uintptr_t updateEndAddress = FS_start - 0x40200000; // TODO: define by platform and selected layout!!!
    uintptr_t updateStartAddress = (updateEndAddress > roundedSize) ? (updateEndAddress - roundedSize) : 0;

    //make sure that the size of both sketches is less than the total space (updateEndAddress)
    if (updateStartAddress < currentSketchSize) {
    	return false;
    }

	//initialize
	this->_flashAddressStart = updateStartAddress;
	this->_flashAddressCurrent = updateStartAddress;
	this->_flashFirmwareSize = size;
    this->_flashBufferSize = ProtonixMemory::FlashBufferSizeExpected();
	//this->_flashBuffer = new uint8_t[this->_flashBufferSize];

    this->_flashBegin = true;

	return true;
    #elif defined(ESP32)
		return false;
	#elif defined(AVR)
		return false;
	#else
		return false;
	#endif
}

bool ProtonixMemory::FlashFileSystemBegin(size_t size) {
	return false;
}

bool ProtonixMemory::FlashWrite(String batch) {
	if (!this->_flashBegin) return false;

    int i = 0;
    uint8_t c;

	if (!this->_flashVerified) {
		c = batch[0];

        if ((c != 0xE9) && (c != 0x1f)) {
    		Serial.println("[WARNING] ProtonixMemory::FlashWrite(): invalid firmware format");

        	this->_flashBegin = false;

        	return false;
        }

		// If the flash settings don't match what we already have, modify them.
		// But restore them after the modification, so the hash isn't affected.
		// This is analogous to what esptool.py does when it receives a --flash_mode argument.
		/*FlashMode_t flashMode = ESP.getFlashChipMode();
    	FlashMode_t bufferFlashMode = ESP.magicFlashChipMode(batch[2]);
    	bool modifyFlashMode = bufferFlashMode != flashMode;
        if (modifyFlashMode) batch[2] = flashMode;
		*/
        batch[2] = 0x3B;
        i++;
	}

    this->_flashVerified = true;

	//bool result = true;
	//if (this->_flashAddressCurrent % FLASH_SECTOR_SIZE == 0) {
	//}

    //while (i < this->_flashBufferSize) {
    //	i++;
    //}
    #if defined(ESP8266)
    int sector = this->_flashAddressCurrent / FLASH_SECTOR_SIZE;

	//Serial.println("[debug] ProtonixMemory::FlashWrite(): erasing sector " + String(sector));

    if (!ESP.flashEraseSector(sector)) {
        Serial.println("[WARNINNG] ProtonixMemory::FlashWrite(): failed to erase sector " + String(sector));

    	return false;
    }

    this->_flashBuffer = (uint8_t*)batch.c_str();

    //Serial.println("[debug] ProtonixMemory::FlashWrite(): writing address " + String(this->_flashAddressCurrent));

    if (!ESP.flashWrite(this->_flashAddressCurrent, this->_flashBuffer, this->_flashBufferSize)) {
    	Serial.println("[WARNING] ProtonixMemory::FlashWrite(): failed to write to " + String(this->_flashAddressCurrent));

    	return false;
    }

    Serial.println("[debug] ProtonixMemory::FlashWrite(): written address:" + String(this->_flashAddressCurrent) + " sector:" + String(sector));

    this->_flashAddressCurrent += FLASH_SECTOR_SIZE;

    //delay(1);

    return true;
    #elif defined(ESP32)
    return false;
	#elif defined(AVR)
    return false;
	#else
    return false;
    #endif
}

bool ProtonixMemory::FlashEnd() {
	if (!this->_flashBegin) return false;

    // other verifications
    if (this->_flashBuffer != nullptr) {
    	//delete this->_flashBuffer;
    	//this->_flashBuffer = nullptr;
    }

    /*
    uint8_t buf[4] __attribute__((aligned(4)));
    if (!ESP.flashRead(this->_flashAddressStart, (uint32_t *) &buf[0], 4)) {
        this->_flashBegin = false;

        Serial.println("[WARNING] ProtonixMemory::FlashEnd(): failed to read from " + String(this->_flashAddressStart));

        return false;
    }

    uint32_t bin_flash_size = ESP.magicFlashChipSize((buf[3] & 0xf0) >> 4);

    if (buf[0] != 0xE9 || bin_flash_size > ESP.getFlashChipRealSize()) {
        this->_flashBegin = false;

        Serial.println("[WARNING] ProtonixMemory::FlashEnd(): invalid flash bin format or size");

        return false;
    }
     */

    #if defined(ESP8266)
    	//ProtonixMemory::EBootCommandRead();
    	ProtonixMemory::EBootCommand(
    		this->_flashAddressStart,
        	0x00000,
        	this->_flashFirmwareSize
    	);

    	return true;
    	//ProtonixMemory::EBootCommandRead();
    #elif defined(ESP32)
    	return false;
    #elif defined(AVR)
    #else
    	return false;
    #endif
}



unsigned int ProtonixMemory::FlashSize() {
	#if defined(ESP32) || defined(ESP8266)
		return ESP.getFlashChipSize();
	#elif defined(AVR)
		return 0;
	#else
		return 0;
	#endif
}

unsigned int ProtonixMemory::FlashBufferSize() {
	return this->_flashBufferSize;
}

unsigned int ProtonixMemory::FlashFirmwareSize() {
	#if defined(ESP32) || defined(ESP8266)
		return ESP.getSketchSize();
	#elif defined(AVR)
		return 0;
	#else
		return 0;
	#endif
}

#if defined(ESP8266)
unsigned int ProtonixMemory::FlashSectorSize(size_t size) {
	return (size + FLASH_SECTOR_SIZE - 1) & (~(FLASH_SECTOR_SIZE - 1));
}

unsigned int ProtonixMemory::FlashBufferSizeExpected() {
    const unsigned int sector = FLASH_SECTOR_SIZE * 2;

    return ProtonixMemory::_ramFree() > sector ? FLASH_SECTOR_SIZE : PROTONIX_MEMORY_BUFFER_SIZE;
}


void ProtonixMemory::EBootCommand(uint32_t addressStart, uint32_t addressEnd, size_t size) {
    eboot_command ebcmd;

    eboot_command_read(&ebcmd);

    ebcmd.action = ACTION_COPY_RAW;
    ebcmd.args[0] = addressStart;
    ebcmd.args[1] = addressEnd;
    ebcmd.args[2] = size;
    //ebcmd.args[21] = addressStart;

    eboot_command_write(&ebcmd);
}

void ProtonixMemory::EBootCommandRead() {
    eboot_command ebcmd;

    if (!eboot_command_read(&ebcmd)) Serial.println("[WARNING] Can not read EBoot");

    Serial.println("[eboot] action: " + String(ebcmd.action));
    int i = 0;
    while (i < 29) {
    	Serial.println("[eboot] arg " + String(i) + ": " + String(ebcmd.args[i]));

    	i++;
    }
}
#endif