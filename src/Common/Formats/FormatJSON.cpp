#include <Arduino.h>

#include "../Value.h"

#include "FormatJSON.h"

using namespace Qybercom;

void Formats::FormatJSON::_skip (const String &s, int &pos) {
	while (pos < s.length() && (s[pos] == ' ' || s[pos] == '\n' || s[pos] == '\r' || s[pos] == '\t')) pos++;
}

Value Formats::FormatJSON::_parseValue (const String &s, int &pos) {
	Formats::FormatJSON::_skip(s, pos);
	if (pos >= s.length()) return Value();

	char c = s[pos];
	if (c == '{') return Formats::FormatJSON::_parseObject(s, pos);
	if (c == '[') return Formats::FormatJSON::_parseArray(s, pos);
	if (c == '"') return Value(Formats::FormatJSON::_parseString(s, pos));
	if (isDigit(c) || c == '-') return Value(Formats::FormatJSON::_parseNumber(s, pos));
	if (s.substring(pos, pos + 4) == "true") { pos += 4; return Value(true); }
	if (s.substring(pos, pos + 5) == "false") { pos += 5; return Value(false); }
	if (s.substring(pos, pos + 4) == "null") { pos += 4; return Value(); }

	return Value();
}

Value Formats::FormatJSON::_parseObject (const String &s, int &pos) {
	Value obj = Value::Object();
	pos++; // skip '{'
	Formats::FormatJSON::_skip(s, pos);

	while (pos < s.length() && s[pos] != '}') {
		String key = Formats::FormatJSON::_parseString(s, pos);

		Formats::FormatJSON::_skip(s, pos);
		if (s[pos] == ':') pos++;
		Value value = Formats::FormatJSON::_parseValue(s, pos);
		obj[key] = value;

		Formats::FormatJSON::_skip(s, pos);
		if (s[pos] == ',') pos++;

		Formats::FormatJSON::_skip(s, pos);
	}

	pos++; // skip '}'

	return obj;
}

Value Formats::FormatJSON::_parseArray (const String &s, int &pos) {
	Value arr = Value::Array();
	pos++; // skip '['
	Formats::FormatJSON::_skip(s, pos);

	while (pos < s.length() && s[pos] != ']') {
		Value value = Formats::FormatJSON::_parseValue(s, pos);
		arr.Add(value);
		Formats::FormatJSON::_skip(s, pos);

		if (s[pos] == ',') pos++;
		Formats::FormatJSON::_skip(s, pos);
	}

	pos++; // skip ']'

	return arr;
}

String Formats::FormatJSON::_parseString(const String &s, int &pos) {
	pos++; // skip opening "
	String str;
	int length = s.length();

	while (pos < length) {
		char c = s[pos++];

		if (c == '"') {
			// closing quote
			break;
		}

		if (c == '\\' && pos < length) {
			// escape sequence
			char next = s[pos++];
			switch (next) {
				case '"':  str += '"';  break;
				case '\\': str += '\\'; break;
				case '/':  str += '/';  break;
				case 'b':  str += '\b'; break;
				case 'f':  str += '\f'; break;
				case 'n':  str += '\n'; break;
				case 'r':  str += '\r'; break;
				case 't':  str += '\t'; break;
				default:
					// неизвестный экранированный символ, просто вставляем как есть
					str += next;
					break;
			}
		}
		else {
			str += c;
		}
	}

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

String Formats::FormatJSON::ValueFormatMIME () {
	return "application/json";
}

String Formats::FormatJSON::ValueFormatSerialize (Value &value) {
	String out = "";

	if (value.HasKey())
		out += "\"" + value.Key() + "\":";

	if (value.IsArray()) {
		out += "[";
		bool start = false;

		for (Value &v : value) {
			if (start) out += ",";
			else start = true;

			out += Formats::FormatJSON::ValueFormatSerialize(v);// + String(v.End() ? "" : ",");
		}

		out += "]";

		return out;
	}

	if (value.IsObject()) {
		out += "{";
		bool start = false;

		for (Value &v : value) {
			if (start) out += ",";
			else start = true;

			out += Formats::FormatJSON::ValueFormatSerialize(v);// + String(v.End() ? "" : ",");
		}

		out += "}";

		return out;
	}

	bool s = value.IsString();

	out += (s ? "\"" : "") + value.ToString() + (s ? "\"" : "");

	return out;
}

Value Formats::FormatJSON::ValueFormatDeserialize (const String &raw) {
	int pos = 0;

	return Formats::FormatJSON::_parseValue(raw, pos);
}