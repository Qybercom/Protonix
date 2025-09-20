#pragma once

#include <Arduino.h>

#include "ProtonixURI.h"

namespace Qybercom {
	namespace Protonix {
		class IProtonixNetworkClient {
			public:
				virtual bool NetworkClientConnect (ProtonixURI* uri) = 0;
				virtual bool NetworkClientConnected () = 0;
				virtual String NetworkClientReceive () = 0;
				virtual bool NetworkClientSend (String data) = 0;
				virtual bool NetworkClientClose () = 0;

				virtual ~IProtonixNetworkClient () { };
		};
	}
}