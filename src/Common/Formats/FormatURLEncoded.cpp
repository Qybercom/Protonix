#include <Arduino.h>

#include "../Data/index.h"

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

void Formats::FormatURLEncoded::_serialize (Bucket &bucket, String &out, String &prefix) {
	if (bucket.IsObject()) {
		for (Bucket &v : bucket) {
			String key = v.Key();
			String newPrefix = prefix.length() ? prefix + "[" + key + "]" : key;

			Formats::FormatURLEncoded::_serialize(v, out, newPrefix);
		}
	}
	else if (bucket.IsArray()) {
		int idx = 0;

		for (Bucket &v : bucket) {
			String newPrefix = prefix + "[" + String(idx) + "]";

			Formats::FormatURLEncoded::_serialize(v, out, newPrefix);

			idx++;
		}
	}
	else if (bucket.IsValue() || bucket.IsNull()) {
		if (out.length()) out += "&";

		String key = prefix;
		String value = String(bucket.IsNull() ? "" : bucket.AsValue());

		Formats::FormatURLEncoded::_encode(key);
		Formats::FormatURLEncoded::_encode(value);

		out += key + "=" + value;
	}
}

void Formats::FormatURLEncoded::_path (Bucket &node, List<String> &keys, String &value) {
	Bucket *current = &node;

	int keysCount = keys.Count();
	int i = 0;

	while (i < keysCount) {
		const String &key = keys[i];
		bool last = (i == keysCount - 1);

		if (key.length() == 0 || isNumeric(key)) {
			current->AsArray();

			// a[]=...
			if (key.length() == 0) {
				if (last) {
					Bucket v;
					v = value;

					current->Add(v);

					return;
				}

				Bucket next;
				current->Add(next);

				int lastIndex = current->AsArray().Count() - 1;
				current = &current->AsArray()[lastIndex];

				i++;

				continue;
			}

			// a[NUM]=...
			int index = toNumeric(key);

			List<Bucket> &arr = current->AsArray();
			int arrCount = arr.Count();

			while (arrCount <= index) {
				arr.Add(Bucket());

				arrCount++;
			}

			if (last) {
				arr[index] = value;

				return;
			}

			current = &arr[index];

			i++;

			continue;
		}

		current->AsObject();

		if (last) {
			(*current)[key] = value;
			return;
		}

		current = &(*current)[key];

		i++;
	}
}

void Formats::FormatURLEncoded::_deserialize (Bucket &root, String &raw) {
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

String Formats::FormatURLEncoded::BucketFormatSerialize (Bucket &bucket) {
	String out;
	String prefix = "";

	Formats::FormatURLEncoded::_serialize(bucket, out, prefix);

	return out;
}

Bucket Formats::FormatURLEncoded::BucketFormatDeserialize (const String &raw) {
	Bucket root = Bucket::Object();
	String in = String(raw);

	Formats::FormatURLEncoded::_deserialize(root, in);

	return root;
}