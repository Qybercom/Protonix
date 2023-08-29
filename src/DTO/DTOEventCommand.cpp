#include <Arduino.h>
#include <ArduinoJson.h>

#include "../ProtonixDTO.h"
#include "DTOEventCommand.h"

using namespace Qybercom::Protonix;



void DTO::DTOEventCommand::Name(String name) {
	this->_name = name;
}

String DTO::DTOEventCommand::Name() {
	return this->_name;
}

void DTO::DTOEventCommand::DTOPopulate(ProtonixDTO* dto) {
	JsonObject data = dto->Data();

	if (data.containsKey("command"))
		this->Name(data["command"]);
}

void DTO::DTOEventCommand::DTOSerialize(JsonDocument& dto) {
	dto["data"]["command"] = this->_name;
}