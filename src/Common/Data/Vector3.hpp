#pragma once

#include <Arduino.h>

namespace Qybercom {
	struct Vector3 {
		int X;
		int Y;
		int Z;

		Vector3 (int x = 0, int y = 0, int z = 0) : X(x), Y(y), Z(z) {}

		String Serialize () {
			return String(X) + "," + String(Y) + "," + String(Z);
		}
	};
}