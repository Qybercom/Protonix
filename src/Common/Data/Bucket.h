#pragma once

#include <Arduino.h>

#include "../Utils.h"
#include "List.hpp"
#include "Value.h"

namespace Qybercom {
	class Bucket;

	class IBucketFormat {
		public:
			virtual String BucketFormatMIME ();
			virtual String BucketFormatSerialize (Bucket &bucket) = 0;
			virtual Bucket BucketFormatDeserialize (const String &raw) = 0;
	};

	class Bucket {
		public:
			enum TYPE {
				UNDEFINED,
				VALUE,
				OBJECT,
				ARRAY
			};

			struct Entry {
				String Key;
				Bucket* Value;

				Entry ();
				Entry (const String &k);
				Entry (const String &k, Bucket* v);
			};

			class Iterator {
				private:
					Bucket* _owner;
					int _index;

				public:
					Iterator (Bucket* owner, int index);
					bool operator!= (const Iterator &other) const;
					Iterator &operator++ ();
					Bucket &operator* ();
			};

		private:
			TYPE _type;
			const String* _key;
			bool _end;

			Qybercom::Value* _value;
			List<Entry>* _object;
			List<Bucket>* _array;

			Bucket (TYPE type, Value* v = nullptr);
			void _clear ();
			Bucket* _bucket (const String &key);
			Bucket &_bucketValue (TYPE t, Value* v);
			Bucket &_associate (const String &key);

		public:
			Bucket ();
			Bucket (const Bucket &other);

			~Bucket ();

			static Bucket Null ();
			static Bucket Value (const Qybercom::Value &value);
			static Bucket Object ();
			static Bucket Array ();

			void Add (const Bucket &value);
			bool HasKey () const;
			const String Key () const;
			bool End () const;

			Iterator begin ();
			Iterator end ();

			Bucket &operator= (decltype(nullptr) v);
			Bucket &operator= (bool v);
			Bucket &operator= (int v);
			Bucket &operator= (long v);
			Bucket &operator= (float v);
			Bucket &operator= (double v);
			Bucket &operator= (const char* v);
			Bucket &operator= (const String &v);
			Bucket &operator= (const Qybercom::Value &v);
			Bucket &operator= (const Bucket &other);
			template<typename T>
			Bucket &operator= (const T &value) {
				return _bucketValue(TYPE::VALUE, new Qybercom::Value(value));
			}

			Bucket &operator[] (const String &key);
			Bucket &operator[] (const char* key);
			Bucket &operator[] (int index);
		
			operator bool () const;
			operator int () const;
			operator long () const;
			operator float () const;
			operator double () const;
			operator String () const;

			bool IsUndefined () const;
			bool IsNull () const;
			bool IsValue () const;
			bool IsObject () const;
			bool IsArray () const;

			Qybercom::Value &AsValue ();
			List<Entry> &AsObject ();
			List<Bucket> &AsArray ();

			String Serialize (IBucketFormat *format);
			static Bucket Deserialize (IBucketFormat *format, String raw);
		
			void Dump (int indent = 0);
	};
}