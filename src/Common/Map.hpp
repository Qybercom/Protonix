#pragma once

#include <Arduino.h>

#include "Data.hpp"
#include "List.hpp"

namespace Qybercom {
	class Map {
		public:
			struct Entry {
				String Key;
				Any Value;

				Entry (String key, Any value)
					: Key(key), Value(value) { }

				bool operator== (Entry &other) const {
					return Key == other.Key;
				}
			};

		private:
			List<Entry*> _items;

		public:
			Map () { }

			template<typename T>
			Map &Set (String key, const T &value) {
				bool exists = false;

				for (Entry* entry : this->_items) {
					if (entry->Key != key) continue;

					entry->Value = Any(value);
					exists = true;
				}

				if (!exists)
					this->_items.Add(new Entry(key, Any(value)));

				return *this;
			}

			bool Exists (String key) {
				for (Entry* entry : this->_items)
					if (entry->Key == key) return true;

				return false;
			}

			template<typename T>
			T Get (String key, T fallback) {
				for (Entry* entry : this->_items)
					if (entry->Key == key)
						return entry->Value.As<T>(fallback);

				return fallback;
			}

			/*template<typename T>
			T* Get (String key) {
				for (Entry* entry : this->_items)
					if (entry->Key == key)
						return entry->Value.AsPtr<T>();

				return nullptr;
			}*/

			bool Remove (String key) {
				/*int idx = _indexOf(key);
				if (idx == -1) return false;

				_items.Remove(idx);*/

				return true;
			}

			unsigned int Count () const {
				return _items.Count();
			}

			void Clear () {
				_items.Clear();
			}
	};
}