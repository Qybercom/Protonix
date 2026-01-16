#pragma once

#include <Arduino.h>

#include "../Data/index.h"

namespace Qybercom {
	namespace Formats {
		class FormatINI : public IBucketFormat {
			private:
				Value _parseValue (const String &value);

			public:
				String BucketFormatSerialize (Bucket &bucket);
				Bucket BucketFormatDeserialize (const String &raw);
		};
	}
}