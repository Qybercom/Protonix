#pragma once

#include <Arduino.h>

#include "Value.h"

namespace Qybercom {
	struct Range : public IValue {
		double Min;
		double Max;

		Range (double min = 0.0, double max = 0.0);

		void ValueDeserialize (const String &raw);
		String ValueSerialize () const;
	};
}