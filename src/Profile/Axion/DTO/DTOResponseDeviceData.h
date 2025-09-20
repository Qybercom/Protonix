#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include "../IAxionDTO.h"
#include "../AxionDTO.h"

namespace Qybercom {
	namespace Protonix {
		namespace Profile {
			namespace Axion {
				namespace DTO {
					class DTOResponseDeviceData : public IAxionDTOResponse {
						public:
							void AxionDTOPopulate (AxionDTO* dto);
							void AxionDTOSerialize (JsonDocument& dto);
					};
				}
			}
		}
	}
}