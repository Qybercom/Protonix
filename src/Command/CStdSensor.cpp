#include <Arduino.h>

#include "../IProtonixDevice.h"
#include "../ProtonixDevice.h"
#include "../ProtonixDevicePort.h"
#include "../ProtonixDeviceSensor.h"

#if defined(ESP32) || defined(ESP8266)
#include "../DTO/DTOEventCommand.h"
#endif

#include "CStdSensor.h"

using namespace Qybercom::Protonix;



Command::CStdSensor::CStdSensor() {
	this->_init("std:sensor");
	this->Sensor(new ProtonixDeviceSensor());
}

Command::CStdSensor::CStdSensor(ProtonixDeviceSensor* sensor) {
	this->_init("std:sensor");
	this->Sensor(sensor);
}

void Command::CStdSensor::Sensor(ProtonixDeviceSensor* sensor) {
	this->_sensor = sensor;
}

ProtonixDeviceSensor* Command::CStdSensor::Sensor() {
	return this->_sensor;
}

bool Command::CStdSensor::CommandRecognize(ProtonixDevice* device, ProtonixDevicePort* port, String name) {
	(void)device;
    (void)port;

	String n = name.substring(0, 10);
	if (n != this->_name) return false;
	
	unsigned int i = 11;
	unsigned int l = name.length();
	bool setID = false;
	bool setVal = false;

	String id = "";
	String val = "";
	
	while (i < l) {
		if (!setID) {
			if (name[i] != ';') id += name[i];
			else {
				setID = true;

				this->_sensor->ID(id);
			}
		}
		else {
			if (!setVal) {
				if (name[i] != ';') val += name[i];
				else {
					setVal = true;

					this->_sensor->Value(val);
				}
			}
			else {
				this->_sensor->Active(name[i] == '1');
				i += 2;
				this->_sensor->Failure(name[i] == '1');

				break;
			}
		}

		i++;
	}

	return true;
}

bool Command::CStdSensor::CommandSerialize() {
	this->_output = "";
	this->_output += this->_name;
	this->_output += ":";
	this->_output += this->_sensor->ID();
	this->_output += ";";
	this->_output += this->_sensor->Value();
	this->_output += ";";
	this->_output += String(this->_sensor->Active() ? "1" : "0");
	this->_output += ";";
	this->_output += String(this->_sensor->Failure() ? "1" : "0");

	return true;
}

void Command::CStdSensor::CommandReset() {
	this->_output = "";
}

#if defined(ESP32) || defined(ESP8266)
void Command::CStdSensor::CommandFromDTO(DTO::DTOEventCommand* dto) {
	(void)dto;
}
#endif