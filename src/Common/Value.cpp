#include <Arduino.h>

#include "Utils.h"

#include "Value.h"

using namespace Qybercom;



Value::Iterator::Iterator (Value* ptr) {
	_ptr = ptr;
}

Value &Value::Iterator::operator* () {
	return *_ptr;
}

Value::Iterator &Value::Iterator::operator++ () {
	_ptr++;

	return *this;
}

bool Value::Iterator::operator!= (const Value::Iterator &other) const {
	return _ptr != other._ptr;
}



void Value::_allocate () {
	int min = _value.COLLECTION.count + 1;
	if (_value.COLLECTION.capacity >= min) return;

	int capacity = _value.COLLECTION.capacity ? _value.COLLECTION.capacity + 2 : 2;//* 2 : 2;
	if (capacity < min) capacity = min;

	Value* storage = new Value[capacity];
	int i = 0;

	while (i < _value.COLLECTION.count) {
		storage[i] = _value.COLLECTION.items[i];
		storage[i]._key = _value.COLLECTION.items[i]._key ? strDup(_value.COLLECTION.items[i]._key) : nullptr;

		delete[] _value.COLLECTION.items[i]._key;

		i++;
	}

	if (_value.COLLECTION.items)
		delete[] _value.COLLECTION.items;

	_value.COLLECTION.items = storage;
	_value.COLLECTION.capacity = capacity;
}


Value::Value () {
	_type = Value::TYPE::UNDEFINED;
}

Value::Value (decltype(nullptr) value) {
	Set(value);
}

Value::Value (bool value) {
	Set(value);
}

Value::Value (short value) {
	Set(value);
}

Value::Value (unsigned short value) {
	Set(value);
}

Value::Value (int value) {
	Set(value);
}

Value::Value (unsigned int value) {
	Set(value);
}

Value::Value (long value) {
	Set(value);
}

Value::Value (unsigned long value) {
	Set(value);
}

Value::Value (float value) {
	Set(value);
}

Value::Value (double value) {
	Set(value);
}

Value::Value (const char* value) {
	Set(value);
}

Value::Value (const String &value) {
	Set(value);
}

Value::Value (const Value &value) {
	*this = value;
}


Value::~Value () {
	Clear();
}


Value::operator decltype(nullptr) () const {
	return nullptr;
}

Value::operator bool () const {
	return _type == Value::TYPE::BOOL ? (bool)_value.BOOL : false;
}

Value::operator short () const {
	return _type == Value::TYPE::INT ? (short)_value.INT : 0;
}

Value::operator unsigned short () const {
	return _type == Value::TYPE::INT ? (unsigned short)_value.INT : 0;
}

Value::operator int () const {
	return _type == Value::TYPE::INT ? (int)_value.INT : 0;
}

Value::operator unsigned int () const {
	return _type == Value::TYPE::INT ? (unsigned int)_value.INT : 0;
}

Value::operator long () const {
	return _type == Value::TYPE::INT ? (long)_value.INT : 0;
}

Value::operator unsigned long () const {
	return _type == Value::TYPE::INT ? (unsigned long)_value.INT : 0;
}

Value::operator float () const {
	return _type == Value::TYPE::FLOAT ? (float)_value.FLOAT : 0.0f;
}

Value::operator double () const {
	return _type == Value::TYPE::FLOAT ? (double)_value.FLOAT : 0.0;
}

Value::operator char* () const {
	return _type == Value::TYPE::STRING ? _value.STRING : nullptr;
}

Value::operator String () const {
	return _type == Value::TYPE::STRING ? String(_value.STRING): "";
}


Value &Value::operator[] (const char* key) {
	return Get(key);
}

Value &Value::operator[] (const String &key) {
	return Get(key);
}

Value &Value::operator[] (int key) {
	return Get(key);
}


Value &Value::operator= (decltype(nullptr) value) {
	return Set(value);
}

Value &Value::operator= (bool value) {
	return Set(value);
}

Value &Value::operator= (short value) {
	return Set(value);
}

Value &Value::operator= (unsigned short value) {
	return Set(value);
}

Value &Value::operator= (int value) {
	return Set(value);
}

Value &Value::operator= (unsigned int value) {
	return Set(value);
}

Value &Value::operator= (long value) {
	return Set(value);
}

Value &Value::operator= (unsigned long value) {
	return Set(value);
}

Value &Value::operator= (float value) {
	return Set(value);
}

