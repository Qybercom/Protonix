#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include "../IAxionDTO.h"
#include "../AxionDTO.h"

namespace Qybercom {
	namespace Protonix {
		class Protonix;

		namespace Profile {
			namespace Axion {
				namespace DTO {
					class DTORequestDeviceData : public IAxionDTORequest {
						private:
							Protonix* _device;

						public:
							DTORequestDeviceData (Protonix* device);

							void AxionDTOPopulate (AxionDTO* dto);
							void AxionDTOSerialize (JsonDocument& dto);
					};
				}
			}
		}
	}
}