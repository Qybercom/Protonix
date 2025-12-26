#pragma once

#include <Arduino.h>

#include "../IProtonixCommand.h"
#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Command {
			class CStdRegistry : public IProtonixCommand {
				private:
					String _key;
					String _value;

				public:
					CStdRegistry ();

					CStdRegistry* Key (String key);
					String Key ();

					CStdRegistry* Value (String value);
					String Value ();

					bool CommandRecognize (Protonix* device, String command, IProtonixHardware* hardware);
					bool CommandSerialize ();
			};
		}
	}
}