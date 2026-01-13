#pragma once

#include <Arduino.h>

#include "Any.hpp"
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

			List<Entry*> &Items () {
				return _items;
			}

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

			template<typename T>
			T* Get (String key) {
				for (Entry* entry : this->_items)
					if (entry->Key == key)
						return entry->Value.As<T>();

				return nullptr;
			}

			bool Remove (String key) {
				unsigned int count = _items.Count();

				for (unsigned int i = 0; i < count; i++) {
					if (_items[i]->Key == key) {
						_items.Remove(i);

						return true;
					}
				}

				return false;
			}

			unsigned int Count () const {
				return _items.Count();
			}

			void Clear () {
				_items.Clear();
			}

			using Iterator = List<Entry*>::Iterator;

			Iterator begin () { return _items.begin(); }
			Iterator end () { return _items.end(); }

			~Map () { Clear(); }
	};
}