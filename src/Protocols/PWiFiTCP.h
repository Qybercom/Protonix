#pragma once

#include <Arduino.h>

#if defined(ESP32)
#include <WiFi.h>
#include <esp_wifi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#endif

#include "../IProtonixProtocol.h"
#include "../ProtonixURI.h"
#include "../ProtonixDevice.h"

namespace Qybercom {
	namespace Protonix {
		namespace Protocol {
			class PWiFiTCP : public IProtonixProtocol {
				private:
					#if defined(ESP32) || defined(ESP8266)
					WiFiClient _client;
					#endif
					ProtonixDevice* _device;
					unsigned char _buffer[2048];
					unsigned char* _bufferPTR;

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