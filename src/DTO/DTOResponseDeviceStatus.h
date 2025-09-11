#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include "../IProtonixDTO.h"
#include "../ProtonixDTO.h"

namespace Qybercom {
	namespace Protonix {
		namespace DTO {
			class DTOResponseDeviceStatus : public IProtonixDTOResponse {
				private:
					unsigned short _status;

				public:
					void Status (unsigned short status);
					unsigned short Status ();

					void DTOPopulate (ProtonixDTO* dto);
					void DTOSerialize (JsonDocument& dto);
					String DTOSerializeFilter (String raw);
					unsigned short DTOResponseStatus ();
			};
		}
	}
}