#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include "../IProtonixDTO.h"
#include "../ProtonixDTO.h"
#include "../ProtonixDeviceStatus.h"

namespace Qybercom {
	namespace Protonix {
		namespace DTO {
			class DTORequestDeviceStatus : public IProtonixDTORequest {
				private:
					ProtonixDeviceStatus* _status;

				public:
					DTORequestDeviceStatus();
					DTORequestDeviceStatus(ProtonixDeviceStatus* status);

					void Status(ProtonixDeviceStatus* status);
					ProtonixDeviceStatus* Status();

					void DTOPopulate(ProtonixDTO* dto);
					void DTOSerialize(JsonDocument& dto);

					DTORequestDeviceStatus* Reset(ProtonixDeviceStatus* status);
			};
		}
	}
}