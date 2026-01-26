#pragma once

#include <Arduino.h>

#include "../Value.h"

namespace Qybercom {
	namespace Formats {
		class FormatJSON : public IValueFormat {
			private:
				static void _skip (const String &s, int &pos);
				static Value _parseValue (const String &s, int &pos);
				static Value _parseObject (const String &s, int &pos);
				static Value _parseArray (const String &s, int &pos);
				static String _parseString (const String &s, int &pos);
				static int _parseNumber (const String &s, int &pos);

			public:
				String ValueFormatMIME ();
				String ValueFormatSerialize (Value &value);
				Value ValueFormatDeserialize (const String &raw);
		};
	}
}