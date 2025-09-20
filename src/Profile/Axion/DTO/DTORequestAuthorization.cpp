#include <Arduino.h>
#include <ArduinoJson.h>

#include "../AxionDTO.h"

#include "DTORequestAuthorization.h"

using namespace Qybercom::Protonix;

Profile::Axion::DTO::DTORequestAuthorization::DTORequestAuthorization () { }

Profile::Axion::DTO::DTORequestAuthorization::DTORequestAuthorization (String id, String passphrase) {
	this->ID(id);
	this->Passphrase(passphrase);
}

void Profile::Axion::DTO::DTORequestAuthorization::ID (String id) {
	this->_id = id;
}

String Profile::Axion::DTO::DTORequestAuthorization::ID () {
	return this->_id;
}

void Profile::Axion::DTO::DTORequestAuthorization::Passphrase (String passphrase) {
	this->_passphrase = passphrase;
}

String Profile::Axion::DTO::DTORequestAuthorization::Passphrase () {
	return this->_passphrase;
}

void Profile::Axion::DTO::DTORequestAuthorization::AxionDTOPopulate (Profile::Axion::AxionDTO* dto) {
	JsonObject data = dto->Data();

	if (data["id"].is<const char*>())
		this->ID(data["id"]);

	if (data["passphrase"].is<const char*>())
		this->Passphrase(data["passphrase"]);
}

void Profile::Axion::DTO::DTORequestAuthorization::AxionDTOSerialize (JsonDocument& dto) {
	dto["data"]["id"] = this->_id;
	dto["data"]["passphrase"] = this->_passphrase;
}