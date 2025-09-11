#pragma once

#include <Arduino.h>

#if defined(ESP32) || defined(ESP8266)
#include <Client.h>
#endif

namespace Qybercom {
	namespace Protonix {
		/*
		class ProrotnixNetworkClient {

		};

		// TODO: split HTTP into Protocol and Client part
		class ProtonixNetwork {
			public:
				ProtonixNetwork* InterfaceAdd (String name, IProtonixNetwork* interface);
				ProtonixNetwork* InterfaceRemove (String name);

				ProtonixNetwork* ProtocolAdd (String scheme, ProtonixNetworkProtocol* protocol);
				ProtonixNetworkProtocol* Protocol (String scheme);
				ProtonixNetwork* ProtocolRemove (String scheme);

				ProtonixNetworkClient* ClientTCP (String interface);
				ProtonixNetworkClient* ClientTCP (String interface, ProtonixNetworkProtocol* protocol);
				//ProtonixNetworkClient* ClientUDP (String interface);
				//ProtonixNetworkClient* ServerTCP (String interface);
				//ProtonixNetworkClient* ServerUDP (String interface);
		};

		class Protonix {
			public:
				ProtonixMemory* Memory ();

		};
		*/
	}
}