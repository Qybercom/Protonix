#pragma once

#include <Arduino.h>

#include "../Value.h"

namespace Qybercom {
	namespace Types {
		struct Color : public IValueType {
			short R;
			short G;
			short B;
			float A;

			Color (short r = 0, short g = 0, short b = 0, float a = 1.0);

			void ValueTypeDeserialize (const String &raw);
			String ValueTypeSerialize () const;
		};
	}
}