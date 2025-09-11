#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include "../IProtonixDTO.h"
#include "../ProtonixDTO.h"

namespace Qybercom {
	namespace Protonix {
		namespace DTO {
			class DTOEventCommand : public IProtonixDTOEvent {
				private:
					String _name;

				public:
					void Name (String name);
					String Name ();

					void DTOPopulate (ProtonixDTO* dto);
					void DTOSerialize (JsonDocument& dto);
					String DTOSerializeFilter (String raw);
			};
		}
	}
}