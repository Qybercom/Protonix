#pragma once

#include <Arduino.h>

#include "IProtonixHardware.h"

namespace Qybercom {
	namespace Protonix {
		class Protonix;

		class IProtonixCommand {
			protected:
				String _name;
				String _buffer;

				void _init (String name);
				long _recognize (String name);

			public:
				void CommandReset ();
				String CommandName ();
				String CommandBuffer ();

				virtual bool CommandRecognize (Protonix* device, String command, IProtonixHardware* hardware) = 0;
				virtual bool CommandSerialize () = 0;

				virtual ~IProtonixCommand () { };
		};
	}
}