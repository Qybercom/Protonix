#include <Arduino.h>

#include "../Utils.h"

#include "Range.h"

using namespace Qybercom::Types;

Range::Range (double min, double max) {
	Min = min;
	Max = max;
}

bool Range::Match (float value, bool minEQ, bool maxEQ) {
	return Qybercom::matchRange(value, Min, Max, minEQ, maxEQ);
}

bool Range::Match (int value, bool minEQ, bool maxEQ) {
	return Qybercom::matchRange(value, Min, Max, minEQ, maxEQ);
}

void Range::ValueTypeDeserialize (const String &raw) {
	int delim = raw.indexOf('/');
	if (delim == -1) return;

	Min = raw.substring(0, delim).toDouble();
	Max = raw.substring(delim + 1).toDouble();
}

String Range::ValueTypeSerialize () {
	return String(Min) + "," + String(Max);
}