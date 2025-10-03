#include <Arduino.h>

#include "ProtonixHardwareCapability.h"

using namespace Qybercom::Protonix;

ProtonixHardwareCapability::ProtonixHardwareCapability (String kind, String id, String comment) {
	this->_kind = kind;
	this->_id = id;
	this->_comment = comment;
	this->_value = "";
}

String ProtonixHardwareCapability::Kind () {
	return this->_kind;
}

ProtonixHardwareCapability* ProtonixHardwareCapability::Kind (String kind) {
	this->_kind = kind;

	return this;
}

String ProtonixHardwareCapability::ID () {
	return this->_id;
}

ProtonixHardwareCapability* ProtonixHardwareCapability::ID (String id) {
	this->_id = id;

	return this;
}

String ProtonixHardwareCapability::Value () {
	return this->_value;
}

ProtonixHardwareCapability* ProtonixHardwareCapability::Value (String value) {
	this->_value = value;

	return this;
}

String ProtonixHardwareCapability::Comment () {
	return this->_comment;
}

ProtonixHardwareCapability* ProtonixHardwareCapability::Comment (String comment) {
	this->_comment = comment;

	return this;
}