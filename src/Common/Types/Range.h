#pragma once

#include <Arduino.h>

#include "../Value.h"

namespace Qybercom {
	namespace Types {
		struct Range : public IValueType {
			double Min;
			double Max;

			Range (double min = 0.0, double max = 0.0);

			void ValueTypeDeserialize (const String &raw);
			String ValueTypeSerialize () const;
		};
	}
}