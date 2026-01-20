#pragma once

#include <Arduino.h>

namespace Qybercom {
	struct Color {
		short R;
		short G;
		short B;
		float A;

		Color (short r = 0, short g = 0, short b = 0, float a = 1.0);

		String Serialize ();
		static Color Deserialize (String &raw);
	};
}