#include <Arduino.h>

#include "../Data/index.h"

#include "FormatJSON.h"

using namespace Qybercom;

void Formats::FormatJSON::_skip (const String &s, int &pos) {
	while (pos < s.length() && (s[pos] == ' ' || s[pos] == '\n' || s[pos] == '\r' || s[pos] == '\t')) pos++;
}

Bucket Formats::FormatJSON::_parseValue (const String &s, int &pos) {
	Formats::FormatJSON::_skip(s, pos);
	if (pos >= s.length()) return Bucket::Null();

	char c = s[pos];
	if (c == '{') return Formats::FormatJSON::_parseObject(s, pos);
	if (c == '[') return Formats::FormatJSON::_parseArray(s, pos);
	if (c == '"') return Bucket::Value(Formats::FormatJSON::_parseString(s, pos));
	if (isDigit(c) || c == '-') return Bucket::Value(Formats::FormatJSON::_parseNumber(s, pos));
	if (s.substring(pos, pos + 4) == "true") { pos += 4; return Bucket::Value(true); }
	if (s.substring(pos, pos + 5) == "false") { pos += 5; return Bucket::Value(false); }
	if (s.substring(pos, pos + 4) == "null") { pos += 4; return Bucket::Null(); }

	return Bucket::Null();
}

Bucket Formats::FormatJSON::_parseObject (const String &s, int &pos) {
	Bucket obj = Bucket::Object();
	pos++; // skip '{'
	Formats::FormatJSON::_skip(s, pos);

	while (pos < s.length() && s[pos] != '}') {
		String key = Formats::FormatJSON::_parseString(s, pos);

		Formats::FormatJSON::_skip(s, pos);
		if (s[pos] == ':') pos++;
		Bucket value = Formats::FormatJSON::_parseValue(s, pos);
		obj[key] = value;

		Formats::FormatJSON::_skip(s, pos);
		if (s[pos] == ',') pos++;

		Formats::FormatJSON::_skip(s, pos);
	}

	pos++; // skip '}'

	return obj;
}

Bucket Formats::FormatJSON::_parseArray (const String &s, int &pos) {
	Bucket arr = Bucket::Array();
	pos++; // skip '['
	Formats::FormatJSON::_skip(s, pos);

	while (pos < s.length() && s[pos] != ']') {
		Bucket value = Formats::FormatJSON::_parseValue(s, pos);
		arr.Add(value);
		Formats::FormatJSON::_skip(s, pos);

		if (s[pos] == ',') pos++;
		Formats::FormatJSON::_skip(s, pos);
	}

	pos++; // skip ']'

	return arr;
}

String Formats::FormatJSON::_parseString (const String &s, int &pos) {
	pos++; // skip opening "
	String str;
	int length = s.length();

	while (pos < length && s[pos] != '"') {
		str += s[pos++];
	}

	pos++; // skip closing "

	return str;
}

int Formats::FormatJSON::_parseNumber (const String &s, int &pos) {
	String num;
	int length = s.length();

	while (pos < length && (isDigit(s[pos]) || s[pos] == '.' || s[pos] == '-')) {
		num += s[pos++];
	}

	return num.indexOf('.') >= 0 ? num.toFloat() : num.toInt();
}

String Formats::FormatJSON::BucketFormatSerialize (Bucket &bucket) {
	String out = "";

	if (bucket.HasKey())
		out += "\"" + bucket.Key() + "\":";

	if (bucket.IsValue()) {
		Value &v = bucket.AsValue();
		bool s = v.IsString();

		out += (s ? "\"" : "") + v.ToString() + (s ? "\"" : "");
	}

	if (bucket.IsArray()) {
		out += "[";

		for (Bucket &v : bucket)
			out += Formats::FormatJSON::BucketFormatSerialize(v) + String(v.End() ? "" : ",");

		out += "]";
	}

	if (bucket.IsObject()) {
		out += "{";

		for (Bucket &v : bucket)
			out += Formats::FormatJSON::BucketFormatSerialize(v) + String(v.End() ? "" : ",");

		out += "}";
	}

	return out;
}

Bucket Formats::FormatJSON::BucketFormatDeserialize (const String &raw) {
	int pos = 0;

	return Formats::FormatJSON::_parseValue(raw, pos);
}