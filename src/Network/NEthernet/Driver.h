#pragma once

#include <Arduino.h>

#include <Ethernet.h>

#include "../../IProtonixNetworkDriver.h"
#include "../../IProtonixNetworkClient.h"

namespace Qybercom {
	namespace Protonix {
		namespace Network {
			namespace NEthernet {
				class Driver : public IProtonixNetworkDriver {
					private:
						short _pinSS;
						EthernetLinkStatus _status;

						String _statusRecognize (EthernetLinkStatus code);

					public:
						Driver (unsigned short pinSS, bool connectAuto = true, unsigned int timerConnect = 1000);
						Driver (String mac, bool connectAuto = true, unsigned int timerConnect = 1000);
						Driver (unsigned short pinSS, String mac, bool connectAuto = true, unsigned int timerConnect = 1000);

						short PinSS ();
						Driver* PinSS (unsigned short pinSS);

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