#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include "../IProtonixDTO.h"
#include "../ProtonixDTO.h"

namespace Qybercom {
	namespace Protonix {
		namespace DTO {
			class DTORequestAuthorization : public IProtonixDTORequest {
				private:
					String _id;
					String _passphrase;

				public:
					DTORequestAuthorization ();
					DTORequestAuthorization (String id, String passphrase);

					void ID (String id);
					String ID ();

					void Passphrase (String passphrase);
					String Passphrase ();

					void DTOPopulate (ProtonixDTO* dto);
					void DTOSerialize (JsonDocument& dto);
					String DTOSerializeFilter (String raw);

					DTORequestAuthorization* Reset (String id, String passphrase);
			};
		}
	}
}