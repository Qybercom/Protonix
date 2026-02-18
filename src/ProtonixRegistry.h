#pragma once

#include <Arduino.h>

#include "Common/index.h"

#include "ProtonixMemory.h"

namespace Qybercom {
	namespace Protonix {
		class ProtonixRegistry {
			private:
				ProtonixMemory* _memory;
				IValueFormat* _format;
				Value _data;
				String _raw;
				bool _loaded;
				bool _debug;

			public:
				ProtonixRegistry (ProtonixMemory* memory);

				Value &Data ();
				String &Raw ();

				bool Load ();
				bool Load (String raw);

				bool Save ();
				bool Save (String key, const Value &value);

				ProtonixRegistry* Debug (bool debug);
				bool Debug ();
		};
	}
}