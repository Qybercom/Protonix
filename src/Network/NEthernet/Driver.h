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
						Driver (unsigned short pinSS);
						Driver (String mac);
						Driver (unsigned short pinSS, String mac);

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