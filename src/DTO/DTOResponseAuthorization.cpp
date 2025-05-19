#include <Arduino.h>
#include <ArduinoJson.h>

#include "../ProtonixDTO.h"
#include "DTOResponseAuthorization.h"

using namespace Qybercom::Protonix;



void DTO::DTOResponseAuthorization::Status(unsigned short status) {
	this->_status = status;
}

unsigned short DTO::DTOResponseAuthorization::Status() {
	return this->_status;
}

void DTO::DTOResponseAuthorization::DTOPopulate(ProtonixDTO* dto) {
	JsonObject data = dto->Data();

	//if (data.containsKey("status"))
	if (data["status"].is<unsigned short>())
		this->Status(data["status"]);
}

void DTO::DTOResponseAuthorization::DTOSerialize(JsonDocument& dto) {
	dto["data"]["status"] = this->_status;
}

String DTO::DTOResponseAuthorization::DTOSerializeFilter(String raw) {
	return raw;
}

unsigned short DTO::DTOResponseAuthorization::DTOResponseStatus() {
	return this->_status;
}