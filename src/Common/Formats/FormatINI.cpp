#include <Arduino.h>

#include "../Data/index.h"

#include "FormatINI.h"

using namespace Qybercom;

String Formats::FormatINI::BucketFormatMIME () {
	return "application/ini";
}

String Formats::FormatINI::BucketFormatSerialize (Bucket &bucket) {
	String out = "";

	for (Bucket &entry : bucket) {
		String key = entry.HasKey() ? entry.Key() : "";

		if (entry.IsArray()) {
			// TODO: think about arrays
			continue;
		}

		if (entry.IsObject()) {
			out += "[" + key + "]\r\n";

			for (Bucket &item : entry) {
				if (item.IsObject() || item.IsArray()) {
					// TODO: think about nested
					continue;
				}

				out += item.Key() + "=" + item.AsValue().ToString() + "\r\n";
			}

			continue;
		}

		out += entry.Key() + "=" + entry.AsValue().ToString() + "\r\n";
	}

	return out;
}

Bucket Formats::FormatINI::BucketFormatDeserialize (const String &raw) {
	Bucket root = Bucket::Object();
	Bucket *currentSection = &root;

	int i = 0;
	int len = raw.length();

	while (i < len) {
		int start = i;
		while (i < len && raw[i] != '\n' && raw[i] != '\r') i++;

		String line = raw.substring(start, i);
		line.trim();

		while (i < len && (raw[i] == '\n' || raw[i] == '\r')) i++;

		if (line.length() == 0) continue;

		char c0 = line[0];
		if (c0 == ';' || c0 == '#') continue;

		if (c0 == '[' && line.endsWith("]")) {
			String sectionName = line.substring(1, line.length() - 1);
			sectionName.trim();

			currentSection = &root[sectionName];

			continue;
		}

		int eq = line.indexOf('=');
		if (eq < 0) continue;

		String key = line.substring(0, eq);
		String value = line.substring(eq + 1);
		key.trim();
		value.trim();

		// TODO: review key[]=value syntax
		/*if (key.endsWith("[]")) {
			String baseKey = key.substring(0, key.length() - 2);
			Bucket &arrBucket = (*currentSection)[baseKey];
			if (!arrBucket.IsArray()) arrBucket = Bucket::Array();

			arrBucket.AsArray().Add(Bucket::Value(Formats::FormatINI::_parseValue(value)));
		}*/
		(*currentSection)[key] = Bucket::Value(Formats::FormatINI::_parseValue(value));
	}

	return root;
}

Value Formats::FormatINI::_parseValue (const String &value) {
	if (value.equalsIgnoreCase("true")) return Value(true);
	if (value.equalsIgnoreCase("false")) return Value(false);

	bool isInt = true;
	bool isFloat = false;
	int i = 0;
	int len = value.length();

	if (len == 0) return Value(value);

	if (value[0] == '-' || value[0] == '+') i = 1;

	while (i < len) {
		char c = value[i];

		if (c == '.') {
			if (isFloat) { isInt = false; break; }

			isFloat = true;
			i++;

			continue;
		}

		if (c < '0' || c > '9') { isInt = false; break; }

		i++;
	}

	if (isInt && !isFloat) return Value(value.toInt());
	if (isInt && isFloat) return Value(value.toFloat());

	return Value(value);
}