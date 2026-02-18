#pragma once

#include <Arduino.h>

#include "../Value.h"

namespace Qybercom {
	namespace Types {
		struct Position : public IValueType {
			double X;
			double Y;
			double Z;

			Position (double x = 0.0, double y = 0.0, double z = 0.0);

			void ValueTypeDeserialize (const String &raw);
			String ValueTypeSerialize ();
		};
	}
}