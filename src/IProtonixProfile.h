#pragma once

#include <Arduino.h>

namespace Qybercom {
	namespace Protonix {
		class Protonix;

		class IProtonixProfile {
			public:
				virtual void ProfilePipe (Protonix* device) = 0;
				virtual String ProfileFirmwareURI (Protonix* device, String platform, String version) = 0;
		};
	}
}