#include <Arduino.h>
#include <ArduinoJson.h>

#include "IAxionDTO.h"

#include "AxionDTO.h"

using namespace Qybercom::Protonix;

Profile::Axion::AxionDTO::AxionDTO () {
	this->_url = "";
	this->_response = "";
	this->_event = "";
	this->_dto = nullptr;
	this->_bufferRaw = "";
}

void Profile::Axion::AxionDTO::URL (String url) {
	this->_url = url;
}

String Profile::Axion::AxionDTO::URL () {
	return this->_url;
}

void Profile::Axion::AxionDTO::Response (String url) {
	this->_response = url;
}

String Profile::Axion::AxionDTO::Response () {
	return this->_response;
}

void Profile::Axion::AxionDTO::Event (String url) {
	this->_event = url;
}

String Profile::Axion::AxionDTO::Event () {
	return this->_event;
}

void Profile::Axion::AxionDTO::DTO (Profile::Axion::IAxionDTO* dto) {
	this->_dto = dto;
}

Profile::Axion::IAxionDTO* Profile::Axion::AxionDTO::DTO () {
	return this->_dto;
}

void Profile::Axion::AxionDTO::Data (JsonObject data) {
	this->_data = data;
}

JsonObject Profile::Axion::AxionDTO::Data () {
	return this->_data;
}

bool Profile::Axion::AxionDTO::IsURL () {
	return this->_url.length() != 0;
}

bool Profile::Axion::AxionDTO::IsResponse () {
	return this->_response.length() != 0;
}

bool Profile::Axion::AxionDTO::IsEvent () {
	return this->_event.length() != 0;
}

bool Profile::Axion::AxionDTO::Serialize () {
	if (this->IsURL())
		this->_buffer["url"] = this->_url;

	if (this->IsResponse())
		this->_buffer["response"] = this->_response;

	if (this->IsEvent())
		this->_buffer["event"] = this->_event;

	this->_dto->AxionDTOSerialize(this->_buffer);

	bool ok = serializeJson(this->_buffer, this->_bufferRaw) != 0;

	return ok;
}

bool Profile::Axion::AxionDTO::Deserialize () {
	DeserializationError err = deserializeJson(this->_buffer, this->_bufferRaw);

	if (err) {
		Serial.println("[AxionDTO] JSON deserialize error: " + String(err.f_str()));
		Serial.println(this->_bufferRaw);

		return false;
	}

	this->_bufferObj = this->_buffer.as<JsonObject>();

	const char* u = this->_bufferObj["url"];
	const char* r = this->_bufferObj["response"];
	const char* e = this->_bufferObj["event"];
	JsonObject d = this->_bufferObj["data"];

	if (u) this->_url = (String) u;
	if (r) this->_response = (String) r;
	if (e) this->_event = (String) e;
	if (d) this->_data = d;

	return true;
}

void Profile::Axion::AxionDTO::BufferRaw (String data) {
	this->_bufferRaw = data;
}

String Profile::Axion::AxionDTO::BufferRaw () {
	return this->_bufferRaw;
}

void Profile::Axion::AxionDTO::Reset () {
	this->_url = "";
	this->_response = "";
	this->_event = "";

	this->_buffer.clear();
	this->_bufferRaw = "";
}