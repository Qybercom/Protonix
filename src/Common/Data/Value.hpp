#pragma once

#include <Arduino.h>

namespace Qybercom {
	class Value {
		public:
			enum TYPE {
				UNDEFINED,
				NULLPTR,
				BOOL,
				INT,
				FLOAT,
				STRING
			};

		private:
			TYPE _type;

			union {
				bool _v_bool;
				long _v_int;
				double _v_float;
			};

			String _v_string;

		public:
			Value () : _type(TYPE::UNDEFINED) { }

			Value (decltype(nullptr) v) { Set(v); }
			Value (bool v) { Set(v); }
			Value (int v) { Set(v); }
			Value (long v) { Set(v); }
			Value (float v) { Set(v); }
			Value (double v) { Set(v); }
			Value (const char* v) { Set(String(v)); }
			Value (const String& v) { Set(v); }

			Value &Clear () {
				_v_string = "";
				_type = TYPE::UNDEFINED;

				return *this;
			}

			Value& Set (decltype(nullptr) v) {
				Clear();

				(void)v;
				_type = TYPE::NULLPTR;

				return *this;
			}

			Value &Set (bool v) {
				Clear();

				_v_bool = v;
				_type = TYPE::BOOL;

				return *this;
			}

			Value &Set (int v) {
				Clear();

				_v_int = v;
				_type = TYPE::INT;

				return *this;
			}

			Value &Set (long v) {
				Clear();

				_v_int = v;
				_type = TYPE::INT;

				return *this;
			}

			Value &Set (float v) {
				Clear();

				_v_float = v;
				_type = TYPE::FLOAT;

				return *this;
			}

			Value &Set (double v) {
				Clear();

				_v_float = v;
				_type = TYPE::FLOAT;

				return *this;
			}

			Value &Set (const String& v) {
				Clear();

				_v_string = v;
				_type = TYPE::STRING;

				return *this;
			}

			Value &operator= (decltype(nullptr) v) { return Set(v); }
			Value &operator= (bool v) { return Set(v); }
			Value &operator= (int v) { return Set(v); }
			Value &operator= (long v) { return Set(v); }
			Value &operator= (float v) { return Set(v); }
			Value &operator= (double v) { return Set(v); }
			Value &operator= (const char* v) { return Set(String(v)); }
			Value &operator= (const String& v) { return Set(v); }

			operator bool () const {
				if (_type == TYPE::NULLPTR) return false;
				if (_type == TYPE::BOOL) return _v_bool;
				if (_type == TYPE::INT) return _v_int != 0;
				if (_type == TYPE::FLOAT) return _v_float != 0.0f;
				if (_type == TYPE::STRING) return _v_string == "true";

				return false;
			}

			operator int () const {
				if (_type == TYPE::NULLPTR) return 0;
				if (_type == TYPE::INT) return (int)_v_int;
				if (_type == TYPE::BOOL) return _v_bool ? 1 : 0;
				if (_type == TYPE::FLOAT) return (int)_v_float;
				if (_type == TYPE::STRING) return _v_string.toInt();

				return 0;
			}

			operator long () const {
				if (_type == TYPE::NULLPTR) return 0;
				if (_type == TYPE::INT) return (long)_v_int;
				if (_type == TYPE::BOOL) return _v_bool ? 1 : 0;
				if (_type == TYPE::FLOAT) return (long)_v_float;
				if (_type == TYPE::STRING) return (long)_v_string.toInt();

				return 0;
			}

			operator float () const {
				if (_type == TYPE::NULLPTR) return 0.0f;
				if (_type == TYPE::FLOAT) return _v_float;
				if (_type == TYPE::INT) return (float)_v_int;
				if (_type == TYPE::BOOL) return _v_bool ? 1.0f : 0.0f;
				if (_type == TYPE::STRING) return _v_string.toFloat();

				return 0.0f;
			}

			operator double () const {
				if (_type == TYPE::NULLPTR) return 0.0;
				if (_type == TYPE::FLOAT) return (double)_v_float;
				if (_type == TYPE::INT) return (double)_v_int;
				if (_type == TYPE::BOOL) return _v_bool ? 1.0 : 0.0;
				if (_type == TYPE::STRING) return (double)_v_string.toFloat();

				return 0.0f;
			}

			operator String () const {
				return ToString();
			}

			TYPE Type () const {
				return _type;
			}

			String TypeName() const {
				switch (_type) {
					case TYPE::NULLPTR: return "null"; break;
					case TYPE::BOOL: return "bool"; break;
					case TYPE::INT:	return "int"; break;
					case TYPE::FLOAT: return "float"; break;
					case TYPE::STRING: return "string"; break;
					default: return "undefined"; break;
				}
			}

			String ToString () const {
				switch (_type) {
					case TYPE::NULLPTR: return "null"; break;
					case TYPE::BOOL: return String(_v_bool ? "true" : "false"); break;
					case TYPE::INT: return String(_v_int); break;
					case TYPE::FLOAT: return String(_v_float); break;
					case TYPE::STRING: return _v_string; break;
					default: return ""; break;
				}
			}

			String Trace () {
				return "(" + TypeName() + ")" + ToString();
			}

