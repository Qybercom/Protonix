#pragma once

#include <Arduino.h>

#include "../Data/index.h"

namespace Qybercom {
	namespace Formats {
		class FormatYAML : public IBucketFormat {
			private:
				static String _serialize (Bucket &bucket, int indentSize = 0, bool first = true);
			
			public:
				String BucketFormatMIME ();
				String BucketFormatSerialize (Bucket &bucket);
				Bucket BucketFormatDeserialize (const String &raw);
		};
	}
}