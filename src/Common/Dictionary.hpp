#pragma once

#include "List.hpp"

namespace Qybercom {
	template<typename TKey, typename TValue>
	class Dictionary {
		public:
			struct Pair {
				TKey Key;
				TValue Value;

				Pair (const TKey &key, const TValue &value)
					: Key(key), Value(value) { }

				bool operator== (const Pair &other) const {
					return Key == other.Key;
				}
			};

		private:
			List<Pair> _items;

			int _index (const TKey &key) {
				for (unsigned int i = 0; i < _items.Count(); i++)
					if (_items[i].Key == key)
						return i;
				
				return -1;
			}

		public:
			Dictionary () { }

			bool Empty () const {
				return _items.Empty();
			}

			unsigned int Count () const {
				return _items.Count();
			}

			void Clear () {
				_items.Clear();
			}

			bool Exists (const TKey &key) {
				return _index(key) != -1;
			}

			Dictionary<TKey, TValue> &Set (const TKey &key, const TValue &value) {
				int idx = _index(key);

				if (idx != -1) _items[idx].Value = value;
				else _items.Add(Pair(key, value));

				return *this;
			}

			bool Remove (const TKey &key) {
				int idx = _index(key);
				if (idx == -1) return false;

				_items.Remove(idx);

				return true;
			}

			TValue &Get (const TKey &key) {
				int idx = _index(key);

				if (idx != -1)
					return _items[idx].Value;

				return *(TValue*) 0;
			}

			TValue* TryGet (const TKey &key) {
				int idx = _index(key);
				if (idx == -1) return nullptr;

				return &_items[idx].Value;
			}

			TKey &KeyAt (unsigned int index) {
				return _items[index].Key;
			}

			TValue &ValueAt (unsigned int index) {
				return _items[index].Value;
			}

			List<Pair> &Items () {
				return _items;
			}

			// dictionary[key]
			TValue &operator[] (const TKey &key) {
				int idx = _index(key);

				if (idx == -1) {
					_items.Add(Pair(key, TValue()));

					return _items.Last().Value;
				}

				return _items[idx].Value;
			}

			typename List<Pair>::Iterator begin () {
				return _items.begin();
			}

			typename List<Pair>::Iterator end () {
				return _items.end();
			}
	};
}