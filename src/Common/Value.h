#pragma once

#include <Arduino.h>

#include "Types/Raw.h"

namespace Qybercom {
	class Value;

	class IValueType {
		public:
			virtual void ValueTypeDeserialize (const String &raw) = 0;
			virtual String ValueTypeSerialize () const = 0;

			virtual ~IValueType () { }
	};

	class IValueFormat {
		public:
			virtual String ValueFormatMIME () = 0;
			virtual String ValueFormatSerialize (Value &value) = 0;
			virtual Value ValueFormatDeserialize (const String &raw) = 0;

			virtual ~IValueFormat () { }
	};

	class IValueListener {
		public:
			virtual void ValueListenerGet (Value &value) { }
			virtual void ValueListenerSet (Value &value) { }

			virtual ~IValueListener () { }
	};

	class Value {
		public:
			enum TYPE { UNDEFINED, NULLPTR, BOOL, INT, FLOAT, STRING, OBJECT, ARRAY, RAW } _type = UNDEFINED;

			class Iterator {
				private:
					Value* _ptr;

				public:
					Iterator (Value* ptr);
					Value &operator* ();
					Iterator &operator++ ();
					bool operator!= (const Iterator &other) const;
			};

		private:
			union {
				bool BOOL;
				int INT;
				float FLOAT;
				char* STRING;
				struct {
					Value* items;
					int count;
					int capacity;
				} COLLECTION;
				Types::Raw* RAW;
			} _value;

			char* _key = nullptr;
			IValueListener* _listener = nullptr;

			void _allocate ();

		public:
			Value ();
			Value (decltype(nullptr) value);
			Value (bool value);
			Value (short value);
			Value (unsigned short value);
			Value (int value);
			Value (unsigned int value);
			Value (long value);
			Value (unsigned long value);
			Value (float value);
			Value (double value);
			Value (const char* value);
			Value (const String &value);
			Value (const Value &value);

			~Value ();

			operator decltype(nullptr) () const;
			operator bool () const;
			operator short () const;
			operator unsigned short () const;
			operator int () const;
			operator unsigned int () const;
			operator long () const;
			operator unsigned long () const;
			operator float () const;
			operator double () const;
			operator char* () const;
			operator String () const;
			template <typename T>
			T As () const {
				return static_cast<T>(*this);
			}

			Value &operator[] (const char* key);
			Value &operator[] (const String &key);
			Value &operator[] (int key);

			Value &operator= (decltype(nullptr) value);
			Value &operator= (bool value);
			Value &operator= (short value);
			Value &operator= (unsigned short value);
			Value &operator= (int value);
			Value &operator= (unsigned int value);
			Value &operator= (long value);
			Value &operator= (unsigned long value);
			Value &operator= (float value);
			Value &operator= (double value);
			Value &operator= (const char* value);
			Value &operator= (const String &value);
			Value &operator= (const Value &value);

			Value &Get (const char* key);
			Value &Get (const String &key);
			Value &Get (int key);
			template<typename T>
			T Get (const char* key) {
				return (*this)[key];
			}
			template<typename T>
			T Get (const String &key) {
				return (*this)[key.c_str()];
			}

			Value &Set (decltype(nullptr) value);
			Value &Set (bool value);
			Value &Set (short value);
			Value &Set (unsigned short value);
			Value &Set (int value);
			Value &Set (unsigned int value);
			Value &Set (long value);
			Value &Set (unsigned long value);
			Value &Set (float value);
			Value &Set (double value);
			Value &Set (const char* value);
			Value &Set (const String &value);
			Value &Set (const Value &value);
			Value &Set (const char* key, const Value &value);
			Value &Set (const String &key, const Value &value);
			template<typename T>
			Value &Set (const char* key, T value) {
				(*this)[key] = value;

				return *this;
			}
			template<typename T>
			Value &Set (const String &key, T value) {
				(*this)[key.c_str()] = value;

				return *this;
			}
			Value &Set (int key, const Value &value);
			template<typename T>
			Value &Set (int key, T value) {
				(*this)[key] = value;

				return *this;
			}

			Value &Replace (const Value &value);
			Value &Replace (const char* key, const Value &value);
			Value &Replace (const String &key, const Value &value);
			Value &Replace (int key, const Value &value);

			bool IsUndefined () const;
			bool IsNull () const;
			bool IsBool () const;
			bool IsInt () const;
			bool IsFloat () const;
			bool IsString () const;
			bool IsObject () const;
			bool IsArray () const;
			bool IsRaw () const;
			bool IsNumeric () const;

			TYPE Type () const;
			String TypeName () const;
			String ToString () const;
			String Trace (bool type = false);
			String Key () const;
			bool HasKey () const;
			int Count () const;
			int Capacity () const;

			bool Contains (const char* key) const;
			bool Contains (const String &key) const;
			bool Contains (int key) const;

			Value &Add (const Value &b);
			Value &Clear ();
			Value &Listener (IValueListener* listener);

			Iterator begin ();
			Iterator end ();

			static Value Object ();
			static Value Array ();

			String Serialize (IValueFormat* format);
			//static Value Deserialize (IValueFormat* format, const char* raw);
			static Value Deserialize (IValueFormat* format, const String &raw);

			void Dump (int indent = 0);
	};
}