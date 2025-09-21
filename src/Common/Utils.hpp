#pragma once

#include <Arduino.h>
#include <math.h>

#include "List.hpp"

namespace Qybercom {
	inline List<String>* explode (String delimiter, String input) {
		List<String>* out = new List<String>();

		unsigned int start = 0;
		int pos = input.indexOf(delimiter, start);

		while (pos >= 0) {
			String token = input.substring(start, pos);
			out->Add(token);

			start = pos + delimiter.length();
			pos = input.indexOf(delimiter, start);
		}

		if (start <= input.length())
			out->Add(input.substring(start));

		return out;
	}

	inline List<String>* strChunks (String input, unsigned int chunkSize) {
		List<String>* out = new List<String>();

		if (chunkSize > 0) {
			int length = input.length();
			int start = 0;

			while (start < length) {
				int end = start + chunkSize;
				if (end > length) end = length;

				out->Add(input.substring(start, end));

				start += chunkSize;
			}
		}

		return out;
	}

	inline uint8_t* strBuf (String str) {
		int len = str.length();
		uint8_t* out = nullptr;

		if (len != 0) {
			out = new uint8_t[len];

			int i = 0;
			while (i < len) {
				out[i] = (uint8_t)str[i];

				i++;
			}
		}

		return out;
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
		if (angleDegrees < 0.0)
			angleDegrees += 360.0;

		return angleDegrees;
	}

	inline int angleByXYRounded (int x, int y, int minX, int maxX, int minY, int maxY) {
		return round(angleByXY(x, y, minX, maxX, minY, maxY));
	}

	inline float angleOffset (float value, float offset) {
		float out = value - offset;

		if (out < 0)
			out = 360.0 + out;

		return out;
	}

	inline int angleOffset (int value, int offset) {
		return (int)angleOffset((float)value, (float)offset);
	}

	inline bool matchRange (float value, float min, float max, bool minEQ = true, bool maxEQ = true) {
		bool _min = minEQ ? value >= min : value > min;
		bool _max = maxEQ ? value <= max : value < max;

		return max >= min ? _min && _max : _min || _max;
	}

	inline bool matchRange (int value, int min, int max, bool minEQ = true, bool maxEQ = true) {
		return matchRange((float)value, (float)min, (float)max, minEQ, maxEQ);
	}

	inline uint8_t hexNibble (char c) {
		if (c >= '0' && c <= '9') return c - '0';
		if (c >= 'A' && c <= 'F') return c - 'A' + 10;
		if (c >= 'a' && c <= 'f') return c - 'a' + 10;

		return 0;
	}

	inline List<uint8_t> hexToBytes (const String &hex) {
		List<uint8_t> out;

		int len = hex.length();
		int i = 0;

		while (i < len) {
			// skip spaces
			while (i < len && isspace((unsigned char) hex[i])) ++i;

			if (i >= len) break;

			// read two hex chars (skip separators if single nibble)
			char c1 = hex[i++];

			// skip spaces
			while (i < len && isspace((unsigned char) hex[i])) ++i;

			if (i >= len) {
				// single nibble -> treat as '0x0c1'
				uint8_t b = hexNibble(c1);
				out.Add((uint8_t)b);
				break;
			}

			char c2 = hex[i++];
			uint8_t b = (hexNibble(c1) << 4) | hexNibble(c2);
			out.Add(b);
		}

		return out;
	}
}