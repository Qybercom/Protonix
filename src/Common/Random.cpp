#include <Arduino.h>

#include "Random.h"

using namespace Qybercom;

String Random::ALPHABET_HEX = "0123456789abcdef";
String Random::ALPHABET_ALL = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
String Random::ALPHABET_DIGITS = "0123456789";
unsigned short Random::UUID_SECTIONS[] = {8, 4, 4, 4, 12};

void Random::Init (bool micro) {
	randomSeed(micro ? micros() : millis());
}

String Random::AZaz09 (unsigned int length, String &alphabet) {
	String out = "";

	unsigned int i = 0;
	unsigned int max = alphabet.length() - 1;

	while (i < length) {
		out += alphabet[random(max)];

		i++;
	}

	return out;
}

String Random::AZaz09 (unsigned int length) {
	return Random::AZaz09(length, ALPHABET_ALL);
}

String Random::Digits (unsigned int length) {
	return Random::AZaz09(length, ALPHABET_DIGITS);
}

String Random::UUID () {
	String out = "";

	unsigned int i = 0;

	while (i < 5) {
		out += Random::AZaz09(UUID_SECTIONS[i], ALPHABET_HEX);

		if (i < 4)
			out += "-";

		i++;
	}

	return out;
}