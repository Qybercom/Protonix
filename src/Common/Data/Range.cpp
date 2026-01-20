#include <Arduino.h>

#include "Range.h"

using namespace Qybercom;

Range::Range (double min, double max) {
	Min = min;
	Max = max;
}

void Range::ValueDeserialize (const String &raw) {
	int delim = raw.indexOf('/');
	if (delim == -1) return;

	Min = raw.substring(0, delim).toDouble();
	Max = raw.substring(delim + 1).toDouble();
}

String Range::ValueSerialize () const {
	return String(Min) + "," + String(Max);
}