#pragma once

#include <Arduino.h>

namespace Qybercom {
	struct Vector4 {
		int A;
		int B;
		int C;
		int D;

		Vector4 (int a = 0, int b = 0, int c = 0, int d = 0) : A(a), B(b), C(c), D(d) {}

		String Serialize () {
			return String(A) + "," + String(B) + "," + String(C) + "," + String(D);
		}
	};
}