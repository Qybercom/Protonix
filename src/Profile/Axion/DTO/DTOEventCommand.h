#pragma once

#include <Arduino.h>
//#include <ArduinoJson.h>

#include "../IAxionDTO.h"
#include "../AxionDTO.h"

namespace Qybercom {
	namespace Protonix {
		namespace Profile {
			namespace Axion {
				namespace DTO {
					class DTOEventCommand : public IAxionDTOEvent {
						private:
							String _command;

						public:
							void Command (String command);
							String Command ();

							void AxionDTOPopulate (AxionDTO* dto);
							//void AxionDTOSerialize (JsonDocument& dto);
					};
				}
			}
		}
	}
}