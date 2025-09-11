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
					String _registry;
					bool _trailing;

				public:
					DTORequestDeviceStatus ();
					DTORequestDeviceStatus (ProtonixDeviceStatus* status, String registry);

					void Status (ProtonixDeviceStatus* status);
					ProtonixDeviceStatus* Status ();

					void Registry (String registry);
					String Registry ();

					void DTOPopulate (ProtonixDTO* dto);
					void DTOSerialize (JsonDocument& dto);
					String DTOSerializeFilter (String raw);

					DTORequestDeviceStatus* Reset (ProtonixDeviceStatus* status);
			};
		}
	}
}