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
					#endif
					bool _init;
					ProtonixDevice* _device;

				public:
					void ProtocolInit (ProtonixDevice* device);
					bool ProtocolConnect (ProtonixURI* uri);
					bool ProtocolConnected ();
					void ProtocolPipe ();
					void ProtocolSend (String data);
			};
		}
	}
}