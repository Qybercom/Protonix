#pragma once

#include <Arduino.h>

#include "IProtonixNetworkDriver.h"
#include "IProtonixNetworkClient.h"
#include "ProtonixURI.h"

namespace Qybercom {
	namespace Protonix {
		class ProtonixNetworkClient {
			private:
				ProtonixURI* _uri;
				bool _uriOwn;
				IProtonixNetworkDriver* _network;
				IProtonixNetworkClient* _client;

				bool _valid ();

			public:
				ProtonixNetworkClient (String uri);
				ProtonixNetworkClient (ProtonixURI* uri);

				ProtonixURI* URI ();
				ProtonixNetworkClient* URI (ProtonixURI* uri);

				IProtonixNetworkDriver* Network ();
				bool Network (IProtonixNetworkDriver* network);

				IProtonixNetworkClient* Client ();
				ProtonixNetworkClient* Client (IProtonixNetworkClient* client);

				bool Connect ();
				bool Connected ();
				String Receive ();
				bool Send (String data);
				bool Close ();

				~ProtonixNetworkClient ();
		};
	}
}