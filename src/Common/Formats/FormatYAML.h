#pragma once

#include <Arduino.h>

#include "../Value.h"

namespace Qybercom {
	namespace Formats {
		class FormatYAML : public IValueFormat {
			private:
				static String _serialize (Value &value, int indentSize = 0, bool first = true);
			
			public:
				String ValueFormatMIME ();
				String ValueFormatSerialize (Value &value);
				Value ValueFormatDeserialize (const String &raw);
		};
	}
}