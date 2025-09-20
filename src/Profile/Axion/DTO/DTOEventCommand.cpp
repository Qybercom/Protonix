#include <Arduino.h>
#include <ArduinoJson.h>

#include "../AxionDTO.h"
#include "DTOEventCommand.h"

using namespace Qybercom::Protonix;

void Profile::Axion::DTO::DTOEventCommand::Command (String command) {
	this->_command = command;
}

String Profile::Axion::DTO::DTOEventCommand::Command () {
	return this->_command;
}

void Profile::Axion::DTO::DTOEventCommand::AxionDTOPopulate (Profile::Axion::AxionDTO* dto) {
	JsonObject data = dto->Data();

	if (data["command"].is<const char*>())
		this->Command(data["command"]);
}

void Profile::Axion::DTO::DTOEventCommand::AxionDTOSerialize (JsonDocument& dto) {
	dto["data"]["command"] = this->_command;
}