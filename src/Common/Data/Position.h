#pragma once

#include <Arduino.h>

namespace Qybercom {
	struct Position {
		double X;
		double Y;
		double Z;

		Position (double x = 0.0, double y = 0.0, double z = 0.0);

		String Serialize ();
		static Position Deserialize (String& str);
	};
}