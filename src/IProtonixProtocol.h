#pragma once

#include <Arduino.h>

#include "ProtonixURI.h"
#include "ProtonixDevice.h"

namespace Qybercom {
	namespace Protonix {
		class ProtonixDevice;

		class IProtonixProtocol {
			public:
				virtual void Init(ProtonixDevice* device);
				virtual bool Connect(ProtonixURI* uri);
				virtual bool Connected();
				virtual void Pipe();
				virtual void Send(String data);
		};
	}
}