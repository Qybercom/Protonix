#pragma once

#include <Arduino.h>

namespace Qybercom {
	struct Vector2 {
		int X;
		int Y;

		Vector2 (int x = 0, int y = 0) : X(x), Y(y) {}

		String Serialize () {
			return String(X) + "," + String(Y);
		}
	};

	struct Vector3 {
		int X;
		int Y;
		int Z;

		Vector3 (int x = 0, int y = 0, int z = 0) : X(x), Y(y), Z(z) {}

		String Serialize () {
			return String(X) + "," + String(Y) + "," + String(Z);
		}
	};

	struct Vector4 {
		int A;
		int B;
		int C;
		int D;

		Vector4 (int a = 0, int b = 0, int c = 0, int d = 0) : A(a), B(b), C(c), D(d) {}

		String Serialize () {
			return String(A) + "," + String(B) + "," + String(C) + "," + String(D);
		}
	};

	class Any {
		public:
			void* Data;
			size_t Size;
			bool IsPointer;
			bool IsPOD;

			Any () : Data(nullptr), Size(0), IsPointer(false), IsPOD(false) {}

			Any (const void* value, size_t size) {
				Size = size;
				IsPOD = true;
				IsPointer = false;
				Data = malloc(size);

				if (value)
					memcpy(Data, value, size);
			}

			template<typename T>
			Any (const T &value) {
				Data = new T(value);
				Size = sizeof(T);
				IsPointer = false;
				IsPOD = false;
			}

			template<typename T>
			Any (T* value) {
				Data = new T(*value);
				Size = sizeof(T);
				IsPointer = true;
				IsPOD = false;
			}

			Any (const Any &other) {
				Size = other.Size;
				IsPointer = other.IsPointer;
				IsPOD = other.IsPOD;

				if (IsPOD) {
					Data = malloc(Size);
					memcpy(Data, other.Data, Size);
				}
				else {
					Data = new char[Size];
					memcpy(Data, other.Data, Size);
				}
			}

			Any &operator= (const Any &other) {
				if (this != &other) {
					if (Data) {
						if (IsPOD) free(Data);
						else delete[] static_cast<char*>(Data);
					}

					Size = other.Size;
					IsPointer = other.IsPointer;
					IsPOD = other.IsPOD;

					if (IsPOD) {
						Data = malloc(Size);
						memcpy(Data, other.Data, Size);
					}
					else {
						Data = new char[Size];
						memcpy(Data, other.Data, Size);
					}
				}

				return *this;
			}

			Any (Any &&other) noexcept {
				Data = other.Data;
				Size = other.Size;
				IsPointer = other.IsPointer;
				IsPOD = other.IsPOD;

				other.Data = nullptr;
				other.Size = 0;
				other.IsPointer = false;
				other.IsPOD = false;
			}

			Any &operator= (Any &&other) noexcept {
				if (this != &other) {
					if (Data) {
						if (IsPOD) free(Data);
						else delete[] static_cast<char*>(Data);
					}

					Data = other.Data;
					Size = other.Size;
					IsPointer = other.IsPointer;
					IsPOD = other.IsPOD;

					other.Data = nullptr;
					other.Size = 0;
					other.IsPointer = false;
					other.IsPOD = false;
				}

				return *this;
			}

			~Any () {
				if (!Data) return;

				if (IsPOD) free(Data);
				else delete[] static_cast<char*>(Data);

				Data = nullptr;
				Size = 0;
			}

			template<typename T>
			T* AsPtr () {
				return static_cast<T*>(Data);
			}

			template<typename T>
			T As (T fallback) {
				T* out = static_cast<T*>(Data);

				return out == nullptr ? fallback : *out;
			}
	};

	struct Pixel {
		unsigned long Index;
		byte R;
		byte G;
		byte B;

		Pixel (byte r = 0, byte g = 0, byte b = 0, unsigned long index = 0)
			: Index(index), R(r), G(g), B(b) { }
	};
}