#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include "IAxionDTO.h"

namespace Qybercom {
	namespace Protonix {
		namespace Profile {
			namespace Axion {
				class AxionDTO {
					private:
						String _url;
						String _response;
						String _event;
						IAxionDTO* _dto;
						JsonObject _data;
						JsonDocument _buffer;
						bool _bufferInit;
						String _bufferRaw;
						JsonObject _bufferObj;

					public:
						AxionDTO ();

						void URL (String url);
						String URL ();

						void Response (String url);
						String Response ();

						void Event (String url);
						String Event ();

						void DTO (IAxionDTO* dto);
						IAxionDTO* DTO ();

						void Data (JsonObject data);
						JsonObject Data ();

						bool IsURL ();
						bool IsResponse ();
						bool IsEvent ();

						bool Serialize ();
						bool Deserialize ();

						void BufferRaw (String data);
						String BufferRaw ();

						void Reset ();
				};
			}
		}
	}
}