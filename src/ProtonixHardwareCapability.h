#pragma once

#include <Arduino.h>

#include "Common/index.h"

namespace Qybercom {
	namespace Protonix {
		class ProtonixHardwareCapability {
			private:
				String _kind;
				String _id;
				String _value;
				String _comment;

			public:
				ProtonixHardwareCapability (String kind = "", String id = "", String comment = "");

				String Kind ();
				ProtonixHardwareCapability* Kind (String kind);

				String ID ();
				ProtonixHardwareCapability* ID (String id);

				String Value ();
				ProtonixHardwareCapability* Value (String value);

				String Comment ();
				ProtonixHardwareCapability* Comment (String comment);

				Qybercom::Value DTO (bool first);
		};
	}
}