#pragma once

namespace Qybercom {
	class Random {
		private:
			inline static String ALPHABET_HEX = "0123456789abcdef";
			inline static String ALPHABET_ALL = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
			inline static String ALPHABET_DIGITS = "0123456789";

			inline static unsigned short UUID_SECTIONS[] = {8, 4, 4, 4, 12};

		public:
			static void Init (bool micro = true) {
				randomSeed(micro ? micros() : millis());
			}

			static String AZaz09 (unsigned int length, String &alphabet) {
				String out = "";

				unsigned int i = 0;
				unsigned int max = alphabet.length() - 1;

				while (i < length) {
					out += alphabet[random(max)];

					i++;
				}

				return out;
			}

			static String AZaz09 (unsigned int length = 8) {
				return AZaz09(length, ALPHABET_ALL);
			}

			static String Digits (unsigned int length = 8) {
				return AZaz09(length, ALPHABET_DIGITS);
			}

			static String UUID () {
				String out = "";

				unsigned int i = 0;

				while (i < 5) {
					out += AZaz09(UUID_SECTIONS[i], ALPHABET_HEX);

					if (i < 4)
						out += "-";

					i++;
				}

				return out;
			}
	};
}