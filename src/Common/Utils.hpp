#pragma once

#include <Arduino.h>

#include "List.hpp"

namespace Qybercom {
	inline List<String>* explode (String delimiter, String input) {
		List<String>* result = new List<String>();

		int start = 0;
		int pos = input.indexOf(delimiter, start);

		while (pos >= 0) {
			String token = input.substring(start, pos);
			result->Add(token);

			start = pos + delimiter.length();
			pos = input.indexOf(delimiter, start);
		}

		if (start <= input.length())
			result->Add(input.substring(start));

		return result;
	}
}