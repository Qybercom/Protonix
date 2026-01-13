#pragma once

#include <Arduino.h>

#include "../Utils.hpp"
#include "List.hpp"
#include "Value.hpp"

namespace Qybercom {
	class Bucket;

	class IBucketFormat {
		public:
			virtual String BucketFormatSerialize (Bucket &bucket) = 0;
			virtual Bucket BucketFormatDeserialize (const String &raw) = 0;
	};

	class Bucket {
		public:
			enum TYPE {
				UNDEFINED,
				NULLPTR,
				VALUE,
				OBJECT,
				ARRAY
			};

			struct Entry {
				String Key;
				Bucket* Value;

				Entry () : Key(""), Value(nullptr) { }
				Entry (const String &k) : Key(k), Value(nullptr) { }
				Entry (const String &k, Bucket* v) : Key(k), Value(v) { }
			};

			class Iterator {
				private:
					Bucket* _owner;
					int _index;

				public:
					Iterator (Bucket* owner, int index)
						: _owner(owner), _index(index) { }

					bool operator!= (const Iterator &other) const {
						return _index != other._index;
					}

					Iterator &operator++ () {
						_index++;
						return *this;
					}

					Bucket &operator* () {
						if (_owner->_type == TYPE::OBJECT) {
							Entry &e = (*_owner->_object)[_index];
							e.Value->_key = &e.Key;
							e.Value->_end = (_index == _owner->_object->Count() - 1);

							return *e.Value;
						}

						if (_owner->_type == TYPE::ARRAY) {
							Bucket &b = (*_owner->_array)[_index];
							b._key = nullptr;
							b._end = (_index == _owner->_array->Count() - 1);

							return b;
						}

						return *_owner;
					}
			};

		private:
			TYPE _type;
			const String* _key;
			bool _end;

			union {
				Qybercom::Value* _value;
				List<Entry>* _object;
				List<Bucket>* _array;
			};

			void _clear () {
				if (_type == TYPE::NULLPTR && _value != nullptr) { delete _value; _value = nullptr; }
				if (_type == TYPE::VALUE && _value != nullptr) { delete _value; _value = nullptr; }
				if (_type == TYPE::OBJECT && _object != nullptr) { delete _object; _object = nullptr; }
				if (_type == TYPE::ARRAY && _array != nullptr) { delete _array; _array = nullptr; }

				_type = TYPE::UNDEFINED;
				_key = nullptr;
				_end = true;
			}

			Bucket (TYPE type, Value* v = nullptr) : _type(type) {
				_key = nullptr;
				_end = true;

				_value = v;
				_object = nullptr;
				_array = nullptr;

				if (type == TYPE::OBJECT)
					_object = new List<Entry>();

				if (type == TYPE::ARRAY)
					_array = new List<Bucket>();
			}

			Bucket* _bucket (const String &key) {
				for (Entry &entry : (*_object))
					if (entry.Key == key)
						return entry.Value;

				return nullptr;
			}

			Bucket &_bucketValue (TYPE t, Value* v) {
				_clear();
				_type = t;
				_value = v;

				return *this;
			}

			Bucket &_associate (const String &key) {
				if (_type != TYPE::OBJECT) {
					_clear();
					_type = TYPE::OBJECT;
					_object = new List<Entry>();
				}

				Bucket* v = _bucket(key);
				if (v != nullptr) return *v;

				Entry e(key, new Bucket());
				_object->Add(e);

				return *_object->Last().Value;
			}

		public:
			Bucket () : _type(TYPE::UNDEFINED) {
				_key = nullptr;
				_end = true;

				_value = nullptr;
				_object = nullptr;
				_array = nullptr;
			}

			~Bucket () {
				_clear();
			}

			Bucket (const Bucket &other) : _type(TYPE::UNDEFINED) {
				*this = other;
			}

			static Bucket Null () {
				return Bucket(TYPE::NULLPTR);
			}

			static Bucket Object () {
				return Bucket(TYPE::OBJECT);
			}

			static Bucket Array () {
				return Bucket(TYPE::ARRAY);
			}

			static Bucket Value (const Qybercom::Value &value) {
				return Bucket(TYPE::VALUE, new Qybercom::Value(value));
			}

			void Add (const Bucket &value) {
				if (_type != TYPE::ARRAY) {
					_clear();
					_type = TYPE::ARRAY;
					_array = new List<Bucket>();
				}

				_array->Add(value);
			}

			bool HasKey () const {
				return _key != nullptr;
			}

			const String Key () const {
				return _key == nullptr ? "" : *_key;
			}

			bool End () const {
				return _end;
			}

			Iterator begin () {
				if (_type == TYPE::OBJECT)
					return Iterator(this, 0);

				if (_type == TYPE::ARRAY)
					return Iterator(this, 0);

				return Iterator(this, 0);
			}

			Iterator end () {
				if (_type == TYPE::OBJECT)
					return Iterator(this, _object->Count());

				if (_type == TYPE::ARRAY)
					return Iterator(this, _array->Count());

				return Iterator(this, 0);
			}

