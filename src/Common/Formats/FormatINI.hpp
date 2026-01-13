#pragma once

namespace Qybercom {
	class FormatINI {
		public:
			struct Entry {
				String Section;
				String Key;
				String Value;

				Entry (String section = "", String key = "", String value = "")
					: Section(section), Key(key), Value(value) { }
			};

		private:
			List<Entry*> _entries;

		public:
			String Serialize () {
				String out = "";
				String section = "";

				for (Entry* entry : _entries) {
					if (entry->Section != section) {
						if (out.length() != 0)
							out += "\r\n";

						out += "[";
						out += entry->Section;
						out += "]\r\n";

						section = entry->Section;
					}

					out += entry->Key;
					out += " = ";
					out += entry->Value;
					out += "\r\n";
				}

				out.trim();

				return out;
			}
			
			FormatINI &Deserialize (const String& text) {
				Clear();

				String section;

				int len = text.length();
				int pos = 0;

				while (pos < len) {
					int end = text.indexOf('\n', pos);
					if (end == -1) end = len;

					String line = text.substring(pos, end);
					pos = end + 1;

					line.trim();
					if (!line.length()) continue;
					char c0 = line[0];
					if (c0 == ';' || c0 == '#') continue;

					if (c0 == '[' && line.endsWith("]")) {
						section = line.substring(1, line.length() - 1);
						section.trim();

						continue;
					}

					int eq = line.indexOf('=');
					if (eq == -1) continue;

					Entry* e = new Entry(
						section,
						line.substring(0, eq),
						line.substring(eq + 1)
					);

					e->Key.trim();
					e->Value.trim();

					_entries.Add(e);
				}

				return *this;
			}

			Entry* Get (const String& section, const String& key) {
				for (Entry* entry : _entries)
					if (entry->Section == section && entry->Key == key)
						return entry;

				return nullptr;
			}

			String GetString (const String& section, const String& key, const String& fallback = "") {
				Entry* entry = Get(section, key);

				return entry == nullptr ? fallback : entry->Value;
			}

			int GetInt (const String& section, const String& key, int fallback = 0) {
				Entry* entry = Get(section, key);

				return entry == nullptr ? fallback : entry->Value.toInt();
			}

			float GetFloat (const String& section, const String& key, float fallback = 0.0) {
				Entry* entry = Get(section, key);

				return entry == nullptr ? fallback : entry->Value.toFloat();
			}

			bool GetBool (const String& section, const String& key, bool fallback = false) {
				Entry* entry = Get(section, key);

				if (entry != nullptr) {
					String value = entry->Value;
					value.toLowerCase();

					if (value == "true") return true;
					if (value == "false") return false;
				}

				return fallback;
			}

			FormatINI &Set (const String& section, const String& key, String value) {
				for (Entry* entry : _entries) {
					if (entry->Section == section && entry->Key == key) {
						entry->Value = value;

						return *this;
					}
				}

				_entries.Add(new Entry(section, key, value));

				return *this;
			}

			FormatINI &Set (const String& section, const String& key, int value) {
				return Set(section, key, String(value));
			}

			FormatINI &Set (const String& section, const String& key, bool value) {
				return Set(section, key, String(value ? "true" : "false"));
			}

			void Clear () {
				_entries.Clear();
			}

			~FormatINI () { Clear(); }
	};
}