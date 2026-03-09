#pragma once

#include <Arduino.h>

#include "../../IProtonixNetworkDriver.h"
#include "../../IProtonixNetworkClient.h"

#if defined(ESP32)
	#define QYBERCOM_PROTONIX_NETWORK_WIFI_TIMEOUT_CONNECT 7000
#elif defined(ESP8266)
	#define QYBERCOM_PROTONIX_NETWORK_WIFI_TIMEOUT_CONNECT 10000
#else
	#define QYBERCOM_PROTONIX_NETWORK_WIFI_TIMEOUT_CONNECT 1000
#endif

namespace Qybercom {
	namespace Protonix {
		namespace Network {
			namespace NWiFi {
				class Driver : public IProtonixNetworkDriver {
					private:
						String _ssid;
						String _password;
						int _status;

						String _statusRecognize (int code);

					public:
						Driver (String ssid, String password, String mac, String hostname, bool connectAuto = true, unsigned int timerCheck = QYBERCOM_PROTONIX_NETWORK_WIFI_TIMEOUT_CONNECT);

						bool NetworkDriverConnect ();
						bool NetworkDriverConnected ();
						bool NetworkDriverDisconnect ();
						String NetworkDriverAddressMAC ();
						String NetworkDriverAddressIP ();
						IProtonixNetworkClient* NetworkDriverAllocateClient ();
				};
			}
		}
	}
}