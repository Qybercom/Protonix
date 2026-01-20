#pragma once

#include <Arduino.h>

namespace Qybercom {
	class Raw {
		private:
			byte* _data;
			int _size;

		public:
			Raw () : _data(0), _size(0) {}

			template<typename T>
			explicit Raw (const T &obj) : _size(sizeof(T)) {
				_data = new byte[_size];

				memcpy(_data, &obj, _size);
			}

			Raw (const Raw &other) : _size(other._size) {
				_data = (_size) ? new byte[_size] : 0;

				if (_size)
					memcpy(_data, other._data, _size);
			}

			Raw &operator=(const Raw &other) {
				if (this == &other) return *this;

				delete[] _data;
				_size = other._size;
				_data = (_size) ? new byte[_size] : 0;

				if (_size)
					memcpy(_data, other._data, _size);

				return *this;
			}

			~Raw () { delete[] _data; }

			void* Get () const { return _data; }
			int Size () const { return _size; }
			bool Valid () const { return _data != 0; }
			
			template<typename T>
			T As () const {
				if (!Valid() || Size() != sizeof(T)) {
					T t{};
					return t;
				}

				T t;

				memcpy(&t, _data, sizeof(T));

				return t;
			}
	};
}