#include "ProtonixDeviceSensor.h"

using namespace Qybercom::Protonix;



ProtonixDeviceSensor::ProtonixDeviceSensor() {
	this->ID("");
	this->Value("");
	this->Active(false);
	this->Failure(false);
}

ProtonixDeviceSensor::ProtonixDeviceSensor(String id) {
	this->ID(id);
	this->Value("");
	this->Active(false);
	this->Failure(false);
}

ProtonixDeviceSensor::ProtonixDeviceSensor(String id, String value) {
	this->ID(id);
	this->Value(value);
	this->Active(false);
	this->Failure(false);
}

ProtonixDeviceSensor::ProtonixDeviceSensor(String id, bool active) {
	this->ID(id);
	this->Value("");
	this->Active(active);
	this->Failure(false);
}

ProtonixDeviceSensor::ProtonixDeviceSensor(String id, bool active, bool failure) {
	this->ID(id);
	this->Value("");
	this->Active(active);
	this->Failure(failure);
}

ProtonixDeviceSensor::ProtonixDeviceSensor(String id, String value, bool active) {
	this->ID(id);
	this->Value(value);
	this->Active(active);
	this->Failure(false);
}

ProtonixDeviceSensor::ProtonixDeviceSensor(String id, String value, bool active, bool failure) {
	this->ID(id);
	this->Value(value);
	this->Active(active);
	this->Failure(failure);
}

void ProtonixDeviceSensor::ID(String id) {
	this->_id = id;
}

String ProtonixDeviceSensor::ID() {
	return this->_id;
}

void ProtonixDeviceSensor::Value(String value) {
	this->_value = value;
}

String ProtonixDeviceSensor::Value() {
	return this->_value;
}

void ProtonixDeviceSensor::Active(bool active) {
	this->_active = active;
}

bool ProtonixDeviceSensor::Active() {
	return this->_active;
}

void ProtonixDeviceSensor::Failure(bool failure) {
	this->_failure = failure;
}

bool ProtonixDeviceSensor::Failure() {
	return this->_failure;
}

String ProtonixDeviceSensor::Summary() {
	return "[s"
		+ this->_id + ":"
		+ this->_value + ""
		+ (this->_active ? ":active" : "")
		+ (this->_failure ? ":failure" : "")
		+ "]";
}

void ProtonixDeviceSensor::Reset() {
	this->Value("");
	this->Active(false);
	this->Failure(false);
}