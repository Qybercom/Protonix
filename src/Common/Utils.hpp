#pragma once

#include <Arduino.h>
#include <math.h>

#include "List.hpp"

namespace Qybercom {
	inline List<String>* explode (String delimiter, String input) {
		List<String>* result = new List<String>();

		int start = 0;
		int pos = input.indexOf(delimiter, start);

		while (pos >= 0) {
			String token = input.substring(start, pos);
			result->Add(token);

			start = pos + delimiter.length();
			pos = input.indexOf(delimiter, start);
		}

		if (start <= input.length())
			result->Add(input.substring(start));

		return result;
	}

	inline float angleByXY (int x, int y, int minX, int maxX, int minY, int maxY) {
		int avgX = (minX + maxX) / 2;
		int avgY = (minY + maxY) / 2;

		// coordinate normalization by center
		float normalizedX = (float)(x - avgX) / (maxX - minX);
		float normalizedY = (float)(y - avgY) / (maxY - minY);

		float angleRadians = atan2(normalizedY, normalizedX);
		float angleDegrees = angleRadians * 180.0 / PI;

		// normalization to 0..360 range
		if (angleDegrees < 0)
			angleDegrees += 360;

		return angleDegrees;
	}

	inline int angleByXYRounded (int x, int y, int minX, int maxX, int minY, int maxY) {
		return round(angleByXY(x, y, minX, maxX, minY, maxY));
	}

	inline int angleOffset (int value, int offset) {
		int out = value - offset;

		if (out < 0)
			out = 360 + out;

		return out;
	}

	inline bool matchRange (int value, int min, int max, bool minEQ = true, bool maxEQ = true) {
		bool _min = minEQ ? value >= min : value > min;
		bool _max = maxEQ ? value <= max : value < max;

		return max >= min ? _min && _max : _min || _max;
	}
}