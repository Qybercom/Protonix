#include <Arduino.h>

#include "Bucket.h"

using namespace Qybercom;

Bucket::Entry::Entry () {
	Bucket::Entry::Key = "";
	Bucket::Entry::Value = nullptr;
}

Bucket::Entry::Entry (const String &k) {
	Bucket::Entry::Key = k;
	Bucket::Entry::Value = nullptr;
}

Bucket::Entry::Entry (const String &k, Bucket* v) {
	Bucket::Entry::Key = k;
	Bucket::Entry::Value = v;
}

Bucket::Iterator::Iterator (Bucket* owner, int index) {
	_owner = owner;
	_index = index;
}

bool Bucket::Iterator::operator!= (const Bucket::Iterator &other) const {
	return _index != other._index;
}

Bucket::Iterator &Bucket::Iterator::operator++ () {
	_index++;

	return *this;
}

Bucket &Bucket::Iterator::operator* () {
	if (_owner->_type == Bucket::TYPE::OBJECT) {
		Entry &e = (*_owner->_object)[_index];
		e.Value->_key = &e.Key;
		e.Value->_end = (_index == _owner->_object->Count() - 1);

		return *e.Value;
	}

	if (_owner->_type == Bucket::TYPE::ARRAY) {
		Bucket &b = (*_owner->_array)[_index];
		b._key = nullptr;
		b._end = (_index == _owner->_array->Count() - 1);

		return b;
	}

	return *_owner;
}

void Bucket::_clear () {
	if (_type == Bucket::TYPE::VALUE && _value != nullptr) { delete _value; _value = nullptr; }
	if (_type == Bucket::TYPE::OBJECT && _object != nullptr) { delete _object; _object = nullptr; }
	if (_type == Bucket::TYPE::ARRAY && _array != nullptr) { delete _array; _array = nullptr; }

	_type = Bucket::TYPE::UNDEFINED;
	_key = nullptr;
	_end = true;
}

Bucket::Bucket (Bucket::TYPE type, Qybercom::Value* v) {
	_type = type;
	_key = nullptr;
	_end = true;

	_value = v;
	_object = nullptr;
	_array = nullptr;

	if (type == Bucket::TYPE::OBJECT)
		_object = new List<Bucket::Entry>();

	if (type == Bucket::TYPE::ARRAY)
		_array = new List<Bucket>();
}

Bucket* Bucket::_bucket (const String &key) {
	for (Bucket::Entry &entry : (*_object))
		if (entry.Key == key)
			return entry.Value;

	return nullptr;
}

Bucket &Bucket::_bucketValue (Bucket::TYPE t, Qybercom::Value* v) {
	_clear();
	_type = t;
	_value = v;

	return *this;
}

Bucket &Bucket::_associate (const String &key) {
	if (_type != Bucket::TYPE::OBJECT) {
		_clear();
		_type = Bucket::TYPE::OBJECT;
		_object = new List<Bucket::Entry>();
	}

	Bucket* v = _bucket(key);
	if (v != nullptr) return *v;

	Bucket::Entry e(key, new Bucket());
	_object->Add(e);

	return *_object->Last().Value;
}

Bucket::Bucket () {
	_type = Bucket::TYPE::UNDEFINED;
	_key = nullptr;
	_end = true;

	_value = nullptr;
	_object = nullptr;
	_array = nullptr;
}

Bucket::Bucket (const Bucket &other) {
	_type = Bucket::TYPE::UNDEFINED;

	*this = other;
}

Bucket::~Bucket () {
	_clear();
}

Bucket Bucket::Null () {
	return Bucket(Bucket::TYPE::VALUE, new Qybercom::Value(nullptr));
}

Bucket Bucket::Value (const Qybercom::Value &value) {
	return Bucket(TYPE::VALUE, new Qybercom::Value(value));
}

Bucket Bucket::Object () {
	return Bucket(TYPE::OBJECT);
}

Bucket Bucket::Array () {
	return Bucket(TYPE::ARRAY);
}

void Bucket::Add (const Bucket &value) {
	if (_type != Bucket::TYPE::ARRAY) {
		_clear();
		_type = Bucket::TYPE::ARRAY;
		_array = new List<Bucket>();
	}

	_array->Add(value);
}

bool Bucket::HasKey () const {
	return _key != nullptr;
}

const String Bucket::Key () const {
	return _key == nullptr ? "" : *_key;
}

bool Bucket::End () const {
	return _end;
}

Bucket::Iterator Bucket::begin () {
	if (_type == Bucket::TYPE::OBJECT)
		return Bucket::Iterator(this, 0);

	if (_type == Bucket::TYPE::ARRAY)
		return Bucket::Iterator(this, 0);

	return Bucket::Iterator(this, 0);
}

Bucket::Iterator Bucket::end () {
	if (_type == Bucket::TYPE::OBJECT)
		return Bucket::Iterator(this, _object->Count());

	if (_type == Bucket::TYPE::ARRAY)
		return Bucket::Iterator(this, _array->Count());

	return Bucket::Iterator(this, 0);
}

Bucket &Bucket::operator= (decltype(nullptr) v) {
	return _bucketValue(Bucket::TYPE::VALUE, new Qybercom::Value(v));
}

Bucket &Bucket::operator= (bool v) {
	return _bucketValue(Bucket::TYPE::VALUE, new Qybercom::Value(v));
}

