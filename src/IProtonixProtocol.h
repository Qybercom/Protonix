#pragma once

#include <Arduino.h>

#include "ProtonixURI.h"
#include "ProtonixDevice.h"

namespace Qybercom {
	namespace Protonix {
		class ProtonixDevice;

		class IProtonixProtocol {
			public:
				virtual void ProtocolInit (ProtonixDevice* device);
				virtual bool ProtocolConnect (ProtonixURI* uri);
				virtual bool ProtocolConnected ();
				virtual void ProtocolPipe ();
				virtual void ProtocolSend (String data);
		};
	}
}