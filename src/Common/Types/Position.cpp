#include <Arduino.h>

#include "Position.h"

using namespace Qybercom::Types;

Position::Position (double x, double y, double z) {
	X = x;
	Y = y;
	Z = z;
}

void Position::ValueTypeDeserialize (const String &raw) {
	int i = 0;
	int length = raw.length();
	String pos[4] = {"", "", ""};
	int j = 0;
	char c;

	while (i < length) {
		c = raw.charAt(i);

		if (c == ',') j++;
		else pos[j] += c;

		if (j == 3) break;

		i++;
	}

	X = pos[0].toInt();
	Y = pos[1].toInt();
	Z = pos[2].toInt();
}

String Position::ValueTypeSerialize () {
	return String(X)
		+ "," + String(Y)
		+ "," + String(Z);
}