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

Value::Value (bool v) {
	_type = Value::TYPE::BOOL;
	_value.BOOL = v;
}

Value::Value (short v) {
	_type = Value::TYPE::INT;
	_value.INT = v;
}

Value::Value (unsigned short v) {
	_type = Value::TYPE::INT;
	_value.INT = v;
}

Value::Value (int v) {
	_type = Value::TYPE::INT;
	_value.INT = v;
}

Value::Value (unsigned int v) {
	_type = Value::TYPE::INT;
	_value.INT = v;
}

Value::Value (long v) {
	_type = Value::TYPE::INT;
	_value.INT = v;
}

Value::Value (unsigned long v) {
	_type = Value::TYPE::INT;
	_value.INT = v;
}

Value::Value (float v) {
	_type = Value::TYPE::FLOAT;
	_value.FLOAT = v;
}

Value::Value (double v) {
	_type = Value::TYPE::FLOAT;
	_value.FLOAT = v;
}

Value::Value (const char* s) {
	_type = Value::TYPE::STRING;
	_value.STRING = strDup(s);
}

Value::Value (String s) {
	_type = Value::TYPE::STRING;
	_value.STRING = strDup(s.c_str());
}

Value::Value (const Value &other) {
	*this = other;
}


Value::~Value () {
	Clear();
}


Value &Value::operator= (bool v) {
	Clear();

	_type = Value::TYPE::BOOL;
	_value.BOOL = v;

	return *this;
}

Value &Value::operator= (short v) {
	Clear();

	_type = Value::TYPE::INT;
	_value.INT = v;

	return *this;
}

Value &Value::operator= (unsigned short v) {
	Clear();

	_type = Value::TYPE::INT;
	_value.INT = v;

	return *this;
}

Value &Value::operator= (int v) {
	Clear();

	_type = Value::TYPE::INT;
	_value.INT = v;

	return *this;
}

Value &Value::operator= (unsigned int v) {
	Clear();

	_type = Value::TYPE::INT;
	_value.INT = v;

	return *this;
}

Value &Value::operator= (long v) {
	Clear();

	_type = Value::TYPE::INT;
	_value.INT = v;

	return *this;
}

Value &Value::operator= (unsigned long v) {
	Clear();

	_type = Value::TYPE::INT;
	_value.INT = v;

	return *this;
}

Value &Value::operator= (float v) {
	Clear();

	_type = Value::TYPE::FLOAT;
	_value.FLOAT = v;

	return *this;
}

Value &Value::operator= (double v) {
	Clear();

	_type = Value::TYPE::FLOAT;
	_value.FLOAT = v;

	return *this;
}

Value &Value::operator= (const char* s) {
	Clear();

	_type = Value::TYPE::STRING;
	_value.STRING = strDup(s);

	return *this;
}

Value &Value::operator= (String s) {
	Clear();

	_type = Value::TYPE::STRING;
	_value.STRING = strDup(s.c_str());

	return *this;
}

Value &Value::operator= (const Value &other) {
	if (this == &other) return *this;

	Clear();

	_type = other._type;

	switch (_type) {
		case Value::TYPE::BOOL: _value.BOOL = other._value.BOOL; break;
		case Value::TYPE::INT: _value.INT = other._value.INT; break;
		case Value::TYPE::FLOAT: _value.FLOAT = other._value.FLOAT; break;
		case Value::TYPE::STRING: _value.STRING = strDup(other._value.STRING); break;
		case Value::TYPE::OBJECT:
		case Value::TYPE::ARRAY: {
			_value.COLLECTION.items = nullptr;
			_value.COLLECTION.count = 0;
			_value.COLLECTION.capacity = 0;

			int i = 0;

			while (i < other._value.COLLECTION.count) {
				Value &src = other._value.COLLECTION.items[i];

				if (_type == Value::TYPE::OBJECT && src._key) (*this)[src._key] = src;
				else Add(src);

				i++;
			}

			break;
		}
		default: break;
	}

	return *this;
}


Value &Value::operator[] (const char* key) {
	if (_type != Value::TYPE::OBJECT) {
		Clear();

		_type = Value::TYPE::OBJECT;
		_value.COLLECTION.items = nullptr;
		_value.COLLECTION.count = 0;
		_value.COLLECTION.capacity = 0;
	}

	int i = 0;

	while (i < _value.COLLECTION.count) {
		Value &b = _value.COLLECTION.items[i];

		if (b._key && strcmp(b._key, key) == 0)
			return b;

		i++;
	}

	_allocate();

	Value &b = _value.COLLECTION.items[_value.COLLECTION.count++];
	b = Value();
	b._key = strDup(key);

	return b;
}

Value &Value::operator[] (const String &key) {
	return (*this)[key.c_str()];
}

Value &Value::operator[] (int index) {
	return *this; // boilerplate
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


bool Value::IsUndefined () const {
	return _type == Value::TYPE::UNDEFINED;
}

bool Value::IsNull () const {
	return false;//_type == Value::TYPE::VALUE && _value != nullptr && _value->IsNull();
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


Value::TYPE Value::Type () const {
	return _type;
}

String Value::TypeName () const {
	switch (_type) {
		//case Value::TYPE::NULLPTR: return "null"; break;
		case Value::TYPE::BOOL: return "bool"; break;
		case Value::TYPE::INT:	return "int"; break;
		case Value::TYPE::FLOAT: return "float"; break;
		case Value::TYPE::STRING: return "string"; break;
		default: return "undefined"; break;
	}
}

String Value::ToString () const {
	switch (_type) {
		//case Value::TYPE::NULLPTR: return "null"; break;
		case Value::TYPE::BOOL: return String(_value.BOOL ? "true" : "false"); break;
		case Value::TYPE::INT: return String(_value.INT); break;
		case Value::TYPE::FLOAT: return String(_value.FLOAT); break;
		case Value::TYPE::STRING: return String(_value.STRING); break;
		default: return "null"; break;
		//default: return ""; break;
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

bool Value::HasKey (String key) const {
	int i = 0;
	
	while (i < _value.COLLECTION.count) {
		if (_value.COLLECTION.items[i].Key() == key) return true;

		i++;
	}

	return false;
}

int Value::Count () const {
	return (_type == Value::TYPE::ARRAY || _type == Value::TYPE::OBJECT) ? _value.COLLECTION.count : -1;
}

int Value::Capacity () const {
	return (_type == Value::TYPE::ARRAY || _type == Value::TYPE::OBJECT) ? _value.COLLECTION.capacity : -1;
}

bool Value::Contains (String key) const {
	if (_type != Value::TYPE::OBJECT || !_value.COLLECTION.items) return false;

	int i = 0;

	while (i < _value.COLLECTION.count) {
		char* k = _value.COLLECTION.items[i]._key;

		if (k && strcmp(k, key.c_str()) == 0)
			return true;

		i++;
	}

	return false;
}

Value &Value::Add (const Value &b) {
	if (_type != ARRAY) {
		Clear();

		_type = Value::TYPE::ARRAY;
		_value.COLLECTION.items = nullptr;
		_value.COLLECTION.count = 0;
		_value.COLLECTION.capacity = 0;
	}

	_allocate();

	Value &out = _value.COLLECTION.items[_value.COLLECTION.count++];
	out = b;

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

Value Value::Deserialize (IValueFormat* format, String raw) {
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