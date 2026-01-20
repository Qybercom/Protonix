#pragma once

#include <Arduino.h>

#include "Raw.hpp"

namespace Qybercom {
	class IValue {
		public:
			virtual void ValueDeserialize (const String &raw) = 0;
			virtual String ValueSerialize () const = 0;

			virtual ~IValue () {}
	};

	class Value {
		public:
			enum TYPE {
				UNDEFINED,
				NULLPTR,
				BOOL,
				INT,
				FLOAT,
				STRING,
				RAW
			};

		private:
			TYPE _type;

			union {
				bool _v_bool;
				long _v_int;
				double _v_float;
			};

			String _v_string;
			Raw _v_raw;

		public:
			Value ();

			Value (decltype(nullptr) v);
			Value (bool v);
			Value (int v);
			Value (long v);
			Value (float v);
			Value (double v);
			Value (const char* v);
			Value (const String &v);
			Value (const Raw &v);
			template<typename T>
			Value (const T &value) {
				IValue *check = static_cast<IValue*>(const_cast<T*>(&value));
				(void)check;

				_v_string = value.ValueSerialize();
			}

			Value &Set (decltype(nullptr) v);
			Value &Set (bool v);
			Value &Set (int v);
			Value &Set (long v);
			Value &Set (float v);
			Value &Set (double v);
			Value &Set (const String &v);
			Value &Set (const Raw &v);

			Value &operator= (decltype(nullptr) v);
			Value &operator= (bool v);
			Value &operator= (int v);
			Value &operator= (long v);
			Value &operator= (float v);
			Value &operator= (double v);
			Value &operator= (const char* v);
			Value &operator= (const String &v);
			Value &operator= (const Raw &v);
			template<typename T>
			Value &operator= (const T &value) {
				IValue *check = static_cast<IValue*>(const_cast<T*>(&value));
				(void)check;

				_v_string = value.ValueSerialize();

				return *this;
			}

			operator bool () const;
			operator int () const;
			operator long () const;
			operator float () const;
			operator double () const;
			operator String () const;
			operator Raw () const;

			Value &Clear ();
			TYPE Type () const;
			String TypeName () const;
			String ToString () const;
			String Trace (bool type = false);
			bool Equals (const Value &other) const;

			bool IsUndefined () const;
			bool IsNull () const;
			bool IsInt () const;
			bool IsFloat () const;
			bool IsBool () const;
			bool IsString () const;
			bool IsRaw () const;

			bool IsNumeric () const;

			template<typename T>
			T As () const {
				IValue *check = static_cast<T*>(nullptr);
				(void)check;

				T value;

				value.ValueDeserialize(_v_string);

				return value;
			}

			bool operator== (const Value &other) const;
			bool operator!= (const Value &other) const;
			bool operator< (const Value &other) const;
			bool operator> (const Value &other) const;
			bool operator<= (const Value &other) const;
			bool operator>= (const Value &other) const;

			// comparison BOOL
			bool operator== (bool v) const;
			bool operator!= (bool v) const;

			// comparison INT
			bool operator== (int v) const;
			bool operator!= (int v) const;
			bool operator< (int v) const;
			bool operator> (int v) const;
			bool operator<= (int v) const;
			bool operator>= (int v) const;
			bool operator== (long v) const;
			bool operator!= (long v) const;
			bool operator< (long v) const;
			bool operator> (long v) const;
			bool operator<= (long v) const;
			bool operator>= (long v) const;

			// comparison FLOAT
			bool operator== (float v) const;
			bool operator!= (float v) const;
			bool operator< (float v) const;
			bool operator> (float v) const;
			bool operator<= (float v) const;
			bool operator>= (float v) const;
			bool operator== (double v) const;
			bool operator!= (double v) const;
			bool operator< (double v) const;
			bool operator> (double v) const;
			bool operator<= (double v) const;
			bool operator>= (double v) const;

			// comparison STRING
			bool operator== (const String &v) const;
			bool operator!= (const String &v) const;
			bool operator< (const String &v) const;
			bool operator> (const String &v) const;
			bool operator<= (const String &v) const;
			bool operator>= (const String &v) const;
			bool operator== (const char* v) const;
			bool operator!= (const char* v) const;
			bool operator< (const char* v) const;
			bool operator> (const char* v) const;
			bool operator<= (const char* v) const;
			bool operator>= (const char* v) const;
	};
}