#pragma once

#include <Arduino.h>

#include "Types/Raw.h"

namespace Qybercom {
	class Value;

	class IValueType {
		public:
			virtual void ValueTypeDeserialize (const String &raw) = 0;
			virtual String ValueTypeSerialize () const = 0;

			virtual ~IValueType () {}
	};

	class IValueFormat {
		public:
			virtual String ValueFormatMIME ();
			virtual String ValueFormatSerialize (Value &value) = 0;
			virtual Value ValueFormatDeserialize (const String &raw) = 0;
	};

	class Value {
		public:
			enum TYPE { UNDEFINED, BOOL, INT, FLOAT, STRING, OBJECT, ARRAY, RAW } _type = UNDEFINED;

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

			void _allocate ();

		public:
			Value ();
			Value (bool v);
			Value (short v);
			Value (unsigned short v);
			Value (int v);
			Value (unsigned int v);
			Value (long v);
			Value (unsigned long v);
			Value (float v);
			Value (double v);
			Value (const char* s);
			Value (String s);
			Value (const Value &other);

			~Value ();

			Value &operator= (bool v);
			Value &operator= (short v);
			Value &operator= (unsigned short v);
			Value &operator= (int v);
			Value &operator= (unsigned int v);
			Value &operator= (long v);
			Value &operator= (unsigned long v);
			Value &operator= (float v);
			Value &operator= (double v);
			Value &operator= (const char* s);
			Value &operator= (String s);
			Value &operator= (const Value &other);

			Value &operator[] (const char* key);
			Value &operator[] (const String &key);
			Value &operator[] (int index);

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
			bool HasKey (String key) const;
			int Count () const;
			int Capacity () const;

			bool Contains (String key) const;

			Value &Add (const Value &b);
			Value &Clear ();

			Iterator begin ();
			Iterator end ();

			static Value Object ();
			static Value Array ();

			String Serialize (IValueFormat* format);
			static Value Deserialize (IValueFormat* format, String raw);

			void Dump (int indent = 0);
	};
}