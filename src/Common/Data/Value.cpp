#include <Arduino.h>

#include "Raw.hpp"

#include "Value.h"

using namespace Qybercom;

Value::Value () {
	_type = Value::TYPE::UNDEFINED;
}

Value::Value (decltype(nullptr) v) {
	Set(v);
}

Value::Value (bool v) {
	Set(v);
}

Value::Value (int v) {
	Set(v);
}

Value::Value (long v) {
	Set(v);
}

Value::Value (float v) {
	Set(v);
}

Value::Value (double v) {
	Set(v);
}

Value::Value (const char* v) {
	Set(String(v));
}

Value::Value (const String &v) {
	Set(v);
}

Value::Value (const Raw &v) {
	Set(v);
}

Value &Value::Set (decltype(nullptr) v) {
	Clear();

	(void)v;
	_type = Value::TYPE::NULLPTR;

	return *this;
}

Value &Value::Set (bool v) {
	Clear();

	_v_bool = v;
	_type = Value::TYPE::BOOL;

	return *this;
}

Value &Value::Set (int v) {
	Clear();

	_v_int = v;
	_type = Value::TYPE::INT;

	return *this;
}

Value &Value::Set (long v) {
	Clear();

	_v_int = v;
	_type = Value::TYPE::INT;

	return *this;
}

Value &Value::Set (float v) {
	Clear();

	_v_float = v;
	_type = Value::TYPE::FLOAT;

	return *this;
}

Value &Value::Set (double v) {
	Clear();

	_v_float = v;
	_type = Value::TYPE::FLOAT;

	return *this;
}

Value &Value::Set (const String &v) {
	Clear();

	_v_string = v;
	_type = Value::TYPE::STRING;

	return *this;
}

Value &Value::Set (const Raw &v) {
	Clear();

	_v_raw = v;
	_type = Value::TYPE::RAW;

	return *this;
}

Value &Value::operator= (decltype(nullptr) v) {
	return Set(v);
}

Value &Value::operator= (bool v) {
	return Set(v);
}

Value &Value::operator= (int v) {
	return Set(v);
}

Value &Value::operator= (long v) {
	return Set(v);
}

Value &Value::operator= (float v) {
	return Set(v);
}

Value &Value::operator= (double v) {
	return Set(v);
}

Value &Value::operator= (const char* v) {
	return Set(String(v));
}

Value &Value::operator= (const String &v) {
	return Set(v);
}

Value &Value::operator= (const Raw &v) {
	return Set(v);
}

Value::operator bool () const {
	if (_type == Value::TYPE::NULLPTR) return false;
	if (_type == Value::TYPE::BOOL) return _v_bool;
	if (_type == Value::TYPE::INT) return _v_int != 0;
	if (_type == Value::TYPE::FLOAT) return _v_float != 0.0f;
	if (_type == Value::TYPE::STRING) return _v_string == "true";

	return false;
}

Value::operator int () const {
	if (_type == Value::TYPE::NULLPTR) return 0;
	if (_type == Value::TYPE::INT) return (int)_v_int;
	if (_type == Value::TYPE::BOOL) return _v_bool ? 1 : 0;
	if (_type == Value::TYPE::FLOAT) return (int)_v_float;
	if (_type == Value::TYPE::STRING) return _v_string.toInt();

	return 0;
}

Value::operator long () const {
	if (_type == Value::TYPE::NULLPTR) return 0;
	if (_type == Value::TYPE::INT) return (long)_v_int;
	if (_type == Value::TYPE::BOOL) return _v_bool ? 1 : 0;
	if (_type == Value::TYPE::FLOAT) return (long)_v_float;
	if (_type == Value::TYPE::STRING) return (long)_v_string.toInt();

	return 0;
}

Value::operator float () const {
	if (_type == Value::TYPE::NULLPTR) return 0.0f;
	if (_type == Value::TYPE::FLOAT) return _v_float;
	if (_type == Value::TYPE::INT) return (float)_v_int;
	if (_type == Value::TYPE::BOOL) return _v_bool ? 1.0f : 0.0f;
	if (_type == Value::TYPE::STRING) return _v_string.toFloat();

	return 0.0f;
}

Value::operator double () const {
	if (_type == Value::TYPE::NULLPTR) return 0.0;
	if (_type == Value::TYPE::FLOAT) return (double)_v_float;
	if (_type == Value::TYPE::INT) return (double)_v_int;
	if (_type == Value::TYPE::BOOL) return _v_bool ? 1.0 : 0.0;
	if (_type == Value::TYPE::STRING) return (double)_v_string.toFloat();

	return 0.0;
}

Value::operator String () const {
	return ToString();
}

Value::operator Raw () const {
	if (_type != Value::TYPE::RAW) return Raw();

	return _v_raw;
}

Value &Value::Clear () {
	_v_string = "";
	_type = Value::TYPE::UNDEFINED;

	return *this;
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
		case Value::TYPE::BOOL: return String(_v_bool ? "true" : "false"); break;
		case Value::TYPE::INT: return String(_v_int); break;
		case Value::TYPE::FLOAT: return String(_v_float); break;
		case Value::TYPE::STRING: return _v_string; break;
		default: return ""; break;
	}
}

String Value::Trace (bool type) {
	String edge = String(_type == Value::TYPE::STRING ? "\"" : "");

	return type
		? String("(" + TypeName() + ")" + ToString())
		: String(edge + ToString() + edge);
}

bool Value::Equals (const Value &other) const {
	if (_type != other._type) return false;

	switch (_type) {
		case Value::TYPE::NULLPTR: return true; break;
		case Value::TYPE::BOOL: return _v_bool == other._v_bool; break;
		case Value::TYPE::INT: return _v_int == other._v_int; break;
		case Value::TYPE::FLOAT: return _v_float == other._v_float; break;
		case Value::TYPE::STRING: return _v_string == other._v_string; break;
		default: return false; break; // maybe true
	}
}

