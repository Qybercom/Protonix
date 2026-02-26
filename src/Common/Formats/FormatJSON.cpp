#include <Arduino.h>

#include "../Value.h"
#include "../Utils.h"

#include "FormatJSON.h"

using namespace Qybercom;

void Formats::FormatJSON::_skip (const String &s, int &pos) {
	while (pos < s.length() && (s[pos] == ' ' || s[pos] == '\n' || s[pos] == '\r' || s[pos] == '\t')) pos++;
}

Value Formats::FormatJSON::_parseValue (const String &s, int &pos) {
	Formats::FormatJSON::_skip(s, pos);
	int len = s.length();
	if (pos >= len) return Value();

	char c = s[pos];
	if (c == '{') return Formats::FormatJSON::_parseObject(s, pos);
	if (c == '[') return Formats::FormatJSON::_parseArray(s, pos);
	if (c == '"') return Value(Formats::FormatJSON::_parseString(s, pos));
	if (isNumeric(s.substring(pos, len - 2)) || c == '-') return Value(Formats::FormatJSON::_parseNumber(s, pos));
	if (s.substring(pos, pos + 4) == "true") { pos += 4; return Value(true); }
	if (s.substring(pos, pos + 5) == "false") { pos += 5; return Value(false); }
	if (s.substring(pos, pos + 4) == "null") { pos += 4; return Value(); }

	return Value();
}

Value Formats::FormatJSON::_parseObject (const String &s, int &pos) {
	Value obj = Value::Object();
	pos++;
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

	pos++;

	return obj;
}

Value Formats::FormatJSON::_parseArray (const String &s, int &pos) {
	Value arr = Value::Array();
	pos++;
	Formats::FormatJSON::_skip(s, pos);

	while (pos < s.length() && s[pos] != ']') {
		Value value = Formats::FormatJSON::_parseValue(s, pos);
		arr.Add(value);
		Formats::FormatJSON::_skip(s, pos);

		if (s[pos] == ',') pos++;
		Formats::FormatJSON::_skip(s, pos);
	}

	pos++;

	return arr;
}

String Formats::FormatJSON::_parseString(const String &s, int &pos) {
	pos++;
	String str;
	int length = s.length();

	while (pos < length) {
		char c = s[pos++];
		if (c == '"') break;

		if (c == '\\' && pos < length) {
			char next = s[pos++];
			switch (next) {
				case '"': str += '"';  break;
				case '\\': str += '\\'; break;
				case '/': str += '/';  break;
				case 'b': str += '\b'; break;
				case 'f': str += '\f'; break;
				case 'n': str += '\n'; break;
				case 'r': str += '\r'; break;
				case 't': str += '\t'; break;
				default: str += next; break;
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

	if (value.IsUndefined()) return out;

	if (value.HasKey())
		out += "\"" + value.Key() + "\":";

	if (value.IsArray()) {
		out += "[";
		bool start = false;

		for (Value &v : value) {
			if (start) out += ",";
			else start = true;

			out += Formats::FormatJSON::ValueFormatSerialize(v);
		}

		out += "]";

		return out;
	}

	if (value.IsObject()) {
		out += "{";
		bool start = false;

		for (Value &v : value) {
			String ov = Formats::FormatJSON::ValueFormatSerialize(v);

			if (ov.length() != 0) {
				if (start) out += ",";
				else start = true;

				out += ov;
			}
		}

		out += "}";

		return out;
	}

	bool s = value.IsString();
	String val = value.ToString();

	if (s) {
		out += "\"";

		int i = 0;
		int size = val.length();
		char c;

		while (i < size) {
			c = val.charAt(i);

			switch (c) {
				case '\"': out += "\\\""; break;
				case '\\': out += "\\\\"; break;
				case '/': out += "\\/"; break;
				case '\b': out += "\\b"; break;
				case '\f': out += "\\f"; break;
				case '\n': out += "\\n"; break;
				case '\r': out += "\\r"; break;
				case '\t': out += "\\t"; break;

				default:
					if (c < 0x20) {
						static const char hex[] = "0123456789ABCDEF";
						out += "\\u00";
						out += hex[(c >> 4) & 0xF];
						out += hex[c & 0xF];
					}
					else {
						out += c;
					}
					break;
			}

			++i;
		}

		out += "\"";
	}
	else out += val;

	return out;
}

Value Formats::FormatJSON::ValueFormatDeserialize (const String &raw) {
	int pos = 0;

	return Formats::FormatJSON::_parseValue(raw, pos);
}