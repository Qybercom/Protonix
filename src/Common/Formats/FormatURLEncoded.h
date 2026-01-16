#pragma once

#include <Arduino.h>
#include "../Data/index.h"

namespace Qybercom {
	namespace Formats {
		class FormatURLEncoded : public IBucketFormat {
			private:
				static String SYMBOLS_CLEAR[];
				static String SYMBOLS_CODES[];

				static void _encode (String &s);
				static void _decode (String &s);

				static void _path (Bucket &node, List<String> &keys, String &value);

				static void _serialize (Bucket &bucket, String &out, String &prefix);
				static void _deserialize (Bucket &root, String &raw);

			public:
				String BucketFormatSerialize (Bucket &bucket);
				Bucket BucketFormatDeserialize (const String &raw);
			};
	}
}