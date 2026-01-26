#pragma once

#include <Arduino.h>

#include "../Value.h"

namespace Qybercom {
	namespace Formats {
		class FormatINI : public IValueFormat {
			private:
				Value _parseValue (const String &value);

			public:
				String ValueFormatMIME ();
				String ValueFormatSerialize (Value &value);
				Value ValueFormatDeserialize (const String &raw);
		};
	}
}