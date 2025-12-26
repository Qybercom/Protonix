#pragma once

#include <Arduino.h>

#include "../IProtonixCommand.h"
#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Command {
			class CStdAction : public IProtonixCommand {
				private:
					String _cmd;
					String _name;

				public:
					CStdAction ();

					CStdAction* CMD (String cmd);
					String CMD ();

					CStdAction* Name (String name);
					String Name ();

					bool CommandRecognize (Protonix* device, String command, IProtonixHardware* hardware);
					bool CommandSerialize ();
			};
		}
	}
}