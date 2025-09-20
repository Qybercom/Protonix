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
					class DTORequestAuthorization : public IAxionDTORequest {
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

							void AxionDTOPopulate (AxionDTO* dto);
							void AxionDTOSerialize (JsonDocument& dto);
					};
				}
			}
		}
	}
}