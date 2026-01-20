#include <Arduino.h>

#include "Color.h"

using namespace Qybercom;

Color::Color (short r, short g, short b, float a) {
	R = r;
	G = g;
	B = b;
	A = a;
}

String Color::Serialize () {
	return String(R)
		+ "," + String(G)
		+ "," + String(B)
		+ "," + String(A);
}

Color Color::Deserialize (String &raw) {
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

	return Color(
		clr[0].toInt(),
		clr[1].toInt(),
		clr[2].toInt(),
		clr[4].toFloat()
	);
}