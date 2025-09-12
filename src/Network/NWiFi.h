#pragma once

#include <Arduino.h>

#include "../IProtonixNetwork.h"
#include "../ProtonixTimer.h"

namespace Qybercom {
	namespace Protonix {
		namespace Network {
			class NWiFi : public IProtonixNetwork {
				private:
					String _ssid;
					String _password;
					String _mac;
					uint8_t _macBuffer[6];
					String _hostname;
					//ProtonixTimer* _timerCheck;

					String _status(int code);

				public:
					NWiFi (String ssid, String password, String mac, String hostname);//, unsigned int timerCheck = 1000);

					//ProtonixTimer* TimerCheck ();

					bool NetworkConnect ();
					bool NetworkConnected ();
					bool NetworkDisconnect ();
					String NetworkAddressMAC ();
					String NetworkAddressIP ();
			};
		}
	}
}