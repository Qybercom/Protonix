#pragma once

#include <Arduino.h>

namespace Qybercom {
	namespace Types {
		class Raw {
			private:
				int _size;
				char* _data;

			public:
				Raw ();
				template<typename T>
				explicit Raw (const T &obj) {
					_size = sizeof(T);
					_data = new char[_size];

					if (_size != 0)
						memcpy(_data, &obj, _size);
				}
				Raw (const Raw &other);
				Raw (Raw&& other) noexcept;
				Raw &operator= (const Raw &other);
				Raw &operator= (Raw&& other) noexcept;
				~Raw ();

				void* Get () const;
				int Size () const;
				bool Valid () const;

				template<typename T>
				T As () const {
					T t;

					if (Valid())
						memcpy(&t, _data, sizeof(T));

					return t;
				}

				static Raw* Copy (Raw *raw);
		};
	}
}