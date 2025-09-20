#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

namespace Qybercom {
	namespace Protonix {
		namespace Profile {
			namespace Axion {
				class AxionDTO;

				class IAxionDTO {
					public:
						virtual void AxionDTOPopulate (AxionDTO* dto) = 0;
						virtual void AxionDTOSerialize (JsonDocument& dto) = 0;

						virtual ~IAxionDTO () { };
				};

				class IAxionDTORequest : public IAxionDTO {
				};

				class IAxionDTOResponse : public IAxionDTO {
					protected:
						unsigned short _status;

					public:
						void AxionDTOResponseStatus (unsigned short status);
						unsigned short AxionDTOResponseStatus ();
				};

				class IAxionDTOEvent : public IAxionDTO {
				};
			}
		}
	}
}