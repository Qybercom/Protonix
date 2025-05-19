#include <Arduino.h>

#include "ProtonixRegistry.h"
#include "ProtonixMemory.h"

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

	String rawS = String(raw);
	rawS.trim();

	if (this->_debug)
		Serial.println("[debug] eeprom ready: `" + rawS + "`");

	DeserializationError err = deserializeJson(this->_buffer, rawS);

	if (err) {
		Serial.println("[WARNING] ProtonixRegistry: json deserialize error: " + String(err.f_str()));

		this->_bufferRaw = "{}";
		deserializeJson(this->_buffer, this->_bufferRaw);
	}
	else {
		if (serializeJson(this->_buffer, this->_bufferRaw) == 0) {
			Serial.println("[WARNING] ProtonixRegistry: json serialize error");

			return false;
		}
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

String ProtonixRegistry::Raw() {
	return this->_bufferRaw;
}

String ProtonixRegistry::GetRaw(String key, String defaultValue) {
	if (!this->_bufferLoad()) return defaultValue;

	String value = this->_bufferObj[key].as<String>();

	return this->_bufferObj[key] == nullptr ? defaultValue : value;
}

bool ProtonixRegistry::SetRaw(String key, String value, bool commit) {
	if (!this->_bufferLoad()) return false;

	this->_bufferObj[key] = value;

	return commit ? this->Commit() : true;
}

int ProtonixRegistry::GetInt(String key, int defaultValue) {
	return this->_bufferLoad() ? this->GetRaw(key, String(defaultValue)).toInt() : defaultValue;
}

bool ProtonixRegistry::SetInt(String key, int value, bool commit) {
	return this->SetRaw(key, String(value), commit);
}

float ProtonixRegistry::GetFloat(String key, float defaultValue) {
	return this->_bufferLoad() ? this->GetRaw(key, String(defaultValue)).toFloat() : defaultValue;
}

bool ProtonixRegistry::SetFloat(String key, float value, bool commit) {
	return this->SetRaw(key, String(value), commit);
}

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

ProtonixRegistryColor ProtonixRegistry::GetColor(String key, String defaultValue) {
	ProtonixRegistryColor color;
	String raw = this->_bufferLoad() ? this->GetRaw(key, String(defaultValue)) : defaultValue;

	int i = 0;
	int length = raw.length();
	String clr[3] = {"", "", ""};
	int j = 0;
	char c;

	while (i < length) {
		c = raw.charAt(i);

		if (c == ',') j++;
		else clr[j] += c;

		i++;
	}

	color.r = clr[0].toInt();
	color.g = clr[1].toInt();
	color.b = clr[2].toInt();

	return color;
}

bool ProtonixRegistry::SetColor(String key, String value, bool commit) {
	return this->SetRaw(key, String(value), commit);
}

bool ProtonixRegistry::SetColor(String key, ProtonixRegistryColor &color, bool commit) {
	return this->SetRaw(key, String(String(color.r) + String(',') + String(color.g) + String(',') + String(color.b)), commit);
}

ProtonixRegistryList ProtonixRegistry::GetList(String key, String defaultValue) {
	ProtonixRegistryList list;
	String raw = this->_bufferLoad() ? this->GetRaw(key, String(defaultValue)) : defaultValue;

	int i = 0;
	int length = raw.length();
	int j = 0;
	char c;

	while (i < length && j < 20) {
		c = raw.charAt(i);

		if (c == ';') j++;
		else list.items[j] += c;

		i++;
	}

	j++;

	while (j < 20) {
		list.items[j] = "";

		j++;
	}

	return list;
}

bool ProtonixRegistry::GetListItemExists(String key, String value) {
	ProtonixRegistryList list = this->GetList(key, "");

	unsigned short i = 0;

	while (i < 20) {
		if (list.items[i] == value) return true;

		i++;
	}

	return false;
}

bool ProtonixRegistry::SetList(String key, String value, bool commit) {
	return this->SetRaw(key, String(value), commit);
}

bool ProtonixRegistry::SetList(String key, ProtonixRegistryList &list, bool commit) {
	String raw = "";
	int i = 0;
	bool begin = true;

	while (i < 20) {
		if (!begin) raw += ";";
		begin = true;

		raw += list.items[i];

		i++;
	}

	return this->SetRaw(key, String(raw), commit);
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