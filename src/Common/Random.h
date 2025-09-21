#pragma once

#include <Arduino.h>

namespace Qybercom {
	class Random {
		private:
			static String ALPHABET_HEX;
			static String ALPHABET_ALL;
			static String ALPHABET_DIGITS;

			static unsigned short UUID_SECTIONS[];

		public:
			static void Init (bool micro = true);

			static String AZaz09 (unsigned int length, String &alphabet);
			static String AZaz09 (unsigned int length = 8);
			static String Digits (unsigned int length = 8);
			static String UUID ();
	};
}