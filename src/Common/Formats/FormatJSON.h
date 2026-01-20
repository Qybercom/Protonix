#pragma once

#include <Arduino.h>

#include "../Data/index.h"

namespace Qybercom {
	namespace Formats {
		class FormatJSON : public IBucketFormat {
			private:
				static void _skip (const String &s, int &pos);
				static Bucket _parseValue (const String &s, int &pos);
				static Bucket _parseObject (const String &s, int &pos);
				static Bucket _parseArray (const String &s, int &pos);
				static String _parseString (const String &s, int &pos);
				static int _parseNumber (const String &s, int &pos);

			public:
				String BucketFormatMIME ();
				String BucketFormatSerialize (Bucket &bucket);
				Bucket BucketFormatDeserialize (const String &raw);
		};
	}
}