#include <Arduino.h>

#include "ProtonixSignal.h"

using namespace Qybercom::Protonix;

ProtonixSignal::ProtonixSignal (String id) {
	this->_id = id;
	this->_valueInt = 0;
	this->_valueFloat = 0;
	this->_from = "";
}

ProtonixSignal::ProtonixSignal (String from, String id) {
	this->_id = id;
	this->_valueInt = 0;
	this->_valueFloat = 0;
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

bool ProtonixSignal::ValueBool () {
	return this->_valueBool;
}

ProtonixSignal* ProtonixSignal::ValueBool (bool value) {
	this->_valueBool = value;

	return this;
}

int ProtonixSignal::ValueInt () {
	return this->_valueInt;
}

ProtonixSignal* ProtonixSignal::ValueInt (int value) {
	this->_valueInt = value;

	return this;
}

float ProtonixSignal::ValueFloat () {
	return this->_valueFloat;
}

ProtonixSignal* ProtonixSignal::ValueFloat (float value) {
	this->_valueFloat = value;

	return this;
}

String ProtonixSignal::ValueString () {
	return this->_valueString;
}

ProtonixSignal* ProtonixSignal::ValueString (String value) {
	this->_valueString = value;

	return this;
}