Value &Value::operator= (double value) {
	return Set(value);
}

Value &Value::operator= (const char* value) {
	return Set(value);
}

Value &Value::operator= (const String &value) {
	return Set(value);
}

Value &Value::operator= (const Value &value) {
	return Set(value);
}


Value &Value::Get (const char* key) {
	if (_type != Value::TYPE::OBJECT) {
		Clear();

		_type = Value::TYPE::OBJECT;
		_value.COLLECTION.items = nullptr;
		_value.COLLECTION.count = 0;
		_value.COLLECTION.capacity = 0;
	}

	int i = 0;

	while (i < _value.COLLECTION.count) {
		Value &item = _value.COLLECTION.items[i];

		if (item._key && strcmp(item._key, key) == 0) {
			item._listener = _listener;

			if (item._listener != nullptr)
				item._listener->ValueListenerGet(item);

			return item;
		}

		i++;
	}

	_allocate();

	Value &item = _value.COLLECTION.items[_value.COLLECTION.count++];
	item = Value();
	item._key = strDup(key);
	item._listener = _listener;

	if (item._listener != nullptr)
		item._listener->ValueListenerGet(item);

	return item;
}

Value &Value::Get (const String &key) {
	return Get(key.c_str());
}

Value &Value::Get (int key) {
	if (_type != Value::TYPE::ARRAY) {
		Clear();

		_type = Value::TYPE::ARRAY;
		_value.COLLECTION.items = nullptr;
		_value.COLLECTION.count = 0;
		_value.COLLECTION.capacity = 0;
	}

	if (key < 0) key = 0;

	while (key >= _value.COLLECTION.count) {
		_allocate();
	}

	Value &item = _value.COLLECTION.items[key];

	if (_listener != nullptr)
		_listener->ValueListenerGet(item);

	return item;
}


Value &Value::Set (decltype(nullptr) value) {
	Clear();

	_type = Value::TYPE::NULLPTR;

	if (_listener != nullptr)
		_listener->ValueListenerSet(*this);

	return *this;
}

Value &Value::Set (bool value) {
	Clear();

	_type = Value::TYPE::BOOL;
	_value.BOOL = value;

	if (_listener != nullptr)
		_listener->ValueListenerSet(*this);

	return *this;
}

Value &Value::Set (short value) {
	Clear();

	_type = Value::TYPE::INT;
	_value.INT = value;

	if (_listener != nullptr)
		_listener->ValueListenerSet(*this);

	return *this;
}

Value &Value::Set (unsigned short value) {
	Clear();

	_type = Value::TYPE::INT;
	_value.INT = value;

	if (_listener != nullptr)
		_listener->ValueListenerSet(*this);

	return *this;
}

Value &Value::Set (int value) {
	Clear();

	_type = Value::TYPE::INT;
	_value.INT = value;

	if (_listener != nullptr)
		_listener->ValueListenerSet(*this);

	return *this;
}

Value &Value::Set (unsigned int value) {
	Clear();

	_type = Value::TYPE::INT;
	_value.INT = value;

	if (_listener != nullptr)
		_listener->ValueListenerSet(*this);

	return *this;
}

Value &Value::Set (long value) {
	Clear();

	_type = Value::TYPE::INT;
	_value.INT = value;

	if (_listener != nullptr)
		_listener->ValueListenerSet(*this);

	return *this;
}

Value &Value::Set (unsigned long value) {
	Clear();

	_type = Value::TYPE::INT;
	_value.INT = value;

	if (_listener != nullptr)
		_listener->ValueListenerSet(*this);

	return *this;
}

Value &Value::Set (float value) {
	Clear();

	_type = Value::TYPE::FLOAT;
	_value.FLOAT = value;

	if (_listener != nullptr)
		_listener->ValueListenerSet(*this);

	return *this;
}

Value &Value::Set (double value) {
	Clear();

	_type = Value::TYPE::FLOAT;
	_value.FLOAT = value;

	if (_listener != nullptr)
		_listener->ValueListenerSet(*this);

	return *this;
}

Value &Value::Set (const char* value) {
	Clear();

	_type = Value::TYPE::STRING;
	_value.STRING = strDup(value);

	if (_listener != nullptr)
		_listener->ValueListenerSet(*this);

	return *this;
}

Value &Value::Set (const String &value) {
	Clear();

	_type = Value::TYPE::STRING;
	_value.STRING = strDup(value.c_str());

	if (_listener != nullptr)
		_listener->ValueListenerSet(*this);

	return *this;
}

