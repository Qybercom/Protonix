#include <Arduino.h>
#include <ArduinoJson.h>

#include "../ProtonixDTO.h"
#include "DTORequestAuthorization.h"

using namespace Qybercom::Protonix;



DTO::DTORequestAuthorization::DTORequestAuthorization() { }

DTO::DTORequestAuthorization::DTORequestAuthorization(String id, String passphrase) {
	this->ID(id);
	this->Passphrase(passphrase);
}

void DTO::DTORequestAuthorization::ID(String id) {
	this->_id = id;
}

String DTO::DTORequestAuthorization::ID() {
	return this->_id;
}

void DTO::DTORequestAuthorization::Passphrase(String passphrase) {
	this->_passphrase = passphrase;
}

String DTO::DTORequestAuthorization::Passphrase() {
	return this->_passphrase;
}

void DTO::DTORequestAuthorization::DTOPopulate(ProtonixDTO* dto) {
	JsonObject data = dto->Data();

	//if (data.containsKey("id"))
	if (data["id"].is<const char*>())
		this->ID(data["id"]);

	//if (data.containsKey("passphrase"))
	if (data["passphrase"].is<const char*>())
		this->Passphrase(data["passphrase"]);
}

void DTO::DTORequestAuthorization::DTOSerialize(JsonDocument& dto) {
	dto["data"]["id"] = this->_id;
	dto["data"]["passphrase"] = this->_passphrase;
}

String DTO::DTORequestAuthorization::DTOSerializeFilter(String raw) {
	return raw;
}

DTO::DTORequestAuthorization* DTO::DTORequestAuthorization::Reset(String id, String passphrase) {
	this->ID(id);
	this->Passphrase(passphrase);

	return this;
}