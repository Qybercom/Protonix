#include <Arduino.h>

#include "ProtonixRegistry.h"
#include "ProtonixMemory.h"

//#include "Command/CStdRegistry.h"

using namespace Qybercom::Protonix;

ProtonixRegistry::ProtonixRegistry(ProtonixMemory* memory) {
	this->_memory = memory;
	this->_bufferRaw = "";
	this->_bufferLoaded = false;
	this->_debug = false;
}

ProtonixRegistry* ProtonixRegistry::Debug(bool debug) {
	this->_debug = debug;

	return this;
}

bool ProtonixRegistry::Debug() {
	return this->_debug;
}

bool ProtonixRegistry::_bufferLoad() {
	if (this->_bufferLoaded) {
		if (this->_debug)
			Serial.println("[debug] eeprom loaded: `" + this->_bufferRaw + "`");

		return true;
	}

	const unsigned int size = PROTONIX_MEMORY_EEPROM_SIZE - PROTONIX_REGISTRY_START;

	char raw[size];
	this->_memory->EEPROMGet(PROTONIX_REGISTRY_START, raw);
	this->_bufferRaw = String(raw);
	this->_bufferRaw.trim();

	if (this->_debug)
		Serial.println("[debug] eeprom ready: `" + this->_bufferRaw + "`");

	DeserializationError err = deserializeJson(this->_buffer, this->_bufferRaw);

	if (err) {
		Serial.println("[WARNING] ProtonixRegistry: json deserialize error: " + String(err.f_str()));

		this->_bufferRaw = "{}";
		deserializeJson(this->_buffer, this->_bufferRaw);
	}

	this->_bufferObj = this->_buffer.as<JsonObject>();
	this->_bufferLoaded = true;

	return true;
}

String ProtonixRegistry::_tuple(String key, bool min) {
	String value = this->GetRaw(key, "");
	String out = "";
	bool delimeter = false;
	unsigned short i = 0;
	unsigned short length = value.length();

	while (i < length) {
		if (value[i] == '/') {
			delimeter = true;
		}
		else {
			if ((min && !delimeter) || (!min && delimeter)) {
				out += value[i];
			}
		}

		i++;
	}

	return out;
}

/*
template<typename T>
T ProtonixRegistry::Get(String key, T defaultValue) {
	if (!this->_bufferLoad()) return nullptr;

	T value = this->_bufferObj[key].as<T>();

	return value == nullptr ? defaultValue : value;
}
*/

/*template<typename T>
bool ProtonixRegistry::Set(String key, T value) {
	return this->Set(key, value, false);
}

template<typename T>
bool ProtonixRegistry::Set(String key, T value, bool commit) {
	if (!this->_bufferLoad()) return false;

	this->_bufferObj[key] = value;

	return commit ? this->Commit() : true;
}*/
String ProtonixRegistry::Raw() {
	return this->_bufferRaw;
}

String ProtonixRegistry::GetRaw(String key, String defaultValue) {
	if (!this->_bufferLoad()) return defaultValue;

	String value = this->_bufferObj[key].as<String>();

	// value == ""
	return this->_bufferObj[key] == nullptr ? defaultValue : value;
}

bool ProtonixRegistry::SetRaw(String key, String value, bool commit) {
	if (!this->_bufferLoad()) return false;

	this->_bufferObj[key] = value;

	return commit ? this->Commit() : true;
}

/*
bool ProtonixRegistry::Set(Command::CStdRegistry* cmd, bool commit) {
	return cmd != nullptr && this->Set(cmd->Key(), cmd->Value(), commit);
}
*/

int ProtonixRegistry::GetIntervalMin(String key, int defaultValue) {
	String val = this->_tuple(key, true);

	return val == "" ? defaultValue : val.toInt();
}

int ProtonixRegistry::GetIntervalMax(String key, int defaultValue) {
	String val = this->_tuple(key, false);

	return val == "" ? defaultValue : val.toInt();
}

float ProtonixRegistry::GetIntervalMin_f(String key, float defaultValue) {
	String val = this->_tuple(key, true);

	return val == "" ? defaultValue : val.toFloat();
}

float ProtonixRegistry::GetIntervalMax_f(String key, float defaultValue) {
	String val = this->_tuple(key, false);

	return val == "" ? defaultValue : val.toFloat();
}

bool ProtonixRegistry::Commit() {
	if (serializeJson(this->_buffer, this->_bufferRaw) == 0) {
		Serial.println("[WARNING] ProtonixRegistry: json serialize error");

		return false;
	}

	const unsigned int size = PROTONIX_MEMORY_EEPROM_SIZE - PROTONIX_REGISTRY_START;
	unsigned int sizeActual = this->_bufferRaw.length();
	char raw[size];
	unsigned int i = 0;

	while (i < size) {
		raw[i] = i < sizeActual ? this->_bufferRaw[i] : ' ';

		i++;
	}

	this->_memory->EEPROMSet(PROTONIX_REGISTRY_START, raw);

	return this->_memory->EEPROMCommit();
}