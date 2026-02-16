#include <Arduino.h>

#include "Raw.h"

using namespace Qybercom::Types;

Raw::Raw () {
	_size = 0;
	_data = nullptr;
}

Raw::Raw (const Raw &other) {
	_size = other._size;
	_data = _size ? new char[_size] : nullptr;

	if (_size != 0)
		memcpy(_data, other._data, _size);
}

Raw::Raw (Raw&& other) noexcept {
	_size = other._size;
	_data = other._data;

	other._size = 0;
	other._data = nullptr;
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

Raw &Raw::operator= (Raw&& other) noexcept {
	if (this == &other) return *this;

	delete[] _data;
	_size = other._size;
	_data = other._data;

	other._size = 0;
	other._data = nullptr;

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
	return _data != nullptr;
}

Raw* Raw::Copy (Raw *raw) {
	return raw == nullptr ? nullptr : new Raw(*raw);
}