			bool Equals (const Value& other) const {
				if (_type != other._type) return false;

				switch (_type) {
					case TYPE::NULLPTR: return true; break;
					case TYPE::BOOL: return _v_bool == other._v_bool; break;
					case TYPE::INT: return _v_int == other._v_int; break;
					case TYPE::FLOAT: return _v_float == other._v_float; break;
					case TYPE::STRING: return _v_string == other._v_string; break;
					default: return false; break; // maybe true
				}
			}

			bool IsUndefined () const { return _type == TYPE::UNDEFINED; }
			bool IsNull () const { return _type == TYPE::NULLPTR; }
			bool IsInt () const { return _type == TYPE::INT; }
			bool IsFloat () const { return _type == TYPE::FLOAT; }
			bool IsBool () const { return _type == TYPE::BOOL; }
			bool IsString () const { return _type == TYPE::STRING; }

			bool IsNumeric () const {
				return _type == TYPE::INT || _type == TYPE::FLOAT;
			}

			bool operator== (const Value& other) const {
				if (_type == other._type)
					return Equals(other);

				if (IsNumeric() && other.IsNumeric())
					return (float)(*this) == (float)other;

				return false;
			}

			bool operator!= (const Value& other) const {
				return !(*this == other);
			}

			bool operator< (const Value& other) const {
				if (IsNumeric() && other.IsNumeric())
					return (float)(*this) < (float)other;

				if (_type == TYPE::STRING && other._type == TYPE::STRING)
					return _v_string < other._v_string;

				return false;
			}

			bool operator> (const Value& other) const {
				return other < *this;
			}

			bool operator<= (const Value& other) const {
				return !(*this > other);
			}

			bool operator>= (const Value& other) const {
				return !(*this < other);
			}

			// comparison BOOL
			bool operator== (bool v) const {
				return IsBool() && _v_bool == v;
			}

			bool operator!= (bool v) const {
				return !(*this == v);
			}

			// comparison INT
			bool operator== (int v) const {
				if (IsInt()) return _v_int == v;
				if (IsFloat()) return _v_float == (float)v;

				return false;
			}

			bool operator!= (int v) const {
				return !(*this == v);
			}

			bool operator< (int v) const {
				if (IsInt()) return _v_int < v;
				if (IsFloat()) return _v_float < (float)v;

				return false;
			}

			bool operator> (int v) const {
				if (IsInt()) return _v_int > v;
				if (IsFloat()) return _v_float > (float)v;

				return false;
			}

			bool operator<= (int v) const {
				return !(*this > v);
			}

			bool operator>= (int v) const {
				return !(*this < v);
			}

			bool operator== (long v) const {
				if (IsInt()) return _v_int == v;
				if (IsFloat()) return _v_float == (double)v;

				return false;
			}

			bool operator!= (long v) const {
				return !(*this == v);
			}

			bool operator< (long v) const {
				if (IsInt()) return _v_int < v;
				if (IsFloat()) return _v_float < (double)v;

				return false;
			}

			bool operator> (long v) const {
				if (IsInt()) return _v_int > v;
				if (IsFloat()) return _v_float > (double)v;

				return false;
			}

			bool operator<= (long v) const {
				return !(*this > v);
			}

			bool operator>= (long v) const {
				return !(*this < v);
			}

			// comparison FLOAT
			bool operator== (float v) const {
				if (IsFloat()) return _v_float == v;
				if (IsInt()) return (float)_v_int == v;

				return false;
			}

			bool operator!= (float v) const {
				return !(*this == v);
			}

			bool operator< (float v) const {
				if (IsFloat()) return _v_float < v;
				if (IsInt()) return (float)_v_int < v;

				return false;
			}

			bool operator> (float v) const {
				if (IsFloat()) return _v_float > v;
				if (IsInt()) return (float)_v_int > v;

				return false;
			}

			bool operator<= (float v) const {
				return !(*this > v);
			}

			bool operator>= (float v) const {
				return !(*this < v);
			}

			bool operator== (double v) const {
				if (IsFloat()) return _v_float == v;
				if (IsInt()) return (double)_v_int == v;

				return false;
			}

			bool operator!= (double v) const {
				return !(*this == v);
			}

			bool operator< (double v) const {
				if (IsFloat()) return _v_float < v;
				if (IsInt()) return (double)_v_int < v;

				return false;
			}

			bool operator> (double v) const {
				if (IsFloat()) return _v_float > v;
				if (IsInt()) return (double)_v_int > v;

				return false;
			}

			bool operator<= (double v) const {
				return !(*this > v);
			}

			bool operator>= (double v) const {
				return !(*this < v);
			}

			// comparison STRING
			bool operator== (const String& v) const {
				return IsString() && _v_string == v;
			}

			bool operator!= (const String& v) const {
				return !(*this == v);
			}

			bool operator< (const String& v) const {
				return IsString() && _v_string < v;
			}

			bool operator> (const String& v) const {
				return IsString() && _v_string > v;
			}

			bool operator<= (const String& v) const {
				return !(*this > v);
			}

			bool operator>= (const String& v) const {
				return !(*this < v);
			}

			bool operator== (const char* v) const {
				return IsString() && _v_string == v;
			}

			bool operator!= (const char* v) const {
				return !(*this == v);
			}

			bool operator< (const char* v) const {
				return IsString() && _v_string < v;
			}

			bool operator> (const char* v) const {
				return IsString() && _v_string > v;
			}

			bool operator<= (const char* v) const {
				return !(*this > v);
			}

			bool operator>= (const char* v) const {
				return !(*this < v);
			}
	};
}