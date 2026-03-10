#pragma once

#include <Arduino.h>

#include "../Value.h"

namespace Qybercom {
	namespace Types {
		struct Range : public IValueType {
			double Min;
			double Max;

			Range (double min = 0.0, double max = 0.0);

			bool Match (float value, bool minEQ = true, bool maxEQ = true);
			bool Match (int value, bool minEQ = true, bool maxEQ = true);

			void ValueTypeDeserialize (const String &raw);
			String ValueTypeSerialize ();
		};
	}
}