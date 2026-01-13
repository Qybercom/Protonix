#pragma once

#include <Arduino.h>

namespace Qybercom {
	struct Vector2 {
		int X;
		int Y;

		Vector2 (int x = 0, int y = 0) : X(x), Y(y) {}

		String Serialize () {
			return String(X) + "," + String(Y);
		}
	};
}