#include <Arduino.h>
//#include <ArduinoJson.h>

#include "../AxionDTO.h"
#include "DTOResponseDeviceData.h"

using namespace Qybercom::Protonix;

void Profile::Axion::DTO::DTOResponseDeviceData::AxionDTOPopulate (Profile::Axion::AxionDTO* dto) {
	/*JsonObject data = dto->Data();

	if (data["status"].is<unsigned short>())
		this->AxionDTOResponseStatus(data["status"]);*/
}

/*void Profile::Axion::DTO::DTOResponseDeviceData::AxionDTOSerialize (JsonDocument& dto) {
	dto["data"]["status"] = this->_status;
}*/