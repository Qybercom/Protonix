#pragma once

#include <Arduino.h>

#include <Ethernet.h>

#include "../../IProtonixNetworkClient.h"
#include "../../ProtonixURI.h"

namespace Qybercom {
	namespace Protonix {
		namespace Network {
			namespace NEthernet {
				class Client : public IProtonixNetworkClient {
					private:
						EthernetClient _client;
						unsigned char _buffer[2048];
						unsigned char* _bufferPTR;

					public:
						Client ();

						bool NetworkClientConnect (ProtonixURI* uri);
						bool NetworkClientConnected ();
						String NetworkClientReceive ();
						bool NetworkClientSend (String data);
						bool NetworkClientClose ();
				};
			}
		}
	}
}