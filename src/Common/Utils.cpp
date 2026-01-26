#include <Arduino.h>

#include "Utils.h"

using namespace Qybercom;

List<String> Qybercom::explode (String delimiter, String input) {
	List<String> out;

	unsigned int start = 0;
	int pos = input.indexOf(delimiter, start);

	while (pos >= 0) {
		String token = input.substring(start, pos);
		out.Add(token);

		start = pos + delimiter.length();
		pos = input.indexOf(delimiter, start);
	}

	if (start <= input.length())
		out.Add(input.substring(start));

	return out;
}

List<String> Qybercom::strChunks (String &input, unsigned int chunkSize) {
	List<String> out;

	if (chunkSize > 0) {
		int length = input.length();
		int start = 0;

		while (start < length) {
			int end = start + chunkSize;
			if (end > length) end = length;

			out.Add(input.substring(start, end));

			start += chunkSize;
		}
	}

	return out;
}

uint8_t* Qybercom::strBuf (String &str) {
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

char* Qybercom::strDup (const char* s) {
	if (!s) return nullptr;

	int len = strlen(s) + 1;
	char* out = new char[len];
	int i = 0;

	while (i < len) {
		out[i] = s[i];

		i++;
	}

	return out;
}

float Qybercom::angleByXY (int x, int y, int minX, int maxX, int minY, int maxY) {
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

int Qybercom::angleByXYRounded (int x, int y, int minX, int maxX, int minY, int maxY) {
	return round(Qybercom::angleByXY(x, y, minX, maxX, minY, maxY));
}

float Qybercom::angleOffset (float value, float offset) {
	float out = value - offset;

	if (out < 0)
		out = 360.0 + out;

	return out;
}

int Qybercom::angleOffset (int value, int offset) {
	return (int)Qybercom::angleOffset((float)value, (float)offset);
}

bool Qybercom::matchRange (float value, float min, float max, bool minEQ, bool maxEQ) {
	bool _min = minEQ ? value >= min : value > min;
	bool _max = maxEQ ? value <= max : value < max;

	return max >= min ? _min && _max : _min || _max;
}

bool Qybercom::matchRange (int value, int min, int max, bool minEQ, bool maxEQ) {
	return Qybercom::matchRange((float)value, (float)min, (float)max, minEQ, maxEQ);
}

inline uint8_t Qybercom::hexNibble (char c) {
	if (c >= '0' && c <= '9') return c - '0';
	if (c >= 'A' && c <= 'F') return c - 'A' + 10;
	if (c >= 'a' && c <= 'f') return c - 'a' + 10;

	return 0;
}

String Qybercom::hexString (String &data, bool delim) {
	String s = "";
	short length = data.length();
	int i = 0;

	while (i < length) {
		uint8_t b = data[i];
		if (b < 16) s += '0';

		s += String(b, HEX);

		if (delim && i + 1 < length) s += ' ';

		i++;
	}

	return s;
}

List<uint8_t> Qybercom::hexToBytes (const String &hex) {
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
			uint8_t b = Qybercom::hexNibble(c1);
			out.Add((uint8_t)b);
			break;
		}

		char c2 = hex[i++];
		uint8_t b = (Qybercom::hexNibble(c1) << 4) | Qybercom::hexNibble(c2);
		out.Add(b);
	}

	return out;
}

String Qybercom::indent (unsigned int size) {
	String out = "";
	unsigned int i = 0;

	while (i < size) {
		out += "  ";

		i++;
	}

	return out;
}

bool Qybercom::isNumeric (const String &s) {
	int l = s.length();
	if (l == 0) return false;

	int i = 0;

	while (i < l) {
		char c = s[i];

		if (c < '0' || c > '9')
			return false;

		i++;
	}

	return true;
}

int Qybercom::toNumeric (const String &s) {
	int v = 0;
	int i = 0;
	int l = s.length();

	while (i < l) {
		v = v * 10 + (s[i] - '0');

		i++;
	}

	return v;
}