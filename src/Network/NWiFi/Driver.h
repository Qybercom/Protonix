#pragma once

#include <Arduino.h>

#include "../../IProtonixNetworkDriver.h"
#include "../../IProtonixNetworkClient.h"

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
						Driver (String ssid, String password, String mac, String hostname, bool connectAuto = true, unsigned int timerCheck = 1000);

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