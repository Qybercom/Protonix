#pragma once

#include <Arduino.h>
#include "Client.h"

#if defined(ESP32)
#include <WiFi.h>
#include <esp_wifi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#endif

#include "../../IProtonixNetworkClient.h"
#include "../../ProtonixURI.h"

namespace Qybercom {
	namespace Protonix {
		namespace Network {
			namespace NWiFi {
				class Client : public IProtonixNetworkClient {
					private:
						#if defined(ESP32) || defined(ESP8266)
							WiFiClient* _client;
							unsigned char _buffer[2048];
						#else
							::Client* _client;
							unsigned char _buffer[0];
						#endif
						unsigned char* _bufferPTR;

					public:
						Client ();

						::Client* NetworkClientClient ();
						bool NetworkClientConnect (ProtonixURI* uri);
						bool NetworkClientConnected ();
						String NetworkClientReceive ();
						bool NetworkClientSend (String data);
						bool NetworkClientClose ();

						~Client ();
				};
			}
		}
	}
}