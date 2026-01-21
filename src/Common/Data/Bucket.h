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
			//virtual Bucket BucketFormatDeserialize (const char* raw) = 0;
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
				//const char* Key;
				Bucket* Value;

				Entry ();
				Entry (const String &k);
				Entry (const String &k, Bucket* v);
				//Entry (const char* k);
				//Entry (const char* k, Bucket* v);

				~Entry () {
					/*if (Value != nullptr) {
						delete Value;
						Value = nullptr;
					}*/
				}
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
			//const char* _key;
			bool _end;

			Qybercom::Value* _value;
			List<Entry>* _object;
			List<Bucket>* _array;

			Bucket (TYPE type, Value* v = nullptr);
			void _clear ();
			Bucket* _bucket (const String &key);
			//Bucket* _bucket (const char* key);
			Bucket &_bucketValue (TYPE t, Value* v);
			Bucket &_associate (const String &key);
			//Bucket &_associate (const char* key);

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
			bool HasKey (String key) const;
			const String Key () const;
			//const char* Key () const;
			bool End () const;

			Iterator begin ();
			Iterator end ();

			Bucket &operator= (decltype(nullptr) v);
			Bucket &operator= (bool v);
			Bucket &operator= (short v);
			Bucket &operator= (unsigned short v);
			Bucket &operator= (int v);
			Bucket &operator= (unsigned int v);
			Bucket &operator= (long v);
			Bucket &operator= (unsigned long v);
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
			operator short () const;
			operator unsigned short () const;
			operator int () const;
			operator unsigned int () const;
			operator long () const;
			operator unsigned long () const;
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