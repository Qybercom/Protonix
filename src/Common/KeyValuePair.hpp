#pragma once

#include <Arduino.h>

namespace Qybercom {
	struct KeyValuePair {
		String Key;
		String Value;

		KeyValuePair(String key = "", String value = "") : Key(key), Value(value) { }
	};
}