#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"
#include "../ProtonixSensor.h"

#include "CStdSensor.h"

using namespace Qybercom::Protonix;

Command::CStdSensor::CStdSensor () {
	this->_init("std:sensor");
	this->Sensor(new ProtonixSensor());
}

Command::CStdSensor::CStdSensor (ProtonixSensor* sensor) {
	this->_init("std:sensor");
	this->Sensor(sensor);
}

void Command::CStdSensor::Sensor (ProtonixSensor* sensor) {
	this->_sensor = sensor;
}

ProtonixSensor* Command::CStdSensor::Sensor () {
	return this->_sensor;
}

bool Command::CStdSensor::CommandRecognize (Protonix* device, String command, IProtonixHardware* hardware) {
	(void)device;
	(void)hardware;

	short trail = this->_recognize(command);
	if (trail == -1) return false;
	
	unsigned int i = trail;
	unsigned int l = command.length();

	bool setID = false;
	bool setVal = false;

	String id = "";
	String val = "";
	
	while (i < l) {
		if (!setID) {
			if (command[i] != ';') id += command[i];
			else {
				setID = true;

				this->_sensor->ID(id);
			}
		}
		else {
			if (!setVal) {
				if (command[i] != ';') val += command[i];
				else {
					setVal = true;

					this->_sensor->Value(val);
				}
			}
			else {
				this->_sensor->Active(command[i] == '1');
				i += 2;
				this->_sensor->Failure(command[i] == '1');

				break;
			}
		}

		i++;
	}

	return true;
}

bool Command::CStdSensor::CommandSerialize () {
	this->_buffer = "";
	this->_buffer += this->_name;
	this->_buffer += ":";
	this->_buffer += this->_sensor->ID();
	this->_buffer += ";";
	this->_buffer += this->_sensor->Value();
	this->_buffer += ";";
	this->_buffer += String(this->_sensor->Active() ? "1" : "0");
	this->_buffer += ";";
	this->_buffer += String(this->_sensor->Failure() ? "1" : "0");

	String state = this->_sensor->State();
	if (state != "") {
		this->_buffer += ";";
		this->_buffer += state;
	}

	return true;
}