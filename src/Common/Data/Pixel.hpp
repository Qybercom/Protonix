#pragma once

#include <Arduino.h>

namespace Qybercom {
	struct Pixel {
		byte R;
		byte G;
		byte B;
		unsigned long X;
		unsigned long Y;

		Pixel (byte r = 0, byte g = 0, byte b = 0, unsigned long x = 0, unsigned long y = 0)
			: R(r), G(g), B(b), X(x), Y(y) { }
	};
}