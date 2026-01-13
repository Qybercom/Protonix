#pragma once

#include <Arduino.h>

#include "../Utils.hpp"
#include "../Data/index.h"

namespace Qybercom {
	namespace Formats {
		class FormatYAML : public IBucketFormat {
			private:
				static String _serialize (Bucket &bucket, int indent = 0, bool first = true) {
					String out = "";
					String ind = Qybercom::indent(indent);

					if (bucket.IsValue())
						out += ind + bucket.AsValue().ToString() + "\n";

					if (bucket.IsArray()) {
						for (Bucket &v : bucket)
							out += ind + "- " + String(v.IsValue()
								? ": " + v.AsValue().ToString() + "\n"
								: _serialize(v, indent + 1, false)
							);
					}

					if (bucket.IsObject()) {
						for (Bucket &v : bucket) {
							out += String(first ? ind : "") + v.Key() + String(v.IsValue()
								? ": " + v.AsValue().ToString() + "\n"
								: ":\n" + _serialize(v, indent + 1)
							);

							first = true;
						}
					}

					return out;
				}

				static void skipWhitespace (const String &s, int &pos) {
					while (pos < s.length() && (s[pos] == ' ' || s[pos] == '\t')) pos++;
				}

				static int countIndent (const String &s, int pos) {
					int cnt = 0;
					while (pos < s.length() && s[pos] == ' ') { cnt++; pos++; }
					return cnt;
				}

				static Bucket parseBlock (const String &s, int &pos, int currentIndent) {
					Bucket root = Bucket::Object();

					while (pos < s.length()) {
						int lineEnd = s.indexOf('\n', pos);
						if (lineEnd < 0) lineEnd = s.length();
						String line = s.substring(pos, lineEnd);
						int indent = countIndent(line, 0);

						if (indent < currentIndent) break;

						line.trim();
						if (line.length() && line[0] != '#') {
							if (line.startsWith("- ")) {
								// array element
								Bucket val = parseValue(line.substring(2));
								root.Add(val);
							}
							else {
								int colon = line.indexOf(':');

								if (colon > 0) {
									String key = line.substring(0, colon);
									String valStr = line.substring(colon+1);
									valStr.trim();

									if (valStr.length() == 0) {
										pos = lineEnd + 1;
										root[key] = parseBlock(s, pos, indent + 2);
										continue;
									}
									else {
										root[key] = parseValue(valStr);
									}
								}
							}
						}

						pos = lineEnd + 1;
					}

					return root;
				}

				static Bucket parseValue (const String &val) {
					String v = val;
					v.trim();

					if (v.equalsIgnoreCase("true")) return Bucket::Value(true);
					if (v.equalsIgnoreCase("false")) return Bucket::Value(false);
					if (v.equalsIgnoreCase("null")) return Bucket::Null();
					if (v.indexOf('.') >= 0) return Bucket::Value(v.toFloat());
					if ((v.length() && isDigit(v[0])) || v[0] == '-') return Bucket::Value(v.toInt());

					return Bucket::Value(v);
				}
			
			public:
				String BucketFormatSerialize (Bucket &bucket) {
					return _serialize(bucket, 0);
				}

				Bucket BucketFormatDeserialize (const String &raw) {
					int pos = 0;
					
					return parseBlock(raw, pos, 0);
				}
		};
	}
}