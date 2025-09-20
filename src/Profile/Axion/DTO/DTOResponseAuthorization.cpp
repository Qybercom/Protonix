#include <Arduino.h>
#include <ArduinoJson.h>

#include "../AxionDTO.h"
#include "DTOResponseAuthorization.h"

using namespace Qybercom::Protonix;

void Profile::Axion::DTO::DTOResponseAuthorization::AxionDTOPopulate (Profile::Axion::AxionDTO* dto) {
	JsonObject data = dto->Data();

	if (data["status"].is<unsigned short>())
		this->AxionDTOResponseStatus(data["status"]);
}

void Profile::Axion::DTO::DTOResponseAuthorization::AxionDTOSerialize (JsonDocument& dto) {
	dto["data"]["status"] = this->_status;
}