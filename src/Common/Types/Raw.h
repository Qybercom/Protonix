#pragma once

#include <Arduino.h>

namespace Qybercom {
	namespace Types {
		class Raw {
			private:
				char* _data;
				int _size;

			public:
				Raw ();
				template<typename T>
				explicit Raw (const T &obj) : _size(sizeof(T)) {
					_data = new char[_size];

					memcpy(_data, &obj, _size);
				}
				Raw (const Raw &other);
				Raw &operator= (const Raw &other);
				~Raw ();

				void* Get () const;
				int Size () const;
				bool Valid () const;

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
}