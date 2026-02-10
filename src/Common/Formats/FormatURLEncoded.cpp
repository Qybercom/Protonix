#include <Arduino.h>

#include "../Value.h"
#include "../Utils.h"

#include "FormatURLEncoded.h"

using namespace Qybercom;

String Formats::FormatURLEncoded::SYMBOLS_CLEAR[] = { " ", "@", "#", "$", "%", "^", "&", "+", "=", "[", "]", "{", "}", "|", ";", ":", "\"", ",", "<", ">", "?", "/" };
String Formats::FormatURLEncoded::SYMBOLS_CODES[] = { "%20", "%40", "%23", "%24", "%25", "%5E", "%26", "%2B", "%3D", "%5B", "%5D", "%7B", "%7D", "%7C", "%3B", "%3A", "%22", "%2C", "%3C", "%3E", "%3F", "%2F" };

void Formats::FormatURLEncoded::_encode (String &s) {
	unsigned short i = 0;

	while (i < 22) {
		s.replace(
			Formats::FormatURLEncoded::SYMBOLS_CLEAR[i],
			Formats::FormatURLEncoded::SYMBOLS_CODES[i]
		);

		i++;
	}
}

void Formats::FormatURLEncoded::_decode (String &s) {
	unsigned short i = 0;

	while (i < 22) {
		s.replace(
			Formats::FormatURLEncoded::SYMBOLS_CODES[i],
			Formats::FormatURLEncoded::SYMBOLS_CLEAR[i]
		);

		i++;
	}
}

void Formats::FormatURLEncoded::_serialize (Value &value, String &out, String &prefix) {
	if (value.IsObject()) {
		for (Value &v : value) {
			String key = v.Key();
			String newPrefix = prefix.length() ? prefix + "[" + key + "]" : key;

			Formats::FormatURLEncoded::_serialize(v, out, newPrefix);
		}
	}
	else if (value.IsArray()) {
		int idx = 0;

		for (Value &v : value) {
			String newPrefix = prefix + "[" + String(idx) + "]";

			Formats::FormatURLEncoded::_serialize(v, out, newPrefix);

			idx++;
		}
	}
	else {
		if (out.length()) out += "&";

		String k = prefix;
		String v = value.ToString();

		Formats::FormatURLEncoded::_encode(k);
		Formats::FormatURLEncoded::_encode(v);

		out += k + "=" + v;
	}
}

void Formats::FormatURLEncoded::_path (Value &node, List<String> &keys, String &value) {
	Value current = node;

	int keysCount = keys.Count();
	int i = 0;

	while (i < keysCount) {
		const String &key = keys[i];
		bool last = (i == keysCount - 1);

		if (key.length() == 0 || isNumeric(key)) {
			current = Value::Array();

			if (key.length() == 0) {
				if (last) {
					Value v;
					v = value;

					current.Add(v);

					return;
				}

				Value next;
				current.Add(next);

				int lastIndex = current.Count() - 1;
				current = &current[lastIndex];

				i++;

				continue;
			}

			i++;

			continue;
		}

		if (last) {
			current[key] = value;

			return;
		}

		current = current[key];

		i++;
	}
}

void Formats::FormatURLEncoded::_deserialize (Value &root, String &raw) {
	List<String> pairs = explode("&", raw);

	for (String &pair : pairs) {
		List<String> parts = explode("=", pair);
		String key = parts[0];
		String value = parts[1];

		Formats::FormatURLEncoded::_decode(key);
		Formats::FormatURLEncoded::_decode(value);

		int sub = key.indexOf('[');
		int len = key.length();
		String rootNode = sub == -1 ? key : key.substring(0, sub);
		String children = sub == -1 ? "" : key.substring(sub + 1, len - 1);

		List<String> keys = sub == -1 ? List<String>() : explode("][", children);
		keys.Add(rootNode, true);

		Formats::FormatURLEncoded::_path(root, keys, value);
	}
}

String Formats::FormatURLEncoded::ValueFormatMIME () {
	return "x-wwww/form-url-encoded";
}

String Formats::FormatURLEncoded::ValueFormatSerialize (Value &value) {
	String out;
	String prefix = "";

	Formats::FormatURLEncoded::_serialize(value, out, prefix);

	return out;
}

Value Formats::FormatURLEncoded::ValueFormatDeserialize (const String &raw) {
	Value root = Value::Object();
	String in = String(raw);

	Formats::FormatURLEncoded::_deserialize(root, in);

	return root;
}