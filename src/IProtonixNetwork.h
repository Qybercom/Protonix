#pragma once

#include <Arduino.h>

namespace Qybercom {
	namespace Protonix {
		class ProtonixDevice;

		class IProtonixNetwork {
			public:
				virtual bool Connect();
				virtual bool Connected();
				virtual bool Disconnect();
				virtual String AddressMAC();
				virtual String AddressIP();

				// https://stackoverflow.com/a/12772708/2097055
				// https://stackoverflow.com/a/1755042/2097055
				static void ParseMAC(String mac, uint8_t out[6]) {
					char buffer[18];
					mac.toCharArray(buffer, 18);

					sscanf(buffer, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &out[0], &out[1], &out[2], &out[3], &out[4], &out[5]);
				}
		};
	}
}