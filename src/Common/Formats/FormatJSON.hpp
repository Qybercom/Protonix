#pragma once

#include <Arduino.h>

#include "../Data/index.h"

namespace Qybercom {
	namespace Formats {
		class FormatJSON : public IBucketFormat {
			private:
				static void _skip (const String &s, int &pos) {
					while (pos < s.length() && (s[pos] == ' ' || s[pos] == '\n' || s[pos] == '\r' || s[pos] == '\t')) pos++;
				}

				static Bucket _parseValue (const String &s, int &pos) {
					_skip(s, pos);
					if (pos >= s.length()) return Bucket::Null();

					char c = s[pos];
					if (c == '{') return _parseObject(s, pos);
					if (c == '[') return _parseArray(s, pos);
					if (c == '"') return Bucket::Value(_parseString(s, pos));
					if (isDigit(c) || c == '-') return Bucket::Value(_parseNumber(s, pos));
					if (s.substring(pos, pos + 4) == "true") { pos += 4; return Bucket::Value(true); }
					if (s.substring(pos, pos + 5) == "false") { pos += 5; return Bucket::Value(false); }
					if (s.substring(pos, pos + 4) == "null") { pos += 4; return Bucket::Null(); }

					return Bucket::Null();
				}

				static Bucket _parseObject (const String &s, int &pos) {
					Bucket obj = Bucket::Object();
					pos++; // skip '{'
					_skip(s, pos);

					while (pos < s.length() && s[pos] != '}') {
						String key = _parseString(s, pos);

						_skip(s, pos);
						if (s[pos] == ':') pos++;
						Bucket value = _parseValue(s, pos);
						obj[key] = value;

						_skip(s, pos);
						if (s[pos] == ',') pos++;

						_skip(s, pos);
					}

					pos++; // skip '}'

					return obj;
				}

				static Bucket _parseArray (const String &s, int &pos) {
					Bucket arr = Bucket::Array();
					pos++; // skip '['
					_skip(s, pos);

					while (pos < s.length() && s[pos] != ']') {
						Bucket value = _parseValue(s, pos);
						arr.Add(value);
						_skip(s, pos);

						if (s[pos] == ',') pos++;
						_skip(s, pos);
					}

					pos++; // skip ']'

					return arr;
				}

				static String _parseString (const String &s, int &pos) {
					pos++; // skip opening "
					String str;
					int length = s.length();

					while (pos < length && s[pos] != '"') {
						str += s[pos++];
					}

					pos++; // skip closing "

					return str;
				}

				static int _parseNumber (const String &s, int &pos) {
					String num;
					int length = s.length();

					while (pos < length && (isDigit(s[pos]) || s[pos] == '.' || s[pos] == '-')) {
						num += s[pos++];
					}

					return num.indexOf('.') >= 0 ? num.toFloat() : num.toInt();
				}

			public:
				String BucketFormatSerialize (Bucket &bucket) {
					String out = "";

					if (bucket.HasKey())
						out += "\"" + bucket.Key() + "\":";

					if (bucket.IsValue()) {
						Value &v = bucket.AsValue();
						bool s = v.IsString();

						out += (s ? "\"" : "") + v.ToString() + (s ? "\"" : "");
					}

					if (bucket.IsNull())
						out += "null";

					if (bucket.IsArray()) {
						out += "[";

						for (Bucket &v : bucket)
							out += BucketFormatSerialize(v) + String(v.End() ? "" : ",");

						out += "]";
					}

					if (bucket.IsObject()) {
						out += "{";

						for (Bucket &v : bucket)
							out += BucketFormatSerialize(v) + String(v.End() ? "" : ",");

						out += "}";
					}

					return out;
				}

				Bucket BucketFormatDeserialize (const String &raw) {
					int pos = 0;

					return _parseValue(raw, pos);
				}
		};
	}
}