bool Value::IsUndefined () const {
	return _type == Value::TYPE::UNDEFINED;
}

bool Value::IsNull () const {
	return _type == Value::TYPE::NULLPTR;
}

bool Value::IsInt () const {
	return _type == Value::TYPE::INT;
}

bool Value::IsFloat () const {
	return _type == Value::TYPE::FLOAT;
}

bool Value::IsBool () const {
	return _type == Value::TYPE::BOOL;
}

bool Value::IsString () const {
	return _type == Value::TYPE::STRING;
}

bool Value::IsRaw () const {
	return _type == Value::TYPE::RAW;
}

bool Value::IsNumeric () const {
	return _type == Value::TYPE::INT || _type == Value::TYPE::FLOAT;
}

bool Value::operator== (const Value &other) const {
	if (_type == other._type)
		return Equals(other);

	if (IsNumeric() && other.IsNumeric())
		return (float)(*this) == (float)other;

	return false;
}

bool Value::operator!= (const Value &other) const {
	return !(*this == other);
}

bool Value::operator< (const Value &other) const {
	if (IsNumeric() && other.IsNumeric())
		return (float)(*this) < (float)other;

	if (_type == Value::TYPE::STRING && other._type == Value::TYPE::STRING)
		return _v_string < other._v_string;

	return false;
}

bool Value::operator> (const Value &other) const {
	return other < *this;
}

bool Value::operator<= (const Value &other) const {
	return !(*this > other);
}

bool Value::operator>= (const Value &other) const {
	return !(*this < other);
}

// comparison BOOL
bool Value::operator== (bool v) const {
	return IsBool() && _v_bool == v;
}

bool Value::operator!= (bool v) const {
	return !(*this == v);
}

// comparison INT
bool Value::operator== (int v) const {
	if (IsInt()) return _v_int == v;
	if (IsFloat()) return _v_float == (float)v;

	return false;
}

bool Value::operator!= (int v) const {
	return !(*this == v);
}

bool Value::operator< (int v) const {
	if (IsInt()) return _v_int < v;
	if (IsFloat()) return _v_float < (float)v;

	return false;
}

bool Value::operator> (int v) const {
	if (IsInt()) return _v_int > v;
	if (IsFloat()) return _v_float > (float)v;

	return false;
}

bool Value::operator<= (int v) const {
	return !(*this > v);
}

bool Value::operator>= (int v) const {
	return !(*this < v);
}

bool Value::operator== (long v) const {
	if (IsInt()) return _v_int == v;
	if (IsFloat()) return _v_float == (double)v;

	return false;
}

bool Value::operator!= (long v) const {
	return !(*this == v);
}

bool Value::operator< (long v) const {
	if (IsInt()) return _v_int < v;
	if (IsFloat()) return _v_float < (double)v;

	return false;
}

bool Value::operator> (long v) const {
	if (IsInt()) return _v_int > v;
	if (IsFloat()) return _v_float > (double)v;

	return false;
}

bool Value::operator<= (long v) const {
	return !(*this > v);
}

bool Value::operator>= (long v) const {
	return !(*this < v);
}

// comparison FLOAT
bool Value::operator== (float v) const {
	if (IsFloat()) return _v_float == v;
	if (IsInt()) return (float)_v_int == v;

	return false;
}

bool Value::operator!= (float v) const {
	return !(*this == v);
}

bool Value::operator< (float v) const {
	if (IsFloat()) return _v_float < v;
	if (IsInt()) return (float)_v_int < v;

	return false;
}

bool Value::operator> (float v) const {
	if (IsFloat()) return _v_float > v;
	if (IsInt()) return (float)_v_int > v;

	return false;
}

bool Value::operator<= (float v) const {
	return !(*this > v);
}

bool Value::operator>= (float v) const {
	return !(*this < v);
}

bool Value::operator== (double v) const {
	if (IsFloat()) return _v_float == v;
	if (IsInt()) return (double)_v_int == v;

	return false;
}

bool Value::operator!= (double v) const {
	return !(*this == v);
}

bool Value::operator< (double v) const {
	if (IsFloat()) return _v_float < v;
	if (IsInt()) return (double)_v_int < v;

	return false;
}

bool Value::operator> (double v) const {
	if (IsFloat()) return _v_float > v;
	if (IsInt()) return (double)_v_int > v;

	return false;
}

bool Value::operator<= (double v) const {
	return !(*this > v);
}

bool Value::operator>= (double v) const {
	return !(*this < v);
}

// comparison STRING
bool Value::operator== (const String &v) const {
	return IsString() && _v_string == v;
}

bool Value::operator!= (const String &v) const {
	return !(*this == v);
}

bool Value::operator< (const String &v) const {
	return IsString() && _v_string < v;
}

bool Value::operator> (const String &v) const {
	return IsString() && _v_string > v;
}

bool Value::operator<= (const String &v) const {
	return !(*this > v);
}

bool Value::operator>= (const String &v) const {
	return !(*this < v);
}

bool Value::operator== (const char* v) const {
	return IsString() && _v_string == v;
}

bool Value::operator!= (const char* v) const {
	return !(*this == v);
}

bool Value::operator< (const char* v) const {
	return IsString() && _v_string < v;
}

bool Value::operator> (const char* v) const {
	return IsString() && _v_string > v;
}

bool Value::operator<= (const char* v) const {
	return !(*this > v);
}

bool Value::operator>= (const char* v) const {
	return !(*this < v);
}