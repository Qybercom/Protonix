#pragma once

#include <Arduino.h>

#include "../IProtonixCommand.h"
#include "../IProtonixHardware.h"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Command {
			class CStdHardware : public IProtonixCommand {
				private:
					String _id;
					String _cmd;

				public:
					CStdHardware ();

					CStdHardware* ID (String id);
					String ID ();

					CStdHardware* CMD (String cmd);
					String CMD ();

					bool CommandRecognize (Protonix* device, String command, IProtonixHardware* hardware);
					bool CommandSerialize ();
			};
		}
	}
}