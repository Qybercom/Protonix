#pragma once

#include <Arduino.h>

#include "../Value.h"

namespace Qybercom {
	namespace Formats {
		class FormatURLEncoded : public IValueFormat {
			private:
				static String SYMBOLS_CLEAR[];
				static String SYMBOLS_CODES[];

				static void _encode (String &s);
				static void _decode (String &s);

				static void _path (Value &node, List<String> &keys, String &value);

				static void _serialize (Value &value, String &out, String &prefix);
				static void _deserialize (Value &root, String &raw);

			public:
				String ValueFormatMIME ();
				String ValueFormatSerialize (Value &value);
				Value ValueFormatDeserialize (const String &raw);
			};
	}
}