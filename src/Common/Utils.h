#pragma once

#include <Arduino.h>
#include <math.h>

#include "Data/List.hpp"

namespace Qybercom {
	List<String> explode (String delimiter, String &input);
	List<String> strChunks (String &input, unsigned int chunkSize);
	uint8_t* strBuf (String &str);

	float angleByXY (int x, int y, int minX, int maxX, int minY, int maxY);
	int angleByXYRounded (int x, int y, int minX, int maxX, int minY, int maxY);
	float angleOffset (float value, float offset);
	int angleOffset (int value, int offset);

	bool matchRange (float value, float min, float max, bool minEQ = true, bool maxEQ = true);
	bool matchRange (int value, int min, int max, bool minEQ = true, bool maxEQ = true);

	inline uint8_t hexNibble (char c);
	String hexString (String &data, bool delim = true);
	List<uint8_t> hexToBytes (const String &hex);

	String indent (unsigned int size);

	bool isNumeric (const String &s);
	int toNumeric (const String &s);
}