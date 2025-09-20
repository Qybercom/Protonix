#pragma once

#include <Arduino.h>

#include "../IProtonixCommand.h"
#include "../IProtonixHardware.h"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Command {
			class CStdActive : public IProtonixCommand {
				private:
					bool _active;

				public:
					CStdActive ();
					CStdActive (bool active);

					void Active (bool active);
					bool Active ();

					bool CommandRecognize (Protonix* device, String command, IProtonixHardware* hardware);
					bool CommandSerialize ();
			};
		}
	}
}