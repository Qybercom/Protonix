#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include "IProtonixDTO.h"

namespace Qybercom {
	namespace Protonix {
		class IProtonixDTO;

		class ProtonixDTO {
			private:
				String _url;
				String _response;
				String _event;
				IProtonixDTO* _dto;
				JsonObject _data;
				bool _debug;
				//StaticJsonDocument<2048> _buffer;
				JsonDocument _buffer;
				bool _bufferInit;
				String _bufferRaw;
				JsonObject _bufferObj;

			public:
				ProtonixDTO ();

				void URL (String url);
				String URL ();

				void Response (String url);
				String Response ();

				void Event (String url);
				String Event ();

				void DTO (IProtonixDTO* dto);
				IProtonixDTO* DTO ();

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

				void Debug (bool debug);
				bool Debug ();
		};
	}
}