#include <Arduino.h>
#include <ArduinoJson.h>

#include "ProtonixDTO.h"

using namespace Qybercom::Protonix;



ProtonixDTO::ProtonixDTO() {
	this->_debug = false;
	this->_bufferRaw = "";
}

void ProtonixDTO::URL(String url) {
	this->_url = url;
}
String ProtonixDTO::URL() {
	return this->_url;
}

void ProtonixDTO::Response(String url) {
	this->_response = url;
}

String ProtonixDTO::Response() {
	return this->_response;
}

void ProtonixDTO::Event(String url) {
	this->_event = url;
}

String ProtonixDTO::Event() {
	return this->_event;
}

void ProtonixDTO::DTO(IProtonixDTO* dto) {
	this->_dto = dto;
}

IProtonixDTO* ProtonixDTO::DTO() {
	return this->_dto;
}

void ProtonixDTO::Data(JsonObject data) {
	this->_data = data;
}

JsonObject ProtonixDTO::Data() {
	return this->_data;
}

bool ProtonixDTO::IsURL() {
	return this->_url.length() != 0;
}

bool ProtonixDTO::IsResponse() {
	return this->_response.length() != 0;
}

bool ProtonixDTO::IsEvent() {
	return this->_event.length() != 0;
}

bool ProtonixDTO::Serialize() {
	if (this->IsURL())
		this->_buffer["url"] = this->_url;

	if (this->IsResponse())
		this->_buffer["response"] = this->_response;

	if (this->IsEvent())
		this->_buffer["event"] = this->_event;

	this->_dto->DTOSerialize(this->_buffer);

	return serializeJson(this->_buffer, this->_bufferRaw) != 0;
}

bool ProtonixDTO::Deserialize() {
	DeserializationError err = deserializeJson(this->_buffer, this->_bufferRaw);

	if (err) {
		Serial.println("[dto] json deserialize error: " + String(err.f_str()));

		return false;
	}

	this->_bufferObj = this->_buffer.as<JsonObject>();

	if (this->_bufferObj.containsKey("url")) {
		const char* u = this->_bufferObj["url"];
		this->_url = (String)u;
	}

	if (this->_bufferObj.containsKey("response")) {
		const char* r = this->_bufferObj["response"];
		this->_response = (String)r;
	}

	if (this->_bufferObj.containsKey("event")) {
		const char* e = this->_bufferObj["event"];
		this->_event = (String)e;
	}

	if (this->_bufferObj.containsKey("data"))
		this->_data = this->_bufferObj["data"];

	return true;
}

void ProtonixDTO::BufferRaw(String data) {
	this->_bufferRaw = data;
}

void ProtonixDTO::BufferRaw(char* data) {
	this->_bufferRaw = String(data);
}

String ProtonixDTO::BufferRaw() {
	return this->_bufferRaw;
}

void ProtonixDTO::Reset() {
	this->_url = "";
	this->_response = "";
	this->_event = "";

	/*delete this->_data;
	this->_data = nullptr;*/

	this->_buffer.clear();
	this->_bufferRaw = "";
}

void ProtonixDTO::Debug(bool debug) {
	this->_debug = debug;
}

bool ProtonixDTO::Debug() {
	return this->_debug;
}