Value &Value::Set (const Value &value) {
	if (this == &value) return *this;

	Clear();

	_type = value._type;

	switch (_type) {
		case Value::TYPE::BOOL: _value.BOOL = value._value.BOOL; break;
		case Value::TYPE::INT: _value.INT = value._value.INT; break;
		case Value::TYPE::FLOAT: _value.FLOAT = value._value.FLOAT; break;
		case Value::TYPE::STRING: _value.STRING = strDup(value._value.STRING); break;
		case Value::TYPE::OBJECT:
		case Value::TYPE::ARRAY: {
			_value.COLLECTION.items = nullptr;
			_value.COLLECTION.count = 0;
			_value.COLLECTION.capacity = 0;

			int i = 0;

			while (i < value._value.COLLECTION.count) {
				Value &src = value._value.COLLECTION.items[i];

				if (_type == Value::TYPE::OBJECT && src._key) (*this)[(const char*)src._key] = src;
				else Add(src);

				i++;
			}

			break;
		}
		default: break;
	}

	if (_listener != nullptr)
		_listener->ValueListenerSet(*this);

	return *this;
}

Value &Value::Set (const char* key, const Value &value) {
	(*this)[key] = value;

	return *this;
}

Value &Value::Set (const String &key, const Value &value) {
	(*this)[key.c_str()] = value;

	return *this;
}

Value &Value::Set (int key, const Value &value) {
	(*this)[key] = value;

	return *this;
}


Value &Value::Replace (const Value &value) {
	if (Contains(value.Key()))
		Set(value.Key(), value);

	return *this;
}

Value &Value::Replace (const char* key, const Value &value) {
	if (Contains(key))
		Set(key, value);

	return *this;
}

Value &Value::Replace (const String &key, const Value &value) {
	return Replace(key.c_str(), value);
}

Value &Value::Replace (int key, const Value &value) {
	if (Contains(key))
		Set(key, value);

	return *this;
}


bool Value::IsUndefined () const {
	return _type == Value::TYPE::UNDEFINED;
}

bool Value::IsNull () const {
	return _type == Value::TYPE::NULLPTR;
}

bool Value::IsBool () const {
	return _type == Value::TYPE::BOOL;
}

bool Value::IsInt () const {
	return _type == Value::TYPE::INT;
}

bool Value::IsFloat () const {
	return _type == Value::TYPE::FLOAT;
}

bool Value::IsString () const {
	return _type == Value::TYPE::STRING;
}

bool Value::IsObject () const {
	return _type == Value::TYPE::OBJECT;
}

bool Value::IsArray () const {
	return _type == Value::TYPE::ARRAY;
}

bool Value::IsRaw () const {
	return _type == Value::TYPE::RAW;
}

bool Value::IsNumeric () const {
	return _type == Value::TYPE::INT || _type == Value::TYPE::FLOAT;
}

bool Value::IsScalar () const {
	return _type != Value::TYPE::UNDEFINED && _type != Value::TYPE::ARRAY && _type != Value::TYPE::OBJECT;
}


Value::TYPE Value::Type () const {
	return _type;
}

String Value::TypeName () const {
	switch (_type) {
		case Value::TYPE::NULLPTR: return "null"; break;
		case Value::TYPE::BOOL: return "bool"; break;
		case Value::TYPE::INT:	return "int"; break;
		case Value::TYPE::FLOAT: return "float"; break;
		case Value::TYPE::STRING: return "string"; break;
		default: return "undefined"; break;
	}
}

String Value::ToString () const {
	switch (_type) {
		case Value::TYPE::NULLPTR: return "null"; break;
		case Value::TYPE::BOOL: return String(_value.BOOL ? "true" : "false"); break;
		case Value::TYPE::INT: return String(_value.INT); break;
		case Value::TYPE::FLOAT: return String(_value.FLOAT); break;
		case Value::TYPE::STRING: return String(_value.STRING); break;
		default: return ""; break;
	}
}

String Value::Trace (bool type) {
	String edge = String(_type == Value::TYPE::STRING ? "\"" : "");

	return type
		? String("(" + TypeName() + ")" + ToString())
		: String(edge + ToString() + edge);
}

String Value::Key () const {
	return String(_key);
}

bool Value::HasKey () const {
	return _key != nullptr;
}

int Value::Count () const {
	return (_type == Value::TYPE::ARRAY || _type == Value::TYPE::OBJECT) ? _value.COLLECTION.count : -1;
}

