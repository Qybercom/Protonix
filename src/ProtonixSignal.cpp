#include <Arduino.h>

#include "Common/Data.hpp"

#include "ProtonixSignal.h"

using namespace Qybercom;
using namespace Qybercom::Protonix;

ProtonixSignal::ProtonixSignal (String id) {
	this->_id = id;
	this->_from = "";
}

ProtonixSignal::ProtonixSignal (String from, String id) {
	this->_id = id;
	this->_from = from;
}

String ProtonixSignal::ID () {
	return this->_id;
}

ProtonixSignal* ProtonixSignal::ID (String id) {
	this->_id = id;

	return this;
}

String ProtonixSignal::From () {
	return this->_from;
}

ProtonixSignal* ProtonixSignal::From (String from) {
	this->_from = from;

	return this;
}

bool ProtonixSignal::Is (String id) {
	return this->_id == id;
}

bool ProtonixSignal::Is (String from, String id) {
	return this->_id == id && this->_from == from;
}

Any ProtonixSignal::Value () {
	return this->_value;
}

ProtonixSignal* ProtonixSignal::Value (Any value) {
	this->_value = value;

	return this;
}

ProtonixSignal::~ProtonixSignal () {
}