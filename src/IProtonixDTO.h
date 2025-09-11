#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include "ProtonixDTO.h"

namespace Qybercom {
	namespace Protonix {
		class ProtonixDTO;

		class IProtonixDTO {
			public:
				virtual void DTOPopulate (ProtonixDTO* dto);
				virtual void DTOSerialize (JsonDocument& dto);
				virtual String DTOSerializeFilter (String raw);
				virtual ~IProtonixDTO () {};
		};

		class IProtonixDTORequest : public IProtonixDTO {
		};

		class IProtonixDTOResponse : public IProtonixDTO {
			public:
				virtual unsigned short DTOResponseStatus();
		};

		class IProtonixDTOEvent : public IProtonixDTO {
		};
	}
}