int Value::Capacity () const {
	return (_type == Value::TYPE::ARRAY || _type == Value::TYPE::OBJECT) ? _value.COLLECTION.capacity : -1;
}


bool Value::Contains (const char* key) const {
	if (_type != Value::TYPE::OBJECT || !_value.COLLECTION.items) return false;

	int i = 0;
	bool out = false;

	while (i < _value.COLLECTION.count) {
		char* k = _value.COLLECTION.items[i]._key;

		if (k && strcmp(k, key) == 0) {
			out = true;
			break;
		}

		i++;
	}

	return out;
}

bool Value::Contains (const String &key) const {
	return Contains(key.c_str());
}

bool Value::Contains (int key) const {
	if (_type != Value::TYPE::ARRAY) return false;
	if (key < 0 || key >= (int)_value.COLLECTION.count) return false;

	return true;
}


Value &Value::Add (const Value &item) {
	if (_type != ARRAY) {
		Clear();

		_type = Value::TYPE::ARRAY;
		_value.COLLECTION.items = nullptr;
		_value.COLLECTION.count = 0;
		_value.COLLECTION.capacity = 0;
	}

	_allocate();

	Value &out = _value.COLLECTION.items[_value.COLLECTION.count++];
	out = item;
	out._listener = _listener;

	return out;
}

Value &Value::Clear () {
	if (_type == Value::TYPE::STRING && _value.STRING) {
		delete[] _value.STRING;
		_value.STRING = nullptr;
	}
	else if ((_type == Value::TYPE::OBJECT || _type == Value::TYPE::ARRAY) && _value.COLLECTION.items) {
		int i = 0;

		while (i < _value.COLLECTION.count) {
			Value &b = _value.COLLECTION.items[i];

			b.Clear();

			if (_type == Value::TYPE::OBJECT && b._key) {
				delete[] b._key;
				b._key = nullptr;
			}

			i++;
		}

		delete[] _value.COLLECTION.items;

		_value.COLLECTION.items = nullptr;
		_value.COLLECTION.count = 0;
		_value.COLLECTION.capacity = 0;
	}

	_type = Value::TYPE::UNDEFINED;

	return *this;
}

Value &Value::Listener (IValueListener* listener) {
	_listener = listener;

	return *this;
}


Value::Iterator Value::begin () {
	return (_type == Value::TYPE::OBJECT || _type == Value::TYPE::ARRAY) ? Iterator(_value.COLLECTION.items) : Iterator(nullptr);
}

Value::Iterator Value::end () {
	return (_type == Value::TYPE::OBJECT || _type == Value::TYPE::ARRAY) ? Iterator(_value.COLLECTION.items + _value.COLLECTION.count) : Iterator(nullptr);
}


Value Value::Object () {
	Value b;

	b._type = Value::TYPE::OBJECT;
	b._value.COLLECTION.items = nullptr;
	b._value.COLLECTION.count = 0;
	b._value.COLLECTION.capacity = 0;

	return b;
}

Value Value::Array () {
	Value b;

	b._type = Value::TYPE::ARRAY;
	b._value.COLLECTION.items = nullptr;
	b._value.COLLECTION.count = 0;
	b._value.COLLECTION.capacity = 0;

	return b;
}


String Value::Serialize (IValueFormat* format) {
	return format == nullptr ? "" : format->ValueFormatSerialize(*this);
}

Value Value::Deserialize (IValueFormat* format, const String &raw) {
	return format->ValueFormatDeserialize(raw);
}


void Value::Dump (int indent) {
	Serial.print(Qybercom::indent(indent));

	if (_key != nullptr) {
		Serial.print(_key);
		Serial.print(": ");
	}

	if (_type == Value::TYPE::OBJECT) {
		Serial.println("{");

		int i = 0;

		while (i < _value.COLLECTION.count) {
			_value.COLLECTION.items[i].Dump(indent + 1);

			i++;
		}

		Serial.print(Qybercom::indent(indent));

		Serial.println("}");

		return;
	}

	if (_type == Value::TYPE::ARRAY) {
		Serial.println("[");

		int i = 0;

		while (i < _value.COLLECTION.count) {
			_value.COLLECTION.items[i].Dump(indent + 1);

			i++;
		}

		Serial.print(Qybercom::indent(indent));

		Serial.println("]");

		return;
	}

	Serial.println(Trace());
}