#include <Arduino.h>

#include "Range.h"

using namespace Qybercom::Types;

Range::Range (double min, double max) {
	Min = min;
	Max = max;
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