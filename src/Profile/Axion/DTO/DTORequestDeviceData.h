#pragma once

#include <Arduino.h>
//#include <ArduinoJson.h>

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
							bool _first;
							bool _includeMemory;
							bool _includeHardware;

						public:
							DTORequestDeviceData (Protonix* device, bool includeMemory = false, bool includeHardware = false, bool first = true);

							void AxionDTOPopulate (AxionDTO* dto);
							//void AxionDTOSerialize (JsonDocument& dto);
					};
				}
			}
		}
	}
}