Bucket &Bucket::operator= (int v) {
	return _bucketValue(Bucket::TYPE::VALUE, new Qybercom::Value(v));
}

Bucket &Bucket::operator= (long v) {
	return _bucketValue(Bucket::TYPE::VALUE, new Qybercom::Value(v));
}

Bucket &Bucket::operator= (float v) {
	return _bucketValue(Bucket::TYPE::VALUE, new Qybercom::Value(v));
}

Bucket &Bucket::operator= (double v) {
	return _bucketValue(Bucket::TYPE::VALUE, new Qybercom::Value(v));
}

Bucket &Bucket::operator= (const char* v) {
	return _bucketValue(Bucket::TYPE::VALUE, new Qybercom::Value(v));
}

Bucket &Bucket::operator= (const String &v) {
	return _bucketValue(Bucket::TYPE::VALUE, new Qybercom::Value(v));
}

Bucket &Bucket::operator= (const Qybercom::Value &v) {
	return _bucketValue(Bucket::TYPE::VALUE, new Qybercom::Value(v));
}

Bucket &Bucket::operator= (const Bucket &other) {
	if (this == &other)
		return *this;

	_clear();

	_type = other._type;

	switch (_type) {
		case Bucket::TYPE::VALUE:
			_value = new Qybercom::Value(*other._value);
			break;

		case Bucket::TYPE::OBJECT:
			_object = new List<Bucket::Entry>();

			for (Bucket::Entry &entry : (*other._object)) {
				Bucket* copy = new Bucket(*entry.Value);

				_object->Add(Bucket::Entry(entry.Key, copy));
			}

			break;

		case Bucket::TYPE::ARRAY:
			_array = new List<Bucket>();

			for (Bucket &bucket : (*other._array))
				_array->Add(bucket);

			break;

		default:
			break;
	}

	return *this;
}

Bucket &Bucket::operator[] (const String &key) {
	return _associate(key);
}

Bucket &Bucket::operator[] (const char* key) {
	String k = String(key);

	return _associate(k);
}

Bucket &Bucket::operator[] (int index) {
	if (_type != Bucket::TYPE::ARRAY) {
		_clear();
		_type = Bucket::TYPE::ARRAY;
		_array = new List<Bucket>();
	}

	while (_array->Count() <= index) {
		_array->Add(Bucket());
	}

	return (*_array)[index];
}

Bucket::operator bool () const {
	return _type == Bucket::TYPE::VALUE && _value != nullptr ? (bool)(*_value) : false;
}

Bucket::operator int () const {
	return _type == Bucket::TYPE::VALUE && _value != nullptr ? (int)(*_value) : 0;
}

Bucket::operator long () const {
	return _type == Bucket::TYPE::VALUE && _value != nullptr ? (long)(*_value) : 0;
}

Bucket::operator float () const {
	return _type == Bucket::TYPE::VALUE && _value != nullptr ? (float)(*_value) : 0.0f;
}

Bucket::operator double () const {
	return _type == Bucket::TYPE::VALUE && _value != nullptr ? (double)(*_value) : 0.0f;
}

Bucket::operator String () const {
	return _type == Bucket::TYPE::VALUE && _value != nullptr ? (String)(*_value) : "";
}

bool Bucket::IsUndefined () const {
	return _type == Bucket::TYPE::UNDEFINED;
}

bool Bucket::IsNull () const {
	return _type == Bucket::TYPE::VALUE && _value != nullptr && _value->IsNull();
}

bool Bucket::IsValue () const {
	return _type == Bucket::TYPE::VALUE;
}

bool Bucket::IsObject () const {
	return _type == Bucket::TYPE::OBJECT;
}

bool Bucket::IsArray () const {
	return _type == Bucket::TYPE::ARRAY;
}

Qybercom::Value &Bucket::AsValue () {
	if (_type != Bucket::TYPE::VALUE) {
		_clear();
		_type = Bucket::TYPE::VALUE;
		_value = new Qybercom::Value();
	}

	return *_value;
}

List<Bucket::Entry> &Bucket::AsObject () {
	if (_type != Bucket::TYPE::OBJECT) {
		_clear();
		_type = Bucket::TYPE::OBJECT;
		_object = new List<Bucket::Entry>();
	}

	return *_object;
}

List<Bucket> &Bucket::AsArray () {
	if (_type != Bucket::TYPE::ARRAY) {
		_clear();
		_type = Bucket::TYPE::ARRAY;
		_array = new List<Bucket>();
	}

	return *_array;
}

String Bucket::Serialize (IBucketFormat *format) {
	return format == nullptr ? "" : format->BucketFormatSerialize(*this);
}

Bucket Bucket::Deserialize (IBucketFormat *format, String raw) {
	return format->BucketFormatDeserialize(raw);
}

void Bucket::Dump (int indent) {
	Serial.print(Qybercom::indent(indent));

	if (HasKey()) {
		Serial.print(Key());
		Serial.print(": ");
	}

	if (IsValue()) {
		Serial.println(AsValue().Trace());
		return;
	}

	if (IsObject()) {
		Serial.println("{");

		for (Bucket &v : *this)
			v.Dump(indent + 1);

		Serial.print(Qybercom::indent(indent));

		Serial.println("}");
	}

	if (IsArray()) {
		Serial.println("[");

		for (Bucket &v : *this)
			v.Dump(indent + 1);

		Serial.print(Qybercom::indent(indent));

		Serial.println("]");

		return;
	}
}