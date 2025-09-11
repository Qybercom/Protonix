#include <Arduino.h>

#include "Common/List.hpp"

#include "ProtonixDeviceStatus.h"
#include "ProtonixDeviceSensor.h"

using namespace Qybercom;
using namespace Qybercom::Protonix;



ProtonixDeviceStatus::ProtonixDeviceStatus () {
	this->_firmware = "";
	this->_on = true;
	this->_state = "";
	this->_uptime = "";
	this->_summary = "";
}

void ProtonixDeviceStatus::Firmware (String firmware) {
	this->_firmware = firmware;
}

String ProtonixDeviceStatus::Firmware () {
	return this->_firmware;
}

void ProtonixDeviceStatus::On (bool on) {
	this->_on = on;
}

bool ProtonixDeviceStatus::On () {
	return this->_on;
}

void ProtonixDeviceStatus::State (String state) {
	this->_state = state;
}

String ProtonixDeviceStatus::State () {
	return this->_state;
}

void ProtonixDeviceStatus::Uptime (String uptime) {
	this->_uptime = uptime;
}

String ProtonixDeviceStatus::Uptime () {
	return this->_uptime;
}

void ProtonixDeviceStatus::Summary (String summary) {
	this->_summary = summary;
}

String ProtonixDeviceStatus::Summary () {
	return this->_summary;
}

List<ProtonixDeviceSensor*> &ProtonixDeviceStatus::Sensors () {
	return this->_sensors;
}

ProtonixDeviceStatus* ProtonixDeviceStatus::SensorAdd (String id) {
	ProtonixDeviceSensor* sensor = new ProtonixDeviceSensor(id);

	this->_sensors.Add(sensor);

	return this;
}

ProtonixDeviceStatus* ProtonixDeviceStatus::SensorSet (String id, String value) {
	for (ProtonixDeviceSensor* sensor : this->_sensors) {
		if (id == "" || sensor->ID() == id) {
			sensor->Value(value);
		}
	}

	return this;
}

ProtonixDeviceStatus* ProtonixDeviceStatus::SensorSet (String id, String value, bool active) {
	for (ProtonixDeviceSensor* sensor : this->_sensors) {
		if (id == "" || sensor->ID() == id) {
			sensor->Value(value);
			sensor->Active(active);
		}
	}

	return this;
}

ProtonixDeviceStatus* ProtonixDeviceStatus::SensorSet (String id, String value, bool active, bool failure) {
	for (ProtonixDeviceSensor* sensor : this->_sensors) {
		if (id == "" || sensor->ID() == id) {
			sensor->Value(value);
			sensor->Active(active);
			sensor->Failure(failure);
		}
	}

	return this;
}

ProtonixDeviceStatus* ProtonixDeviceStatus::SensorSet (String id, String value, bool active, bool failure, String state) {
	for (ProtonixDeviceSensor* sensor : this->_sensors) {
		if (id == "" || sensor->ID() == id) {
			sensor->Value(value);
			sensor->Active(active);
			sensor->Failure(failure);
			sensor->State(state);
		}
	}

	return this;
}

ProtonixDeviceStatus* ProtonixDeviceStatus::SensorSet (String id, bool active) {
	for (ProtonixDeviceSensor* sensor : this->_sensors) {
		if (id == "" || sensor->ID() == id) {
			sensor->Active(active);
		}
	}

	return this;
}

ProtonixDeviceStatus* ProtonixDeviceStatus::SensorSet (String id, bool active, bool failure) {
	for (ProtonixDeviceSensor* sensor : this->_sensors) {
		if (id == "" || sensor->ID() == id) {
			sensor->Active(active);
			sensor->Failure(failure);
		}
	}

	return this;
}

ProtonixDeviceStatus* ProtonixDeviceStatus::SensorSet (String id, bool active, bool failure, String state) {
	for (ProtonixDeviceSensor* sensor : this->_sensors) {
		if (id == "" || sensor->ID() == id) {
			sensor->Active(active);
			sensor->Failure(failure);
			sensor->State(state);
		}
	}

	return this;
}

ProtonixDeviceStatus* ProtonixDeviceStatus::SensorSet (String id, String value, String state) {
	for (ProtonixDeviceSensor* sensor : this->_sensors) {
		if (id == "" || sensor->ID() == id) {
			sensor->Value(value);
			sensor->State(state);
		}
	}

	return this;
}

ProtonixDeviceStatus* ProtonixDeviceStatus::SensorSet (ProtonixDeviceSensor* sensor) {
	return this->SensorSet(
		sensor->ID(),
		sensor->Value(),
		sensor->Active(),
		sensor->Failure(),
		sensor->State()
	);
}

ProtonixDeviceStatus* ProtonixDeviceStatus::SensorReset (String id) {
	for (ProtonixDeviceSensor* sensor : this->_sensors) {
		if (id == "" || sensor->ID() == id) {
			sensor->Reset();
		}
	}

	return this;
}

ProtonixDeviceStatus* ProtonixDeviceStatus::SensorReset () {
	return this->SensorReset("");
}

ProtonixDeviceSensor* ProtonixDeviceStatus::Sensor (String id) {
	for (ProtonixDeviceSensor* sensor : this->_sensors)
		if (sensor->ID() == id) return sensor;

	return nullptr;
}