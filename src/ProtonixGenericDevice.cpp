#include <Arduino.h>

#include "IProtonixDevice.h"
#include "IProtonixCommand.h"
#include "ProtonixDevice.h"
#include "ProtonixDevicePort.h"

#include "Command/CStdOn.h"
#include "Command/CStdOff.h"
#include "Command/CStdReboot.h"
#include "Command/CStdSensor.h"
#include "Command/CCustom.h"

#if defined(ESP32) || defined(ESP8266)
#include "ProtonixDTO.h"
#include "DTO/DTOResponseAuthorization.h"
#include "DTO/DTOEventCommand.h"
#endif

#include "ProtonixGenericDevice.h"

using namespace Qybercom::Protonix;

void ProtonixGenericDevice::_init() {
	this->_init(false);
}

void ProtonixGenericDevice::_init(bool debug) {
	this->_debug = debug;

	this->_cmds[0] = new Command::CStdOn();
	this->_cmds[1] = new Command::CStdOff();
	this->_cmds[2] = new Command::CStdReboot();
	this->_cmds[3] = new Command::CStdSensor();
	this->_cmds[4] = new Command::CCustom();

	#if defined(ESP32) || defined(ESP8266)
		pinMode(2, OUTPUT);
	#elif defined(LED_BUILTIN)
		pinMode(LED_BUILTIN, OUTPUT);
	#else
	#endif
}

void ProtonixGenericDevice::LED(bool on) {
	if (on) {
		#if defined(ESP32)
			digitalWrite(2, HIGH);
		#elif defined(ESP8266)
			digitalWrite(2, LOW);
		#elif defined(LED_BUILTIN)
			digitalWrite(LED_BUILTIN, HIGH);
		#else
		#endif
	}
	else {
		#if defined(ESP32)
			digitalWrite(2, LOW);
		#elif defined(ESP8266)
			digitalWrite(2, HIGH);
		#elif defined(LED_BUILTIN)
			digitalWrite(LED_BUILTIN, LOW);
		#else
		#endif
	}
}

void ProtonixGenericDevice::DeviceOnSerialCommand(ProtonixDevice* device, ProtonixDevicePort* port, IProtonixCommand* command) {
	this->DeviceOnCommand(device, port, command);
}

bool ProtonixGenericDevice::DeviceAutoStatus() {
	return true;
}



#if defined(ESP32) || defined(ESP8266)
void ProtonixGenericDevice::DeviceOnNetworkConnect(ProtonixDevice* device) {
	if (this->_debug)
		Serial.println("[device] NetworkConnect");
}

void ProtonixGenericDevice::DeviceOnProtocolConnect(ProtonixDevice* device) {
	if (this->_debug)
		Serial.println("[device] ProtocolConnect");
}

void ProtonixGenericDevice::DeviceOnStreamResponse(ProtonixDevice* device, ProtonixDTO* dto) {
	if (this->_debug)
		Serial.println("[device] StreamResponse " + dto->Response());
}

void ProtonixGenericDevice::DeviceOnStreamResponseAuthorization(ProtonixDevice* device, DTO::DTOResponseAuthorization* authorization) {
	// TODO: re-auth if failure
	
	if (this->_debug)
		Serial.println("[device:authorize] " + String(authorization->Status() == 200 ? "Success" : "Failure"));
}

void ProtonixGenericDevice::DeviceOnStreamEvent(ProtonixDevice* device, ProtonixDTO* dto) {
	if (this->_debug)
		Serial.println("[device] StreamEvent " + dto->Event());
}

void ProtonixGenericDevice::DeviceOnStreamEventCommand(ProtonixDevice* device, DTO::DTOEventCommand* command) {
	String cmd = command->Name();

	if (this->_debug)
		Serial.println("[device:command] " + cmd);

	unsigned int i = 0;

	while (i < 5) {
		this->_cmds[i]->CommandReset();
		this->_cmds[i]->CommandFromDTO(command);

		if (this->_cmds[i]->CommandRecognize(device, nullptr, cmd)) {
			this->DeviceOnCommand(device, nullptr, this->_cmds[i]);
		}

		i++;
	}
}
#endif
