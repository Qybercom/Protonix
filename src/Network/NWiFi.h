#pragma once

#include <Arduino.h>

#include "../IProtonixNetwork.h"

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

				public:
					NWiFi(String ssid, String password, String mac, String hostname);
					bool Connect();
					bool Connected();
					bool Disconnect();
					String AddressMAC();
					String AddressIP();
			};
		}
	}
}