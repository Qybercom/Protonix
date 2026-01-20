#include <Arduino.h>

#include "Common/index.h"

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

Bucket &ProtonixSignal::Data () {
	return this->_data;
}

ProtonixSignal* ProtonixSignal::Data (Bucket data) {
	this->_data = data;

	return this;
}

ProtonixSignal::~ProtonixSignal () {
}