#pragma once

#include <Arduino.h>

#include "Value.h"

namespace Qybercom {
	struct Position : public IValue {
		double X;
		double Y;
		double Z;

		Position (double x = 0.0, double y = 0.0, double z = 0.0);

		void ValueDeserialize (const String &raw);
		String ValueSerialize () const;
	};
}