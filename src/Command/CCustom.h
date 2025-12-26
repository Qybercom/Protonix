#pragma once

#include <Arduino.h>

#include "../IProtonixCommand.h"
#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Command {
			class CCustom : public IProtonixCommand {
				private:
					String _cmd;

				public:
					CCustom ();
					CCustom (String cmd);

					void CMD (String cmd);
					String CMD ();

					bool CommandRecognize (Protonix* device, String command, IProtonixHardware* hardware);
					bool CommandSerialize ();
			};
		}
	}
}