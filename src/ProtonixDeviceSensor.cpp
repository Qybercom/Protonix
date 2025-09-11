#include <Arduino.h>
#include "ProtonixDeviceSensor.h"

using namespace Qybercom::Protonix;



ProtonixDeviceSensor::ProtonixDeviceSensor () {
	this->ID("");
	this->Value("");
	this->Active(false);
	this->Failure(false);
	this->State("");
}

ProtonixDeviceSensor::ProtonixDeviceSensor (String id) {
	this->ID(id);
	this->Value("");
	this->Active(false);
	this->Failure(false);
	this->State("");
}

ProtonixDeviceSensor::ProtonixDeviceSensor (String id, String value) {
	this->ID(id);
	this->Value(value);
	this->Active(false);
	this->Failure(false);
	this->State("");
}

ProtonixDeviceSensor::ProtonixDeviceSensor (String id, String value, bool active) {
	this->ID(id);
	this->Value(value);
	this->Active(active);
	this->Failure(false);
	this->State("");
}

ProtonixDeviceSensor::ProtonixDeviceSensor (String id, String value, bool active, bool failure) {
	this->ID(id);
	this->Value(value);
	this->Active(active);
	this->Failure(failure);
	this->State("");
}

ProtonixDeviceSensor::ProtonixDeviceSensor (String id, String value, bool active, bool failure, String state) {
	this->ID(id);
	this->Value(value);
	this->Active(active);
	this->Failure(failure);
	this->State(state);
}

ProtonixDeviceSensor::ProtonixDeviceSensor (String id, bool active) {
	this->ID(id);
	this->Value("");
	this->Active(active);
	this->Failure(false);
	this->State("");
}

ProtonixDeviceSensor::ProtonixDeviceSensor (String id, bool active, bool failure) {
	this->ID(id);
	this->Value("");
	this->Active(active);
	this->Failure(failure);
	this->State("");
}

ProtonixDeviceSensor::ProtonixDeviceSensor (String id, bool active, bool failure, String state) {
	this->ID(id);
	this->Value("");
	this->Active(active);
	this->Failure(failure);
	this->State(state);
}

ProtonixDeviceSensor::ProtonixDeviceSensor (String id, String value, String state) {
	this->ID(id);
	this->Value(value);
	this->Failure(false);
	this->Failure(false);
	this->State(state);
}

void ProtonixDeviceSensor::ID (String id) {
	this->_id = id;
}

String ProtonixDeviceSensor::ID () {
	return this->_id;
}

void ProtonixDeviceSensor::Value (String value) {
	this->_value = value;
}

String ProtonixDeviceSensor::Value () {
	return this->_value;
}

void ProtonixDeviceSensor::Active (bool active) {
	this->_active = active;
}

bool ProtonixDeviceSensor::Active () {
	return this->_active;
}

void ProtonixDeviceSensor::Failure (bool failure) {
	this->_failure = failure;
}

bool ProtonixDeviceSensor::Failure () {
	return this->_failure;
}

void ProtonixDeviceSensor::State (String state) {
	this->_state = state;
}

String ProtonixDeviceSensor::State () {
	return this->_state;
}

String ProtonixDeviceSensor::Summary () {
	return "[s"
		+ this->_id + ":"
		+ this->_value + ""
		+ (this->_active ? ":active" : "")
		+ (this->_failure ? ":failure" : "")
		+ (this->_state == "" ? "" : ":" + this->_state)
		+ "]";
}

void ProtonixDeviceSensor::Reset () {
	this->Value("");
	this->Active(false);
	this->Failure(false);
	this->State("");
}