			Bucket &operator= (decltype(nullptr) v) {
				return _bucketValue(TYPE::NULLPTR, new Qybercom::Value(v));
			}

			Bucket &operator= (bool v) {
				return _bucketValue(TYPE::VALUE, new Qybercom::Value(v));
			}

			Bucket &operator= (int v) {
				return _bucketValue(TYPE::VALUE, new Qybercom::Value(v));
			}

			Bucket &operator= (long v) {
				return _bucketValue(TYPE::VALUE, new Qybercom::Value(v));
			}

			Bucket &operator= (float v) {
				return _bucketValue(TYPE::VALUE, new Qybercom::Value(v));
			}

			Bucket &operator= (double v) {
				return _bucketValue(TYPE::VALUE, new Qybercom::Value(v));
			}

			Bucket &operator= (const char* v) {
				return _bucketValue(TYPE::VALUE, new Qybercom::Value(v));
			}

			Bucket &operator= (const String &v) {
				return _bucketValue(TYPE::VALUE, new Qybercom::Value(v));
			}

			Bucket &operator= (const Qybercom::Value &v) {
				return _bucketValue(TYPE::VALUE, new Qybercom::Value(v));
			}

			Bucket &operator= (const Bucket &other) {
				if (this == &other)
					return *this;

				_clear();

				_type = other._type;

				switch (_type) {
					case TYPE::NULLPTR:
						_value = new Qybercom::Value(*other._value);
						break;

					case TYPE::VALUE:
						_value = new Qybercom::Value(*other._value);
						break;

					case TYPE::OBJECT:
						_object = new List<Entry>();

						for (Entry &entry : (*other._object))
							_object->Add(Entry(entry.Key, entry.Value));

						break;

					case TYPE::ARRAY:
						_array = new List<Bucket>();

						for (Bucket &bucket : (*other._array))
							_array->Add(bucket);

						break;

					default:
						break;
				}

				return *this;
			}

			Bucket &operator[] (const String &key) {
				return _associate(key);
			}

			Bucket &operator[] (const char* key) {
				String k = String(key);

				return _associate(k);
			}

			Bucket &operator[] (int index) {
				if (_type != TYPE::ARRAY) {
					_clear();
					_type = TYPE::ARRAY;
					_array = new List<Bucket>();
				}

				while (_array->Count() <= index) {
					_array->Add(Bucket());
				}

				return (*_array)[index];
			}
		
			operator bool () const {
				return _type == VALUE && _value != nullptr ? (bool)(*_value) : false;
			}

			operator int () const {
				return _type == VALUE && _value != nullptr ? (int)(*_value) : 0;
			}

			operator long () const {
				return _type == VALUE && _value != nullptr ? (long)(*_value) : 0;
			}

			operator float () const {
				return _type == VALUE && _value != nullptr ? (float)(*_value) : 0.0f;
			}

			operator double () const {
				return _type == VALUE && _value != nullptr ? (double)(*_value) : 0.0f;
			}

			operator String () const {
				return _type == VALUE && _value != nullptr ? (String)(*_value) : "";
			}

			bool IsUndefined () const { return _type == TYPE::UNDEFINED; }
			bool IsNull () const { return _type == TYPE::NULLPTR; }
			bool IsValue () const { return _type == TYPE::VALUE; }
			bool IsObject () const { return _type == TYPE::OBJECT; }
			bool IsArray () const { return _type == TYPE::ARRAY; }

			Qybercom::Value &AsValue () {
				if (_type != TYPE::VALUE) {
					_clear();
					_type = TYPE::VALUE;
					_value = new Qybercom::Value();
				}

				return *_value;
			}

			List<Entry> &AsObject () {
				if (_type != TYPE::OBJECT) {
					_clear();
					_type = TYPE::OBJECT;
					_object = new List<Entry>();
				}

				return *_object;
			}

			List<Bucket> &AsArray () {
				if (_type != TYPE::ARRAY) {
					_clear();
					_type = TYPE::ARRAY;
					_array = new List<Bucket>();
				}

				return *_array;
			}

			String Serialize (IBucketFormat *format) {
				return format == nullptr ? "" : format->BucketFormatSerialize(*this);
			}
		
			void Dump (int indent = 0) {
				Serial.print(Qybercom::indent(indent));

				if (HasKey()) {
					Serial.print(Key());
					Serial.print(": ");
				}

				if (IsValue()) {
					Serial.println(AsValue().Trace());
					return;
				}

				if (IsNull()) {
					Serial.println("null");
					return;
				}

				if (IsArray()) {
					Serial.println("[");

					for (Bucket &v : *this)
						v.Dump(indent + 1);

					Serial.print(Qybercom::indent(indent));

					Serial.println("]");

					return;
				}

				if (IsObject()) {
					Serial.println("{");

					for (Bucket &v : *this)
						v.Dump(indent + 1);

					Serial.print(Qybercom::indent(indent));

					Serial.println("}");
				}
			}
	};
}