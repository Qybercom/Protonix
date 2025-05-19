#include <Arduino.h>

#include "ProtonixDeviceStatus.h"
#include "ProtonixDeviceSensor.h"

using namespace Qybercom::Protonix;



ProtonixDeviceStatus::ProtonixDeviceStatus() {
	this->_firmware = "";
	this->_on = true;
	this->_state = "";
	this->_uptime = "";
	this->_summary = "";
	this->_sensorCount = 0;
}

void ProtonixDeviceStatus::Firmware(String firmware) {
	this->_firmware = firmware;
}

String ProtonixDeviceStatus::Firmware() {
	return this->_firmware;
}

void ProtonixDeviceStatus::On(bool on) {
	this->_on = on;
}

bool ProtonixDeviceStatus::On() {
	return this->_on;
}

void ProtonixDeviceStatus::State(String state) {
	this->_state = state;
}

String ProtonixDeviceStatus::State() {
	return this->_state;
}

void ProtonixDeviceStatus::Uptime(String uptime) {
	this->_uptime = uptime;
}

String ProtonixDeviceStatus::Uptime() {
	return this->_uptime;
}

void ProtonixDeviceStatus::Summary(String summary) {
	this->_summary = summary;
}

String ProtonixDeviceStatus::Summary() {
	return this->_summary;
}

ProtonixDeviceSensor** ProtonixDeviceStatus::Sensors() {
	return this->_sensors;
}

unsigned int ProtonixDeviceStatus::SensorCount() {
	return this->_sensorCount;
}

ProtonixDeviceStatus* ProtonixDeviceStatus::SensorAdd(String id) {
	ProtonixDeviceSensor* sensor = new ProtonixDeviceSensor(id);

	this->_sensors[this->_sensorCount] = sensor;
	this->_sensorCount++;

	return this;
}

ProtonixDeviceStatus* ProtonixDeviceStatus::SensorSet(String id, String value) {
	unsigned int i = 0;

	while (i < this->_sensorCount) {
		if (id == "" || this->_sensors[i]->ID() == id) {
			this->_sensors[i]->Value(value);
		}

		i++;
	}

	return this;
}

ProtonixDeviceStatus* ProtonixDeviceStatus::SensorSet(String id, String value, bool active) {
	unsigned int i = 0;

	while (i < this->_sensorCount) {
		if (id == "" || this->_sensors[i]->ID() == id) {
			this->_sensors[i]->Value(value);
			this->_sensors[i]->Active(active);
		}

		i++;
	}

	return this;
}

ProtonixDeviceStatus* ProtonixDeviceStatus::SensorSet(String id, String value, bool active, bool failure) {
	unsigned int i = 0;

	while (i < this->_sensorCount) {
		if (id == "" || this->_sensors[i]->ID() == id) {
			this->_sensors[i]->Value(value);
			this->_sensors[i]->Active(active);
			this->_sensors[i]->Failure(failure);
		}

		i++;
	}

	return this;
}

ProtonixDeviceStatus* ProtonixDeviceStatus::SensorSet(String id, String value, bool active, bool failure, String state) {
	unsigned int i = 0;

	while (i < this->_sensorCount) {
		if (id == "" || this->_sensors[i]->ID() == id) {
			this->_sensors[i]->Value(value);
			this->_sensors[i]->Active(active);
			this->_sensors[i]->Failure(failure);
			this->_sensors[i]->State(state);
		}

		i++;
	}

	return this;
}

ProtonixDeviceStatus* ProtonixDeviceStatus::SensorSet(String id, bool active) {
	unsigned int i = 0;

	while (i < this->_sensorCount) {
		if (id == "" || this->_sensors[i]->ID() == id) {
			this->_sensors[i]->Active(active);
		}

		i++;
	}

	return this;
}

ProtonixDeviceStatus* ProtonixDeviceStatus::SensorSet(String id, bool active, bool failure) {
	unsigned int i = 0;

	while (i < this->_sensorCount) {
		if (id == "" || this->_sensors[i]->ID() == id) {
			this->_sensors[i]->Active(active);
			this->_sensors[i]->Failure(failure);
		}

		i++;
	}

	return this;
}

ProtonixDeviceStatus* ProtonixDeviceStatus::SensorSet(String id, bool active, bool failure, String state) {
	unsigned int i = 0;

	while (i < this->_sensorCount) {
		if (id == "" || this->_sensors[i]->ID() == id) {
			this->_sensors[i]->Active(active);
			this->_sensors[i]->Failure(failure);
			this->_sensors[i]->State(state);
		}

		i++;
	}

	return this;
}

ProtonixDeviceStatus* ProtonixDeviceStatus::SensorSet(String id, String value, String state) {
	unsigned int i = 0;

	while (i < this->_sensorCount) {
		if (id == "" || this->_sensors[i]->ID() == id) {
			this->_sensors[i]->Value(value);
			this->_sensors[i]->State(state);
		}

		i++;
	}

	return this;
}

ProtonixDeviceStatus* ProtonixDeviceStatus::SensorSet(ProtonixDeviceSensor* sensor) {
	return this->SensorSet(
		sensor->ID(),
		sensor->Value(),
		sensor->Active(),
		sensor->Failure(),
		sensor->State()
	);
}

ProtonixDeviceStatus* ProtonixDeviceStatus::SensorReset(String id) {
	unsigned int i = 0;

	while (i < this->_sensorCount) {
		if (id == "" || this->_sensors[i]->ID() == id)
			this->_sensors[i]->Reset();

		i++;
	}

	return this;
}

ProtonixDeviceStatus* ProtonixDeviceStatus::SensorReset() {
	return this->SensorReset("");
}

ProtonixDeviceSensor* ProtonixDeviceStatus::Sensor(String id) {
	unsigned int i = 0;

	while (i < this->_sensorCount) {
		if (this->_sensors[i]->ID() == id)
			return this->_sensors[i];

		i++;
	}

	return nullptr;
}

ProtonixDeviceSensor* ProtonixDeviceStatus::Sensor(unsigned int i) {
	unsigned int j = 0;

	while (j < this->_sensorCount) {
		if (j == i)
			return this->_sensors[j];

		j++;
	}

	return nullptr;
}