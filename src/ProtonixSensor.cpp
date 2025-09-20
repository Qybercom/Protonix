#include <Arduino.h>

#include "ProtonixSensor.h"

using namespace Qybercom::Protonix;



ProtonixSensor::ProtonixSensor () {
	this->ID("");
	this->Value("");
	this->Active(false);
	this->Failure(false);
	this->State("");
}

ProtonixSensor::ProtonixSensor (String id) {
	this->ID(id);
	this->Value("");
	this->Active(false);
	this->Failure(false);
	this->State("");
}

ProtonixSensor::ProtonixSensor (String id, String value) {
	this->ID(id);
	this->Value(value);
	this->Active(false);
	this->Failure(false);
	this->State("");
}

ProtonixSensor::ProtonixSensor (String id, String value, bool active) {
	this->ID(id);
	this->Value(value);
	this->Active(active);
	this->Failure(false);
	this->State("");
}

ProtonixSensor::ProtonixSensor (String id, String value, bool active, bool failure) {
	this->ID(id);
	this->Value(value);
	this->Active(active);
	this->Failure(failure);
	this->State("");
}

ProtonixSensor::ProtonixSensor (String id, String value, bool active, bool failure, String state) {
	this->ID(id);
	this->Value(value);
	this->Active(active);
	this->Failure(failure);
	this->State(state);
}

ProtonixSensor::ProtonixSensor (String id, bool active) {
	this->ID(id);
	this->Value("");
	this->Active(active);
	this->Failure(false);
	this->State("");
}

ProtonixSensor::ProtonixSensor (String id, bool active, bool failure) {
	this->ID(id);
	this->Value("");
	this->Active(active);
	this->Failure(failure);
	this->State("");
}

ProtonixSensor::ProtonixSensor (String id, bool active, bool failure, String state) {
	this->ID(id);
	this->Value("");
	this->Active(active);
	this->Failure(failure);
	this->State(state);
}

ProtonixSensor::ProtonixSensor (String id, String value, String state) {
	this->ID(id);
	this->Value(value);
	this->Failure(false);
	this->Failure(false);
	this->State(state);
}

void ProtonixSensor::ID (String id) {
	this->_id = id;
}

String ProtonixSensor::ID () {
	return this->_id;
}

void ProtonixSensor::Value (String value) {
	this->_value = value;
}

String ProtonixSensor::Value () {
	return this->_value;
}

void ProtonixSensor::Active (bool active) {
	this->_active = active;
}

bool ProtonixSensor::Active () {
	return this->_active;
}

void ProtonixSensor::Failure (bool failure) {
	this->_failure = failure;
}

bool ProtonixSensor::Failure () {
	return this->_failure;
}

void ProtonixSensor::State (String state) {
	this->_state = state;
}

String ProtonixSensor::State () {
	return this->_state;
}

String ProtonixSensor::Summary () {
	return "[s"
		+ this->_id + ":"
		+ this->_value + ""
		+ (this->_active ? ":active" : "")
		+ (this->_failure ? ":failure" : "")
		+ (this->_state == "" ? "" : ":" + this->_state)
		+ "]";
}

void ProtonixSensor::Reset () {
	this->Value("");
	this->Active(false);
	this->Failure(false);
	this->State("");
}