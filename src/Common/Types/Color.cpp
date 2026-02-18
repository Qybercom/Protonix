#include <Arduino.h>

#include "Color.h"

using namespace Qybercom::Types;

Color::Color (short r, short g, short b, float a) {
	R = r;
	G = g;
	B = b;
	A = a;
}

void Color::ValueTypeDeserialize (const String &raw) {
	int i = 0;
	int length = raw.length();
	String clr[4] = {"", "", "", ""};
	int j = 0;
	char c;

	while (i < length) {
		c = raw.charAt(i);

		if (c == ',') j++;
		else clr[j] += c;

		if (j == 4) break;

		i++;
	}

	R = clr[0].toInt();
	G = clr[1].toInt();
	B = clr[2].toInt();
	A = clr[3].toFloat();
}

String Color::ValueTypeSerialize () {
	return String(R)
		+ "," + String(G)
		+ "," + String(B)
		+ "," + String(A);
}