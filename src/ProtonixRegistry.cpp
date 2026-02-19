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

String &ProtonixRegistry::Raw () {
	return this->_raw;
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

	const unsigned int size = QYBERCOM_PROTONIX_MEMORY_EEPROM_SIZE - QYBERCOM_PROTONIX_REGISTRY_START;
	char raw[size];

	this->_memory->EEPROMGet(QYBERCOM_PROTONIX_REGISTRY_START, raw);
	String out = String(raw);
	out.trim();

	if (this->_debug)
		Serial.println("[registry:eeprom.2] Ready: `" + out + "`");

	return this->Load(out);
}

bool ProtonixRegistry::Load (String raw) {
	this->_format = Protonix::Instance()->Format("application/json");
	raw.replace("\\\"", "\"");

	this->_data = Qybercom::Value::Deserialize(this->_format, raw);
	Qybercom::Value testList = Qybercom::Value::Array();
	testList.Add("first");
	testList.Add("second");
	testList.Add(1);
	testList.Add(2);
	testList.Add(true);
	this->_data["statuette"] = testList;
	this->_raw = this->_data.Serialize(this->_format);
	this->_loaded = true;

	if (true) {//this->_debug) {
		Serial.print("[registry:eeprom] RAM:"); Serial.println(ESP.getFreeHeap());

		this->_data.Dump();
	}

	return true;
}

bool ProtonixRegistry::Save () {
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

bool ProtonixRegistry::Save (String key, const Value &value) {
	this->_data.Set(key, value);

	return this->Save();
}

bool ProtonixRegistry::Clear () {
	this->_data.Flush();

	return this->Save();
}

ProtonixRegistry* ProtonixRegistry::Debug (bool debug) {
	this->_debug = debug;

	return this;
}

bool ProtonixRegistry::Debug () {
	return this->_debug;
}