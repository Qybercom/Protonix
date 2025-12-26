#pragma once

#include <Arduino.h>

#include "../IProtonixCommand.h"
#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Command {
			class CStdFirmware : public IProtonixCommand {
				private:
					String _action;
					String _version;

				public:
					CStdFirmware ();

					CStdFirmware* Action (String action);
					String Action ();

					bool ActionUpdate ();

					CStdFirmware* Version (String version);
					String Version ();

					bool VersionLatest ();

					bool CommandRecognize (Protonix* device, String command, IProtonixHardware* hardware);
					bool CommandSerialize ();
			};
		}
	}
}