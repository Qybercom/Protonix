#include <Arduino.h>

#include "Raw.h"

using namespace Qybercom::Types;

Raw::Raw () {
	_data = 0;
	_size = 0;
}

Raw::Raw (const Raw &other) {
	_size = other._size;
	_data = _size ? new char[_size] : 0;

	if (_size)
		memcpy(_data, other._data, _size);
}

Raw &Raw::operator= (const Raw &other) {
	if (this == &other) return *this;

	delete[] _data;
	_size = other._size;
	_data = _size ? new char[_size] : 0;

	if (_size)
		memcpy(_data, other._data, _size);

	return *this;
}

Raw::~Raw () {
	delete[] _data;
}

void* Raw::Get () const {
	return _data;
}

int Raw::Size () const {
	return _size;
}

bool Raw::Valid () const {
	return _data != 0;
}