#include <Arduino.h>

#include "_platforms.h"

#include "Common/index.h"

#include "Protonix.h"
#include "ProtonixRegistry.h"
#include "ProtonixMemory.h"

using namespace Qybercom::Protonix;

ProtonixRegistry::ProtonixRegistry (ProtonixMemory* memory) {
	this->_memory = memory;
	this->_format = nullptr;
	this->_raw = "";
	this->_loaded = false;
	this->_debug = false;
}

Qybercom::Value &ProtonixRegistry::Data () {
	return this->_data;
}

ProtonixRegistry* ProtonixRegistry::Debug (bool debug) {
	this->_debug = debug;

	return this;
}

bool ProtonixRegistry::Debug () {
	return this->_debug;
}

bool ProtonixRegistry::Load () {
	if (this->_debug) {
		Serial.print("[registry:eeprom.0]"); Serial.println(ESP.getFreeHeap());
	}

	if (this->_loaded) {
		if (this->_debug)
			Serial.println("[registry:eeprom] Loaded: `" + this->_raw + "`");

		return true;
	}

	if (this->_debug) {
		Serial.print("[registry:eeprom.1]"); Serial.println(ESP.getFreeHeap());
	}

	this->_format = Protonix::Instance()->Format("application/json");

	const unsigned int size = QYBERCOM_PROTONIX_MEMORY_EEPROM_SIZE - QYBERCOM_PROTONIX_REGISTRY_START;
	char raw[size];

	this->_memory->EEPROMGet(QYBERCOM_PROTONIX_REGISTRY_START, raw);

	this->_raw = String(raw);
	this->_raw.trim();

	if (this->_debug)
		Serial.println("[registry:eeprom] Ready: `" + this->_raw + "`");

	this->_data = Qybercom::Value::Deserialize(this->_format, this->_raw);
	this->_loaded = true;

	if (this->_debug) {
		Serial.print("[registry:eeprom.2]"); Serial.println(ESP.getFreeHeap());

		this->_data.Dump();
	}

	return true;
}

String &ProtonixRegistry::Raw () {
	return this->_raw;
}

bool ProtonixRegistry::Commit () {
	this->_raw = this->_data.Serialize(this->_format);

	const unsigned int size = QYBERCOM_PROTONIX_MEMORY_EEPROM_SIZE - QYBERCOM_PROTONIX_REGISTRY_START;
	unsigned int sizeActual = this->_raw.length();
	char raw[size];
	unsigned int i = 0;

	while (i < size) {
		raw[i] = i < sizeActual ? this->_raw[i] : ' ';

		i++;
	}

	this->_memory->EEPROMSet(QYBERCOM_PROTONIX_REGISTRY_START, raw);

	return this->_memory->EEPROMCommit();
}