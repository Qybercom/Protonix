#pragma once

#include <Arduino.h>

#if defined(ESP32) || defined(ESP8266)
#include <ArduinoWebsockets.h>
#endif

#include "../IProtonixProtocol.h"
#include "../ProtonixURI.h"
#include "../ProtonixDevice.h"

namespace Qybercom {
	namespace Protonix {
		namespace Protocol {
			class PWebSocket : public IProtonixProtocol {
				private:
					#if defined(ESP32) || defined(ESP8266)
					websockets::WebsocketsClient _client;
					#else
					bool _client;
					#endif
					ProtonixDevice* _device;

				public:
					void Init(ProtonixDevice* device);
					bool Connect(ProtonixURI* uri);
					bool Connected();
					void Pipe();
					void Send(String data);
			};
		}
	}
}