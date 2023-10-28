#include <Arduino.h>
#include <SoftwareSerial.h>

#include "IProtonixDevice.h"
#include "ProtonixDevice.h"
#include "ProtonixDeviceStatus.h"
#include "ProtonixTimer.h"

#include "Command/CStdSensor.h"
#include "Command/CCustom.h"

#if defined(ESP32) || defined(ESP8266)
#include "DTO/DTORequestAuthorization.h"
#include "DTO/DTOResponseAuthorization.h"
#include "DTO/DTOResponseDeviceStatus.h"
#include "DTO/DTORequestDeviceStatus.h"
#include "DTO/DTOEventCommand.h"
#endif

using namespace Qybercom::Protonix;

ProtonixDevice::ProtonixDevice(IProtonixDevice* device) {
	this->_timer = new ProtonixTimer();
	this->_status = new ProtonixDeviceStatus();
	this->_ready = false;
	this->_portCount = 0;
	this->Device(device);
	this->Debug(false);

	#if defined(ESP32) || defined(ESP8266)
	this->_networkConnected1 = false;
	this->_networkConnected2 = false;
	this->_protocolConnected1 = false;
	this->_protocolConnected2 = false;

	this->_dtoInput = new ProtonixDTO();
	this->_dtoOutput = new ProtonixDTO();
	#endif
}

void ProtonixDevice::Device(IProtonixDevice* device) {
	this->_device = device;
	this->_timer->Interval(this->_device->DeviceTick());
}

IProtonixDevice* ProtonixDevice::Device() {
	return this->_device;
}

ProtonixTimer* ProtonixDevice::Timer() {
	return this->_timer;
}

ProtonixDeviceStatus* ProtonixDevice::Status() {
	return this->_status;
}

void ProtonixDevice::Summary(String additional) {
	String summary = "[on:" + String(this->_status->On() ? "yes" : "no") + "]";

	unsigned int i = 0;
	unsigned int count = this->_status->SensorCount();

	while (i < count) {
		summary += " " + this->_status->Sensors()[i]->Summary();

		i++;
	}

	this->_status->Summary(summary + additional);
}

bool ProtonixDevice::Ready() {
	return this->_ready;
}

unsigned int ProtonixDevice::Tick() {
	return this->_device->DeviceTick();
}

void ProtonixDevice::Debug(bool debug) {
	this->_debug = debug;
}

bool ProtonixDevice::Debug() {
	return this->_debug;
}

extern int __heap_start, * __brkval;

int ProtonixDevice::FreeRAM() {
	#if defined(ESP32)
		return esp_get_free_heap_size();
	#elif defined(ESP8266)
		return ESP.getFreeHeap();
	#elif defined(AVR)
		// https://docs.arduino.cc/learn/programming/memory-guide
		int v;
		//int __heap_start, * __brkval;

		return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
	#else
		return 0;
	#endif
}

ProtonixDevice* ProtonixDevice::Port(ProtonixDevicePort* port) {
	this->_ports[this->_portCount] = port;
	this->_portCount++;

	return this;
}

ProtonixDevice* ProtonixDevice::Port(String name, unsigned int pinTX, unsigned int pinRX) {
	return this->Port(new ProtonixDevicePort(name, pinTX, pinRX));
}

ProtonixDevice* ProtonixDevice::Port(String name, unsigned int pinTX, unsigned int pinRX, unsigned int speed) {
	return this->Port(new ProtonixDevicePort(name, pinTX, pinRX, speed));
}

ProtonixDevice* ProtonixDevice::Port(String name, unsigned int pinTX, unsigned int pinRX, unsigned int speed, unsigned int timeout) {
	return this->Port(new ProtonixDevicePort(name, pinTX, pinRX, speed, timeout));
}

ProtonixDevice* ProtonixDevice::Port(String name) {
	return this->Port(new ProtonixDevicePort(name));
}

void ProtonixDevice::_pipeNetwork() {
	#if defined(ESP32) || defined(ESP8266)
	if (!this->_networkConnected1 || !this->_networkConnected2) {
		if (!this->_networkConnected1) {
			if (this->_debug)
				Serial.println("[network:connect]");

			this->_network->Connect();
			this->_networkConnected1 = true;
		}

		if (!this->_network->Connected()) return;

		if (this->_debug)
			Serial.println("[network:connected]");

		this->_networkConnected2 = true;

		this->_device->DeviceOnNetworkConnect(this);
	}

	if (!this->_protocolConnected1 || !this->_protocolConnected2) {
		if (!this->_protocolConnected1) {
			if (this->_debug)
				Serial.println("[protocol:connect]");

			this->_protocol->Init(this);
			this->_protocol->Connect(this->_uri);
			this->_protocolConnected1 = true;
		}

		if (!this->_protocol->Connected()) return;

		if (this->_debug)
			Serial.println("[protocol:connected]");

		this->_protocolConnected2 = true;

		this->_device->DeviceOnProtocolConnect(this);

		this->RequestStreamAuthorize();
	}

	if (!this->Connected()) {
		bool onlineN = this->_network->Connected();
		bool onlineP = this->_protocol->Connected();

		Serial.println("[WARNING] RECONNECT N:" + String(onlineN) + " P:" + String(onlineP));

		if (!onlineN) {
			this->_networkConnected1 = false;
			this->_networkConnected2 = false;
		}

		if (!onlineP) {
			this->_protocolConnected1 = false;
			this->_protocolConnected2 = false;
		}

		return;
	}
	
	this->_protocol->Pipe();
	#endif
}

void ProtonixDevice::Pipe() {
	unsigned int i = 0;

	if (!this->_ready) {
		this->_ready = true;

		i = 0;
		while (i < this->_portCount) {
			this->_ports[i]->Init(this);

			i++;
		}

		this->_device->DeviceOnReady(this);
	}
	
	this->_pipeNetwork();

	i = 0;

	while (i < this->_portCount) {
		this->_ports[i]->Pipe(this);

		i++;
	}

	if (this->_timer->Pipe()) {
		this->_device->DeviceOnTick(this);

		if (this->_device->DeviceAutoStatus()) {
			#if defined(ESP32) || defined(ESP8266)
			this->RequestStream("/api/mechanism/status", new DTO::DTORequestDeviceStatus(this->_status));
			#endif
		}
	}
}

void ProtonixDevice::OnSerial(ProtonixDevicePort* port, IProtonixCommand* command) {
	this->_device->DeviceOnSerialCommand(this, port, command);
}

bool ProtonixDevice::SerialCommand(String port, IProtonixCommand* command) {
	unsigned int i = 0;
	bool ok = true;

	while (i < this->_portCount) {
		if (this->_ports[i]->Name() == port)
			ok &= this->_ports[i]->Send(command);

		i++;
	}

	return ok;
}

bool ProtonixDevice::SerialCommandCustom(String port, String command) {
	Command::CCustom* cmd = new Command::CCustom(command);

	bool ok = this->SerialCommand(port, cmd);

	delete cmd;
	cmd = nullptr;

	return ok;
}

bool ProtonixDevice::SerialCommandSensor(String port, ProtonixDeviceSensor* sensor) {
	Command::CStdSensor* cmd = new Command::CStdSensor(sensor);

	bool ok = this->SerialCommand(port, cmd);

	delete cmd;
	cmd = nullptr;

	return ok;
}

bool ProtonixDevice::SerialCommandSensor(String port, String id, String value) {
	ProtonixDeviceSensor* sensor = new ProtonixDeviceSensor(id, value);
	
	bool ok = this->SerialCommandSensor(port, sensor);
	
	delete sensor;
	sensor = nullptr;

	return ok;
}

bool ProtonixDevice::SerialCommandSensor(String port, String id, String value, bool active) {
	ProtonixDeviceSensor* sensor = new ProtonixDeviceSensor(id, value, active);
	
	bool ok = this->SerialCommandSensor(port, sensor);
	
	delete sensor;
	sensor = nullptr;

	return ok;
}

bool ProtonixDevice::SerialCommandSensor(String port, String id, String value, bool active, bool failure) {
	ProtonixDeviceSensor* sensor = new ProtonixDeviceSensor(id, value, active, failure);
	
	bool ok = this->SerialCommandSensor(port, sensor);
	
	delete sensor;
	sensor = nullptr;

	return ok;
}

bool ProtonixDevice::SerialCommandSensor(String port, String id, bool active) {
	ProtonixDeviceSensor* sensor = new ProtonixDeviceSensor(id, active);
	
	bool ok = this->SerialCommandSensor(port, sensor);
	
	delete sensor;
	sensor = nullptr;

	return ok;
}

bool ProtonixDevice::SerialCommandSensor(String port, String id, bool active, bool failure) {
	ProtonixDeviceSensor* sensor = new ProtonixDeviceSensor(id, active, failure);
	
	bool ok = this->SerialCommandSensor(port, sensor);
	
	delete sensor;
	sensor = nullptr;

	return ok;
}

bool ProtonixDevice::SerialStatus(String port) {
	unsigned count = this->_status->SensorCount();
	unsigned int i = 0;
	bool ok = true;

	while (i < count) {
		this->SerialCommandSensor(port, this->_status->Sensors()[i]);

		i++;
	}

	return ok;
}

#if defined(ESP32) || defined(ESP8266)
void ProtonixDevice::Network(IProtonixNetwork* network) {
	this->_network = network;
}

IProtonixNetwork* ProtonixDevice::Network() {
	return this->_network;
}

void ProtonixDevice::Protocol(IProtonixProtocol* protocol) {
	this->_protocol = protocol;
}

IProtonixProtocol* ProtonixDevice::Protocol() {
	return this->_protocol;
}

void ProtonixDevice::Server(ProtonixURI* uri) {
	this->_uri = uri;
}

ProtonixURI* ProtonixDevice::Server() {
	return this->_uri;
}

void ProtonixDevice::ServerEndpoint(String host, unsigned int port) {
	this->Server(new ProtonixURI(host, port));
}

void ProtonixDevice::ServerEndpoint(String host, unsigned int port, String path) {
	this->Server(new ProtonixURI(host, port, path));
}

bool ProtonixDevice::Connected() {
	return this->_network->Connected() && this->_protocol->Connected();
}

void ProtonixDevice::RequestStream(String url, IProtonixDTORequest* request) {
	if (!this->Connected()) {
		if (this->_debug)
			Serial.println("[WARNING] Device not connected, can not send request");

		return;
	}

	this->_dtoOutput->Debug(this->_debug);
	this->_dtoOutput->URL(url);
	this->_dtoOutput->DTO(request);

	if (!this->_dtoOutput->Serialize()) {
		if (this->_debug)
			Serial.println("[WARNING] Cannot serialize request");

		return;
	}

	this->_protocol->Send(this->_dtoOutput->BufferRaw());

	if (this->_debug)
		Serial.println("[request] " + url + " :: '" + this->_dtoOutput->BufferRaw() + "'");

	delete request;
	request = nullptr;

	this->_dtoOutput->Reset();
}

void ProtonixDevice::RequestStreamAuthorize() {
	this->RequestStream("/api/authorize/mechanism", new DTO::DTORequestAuthorization(
		this->_device->DeviceID(),
		this->_device->DevicePassphrase()
	));
}

void ProtonixDevice::OnStream(unsigned char* data) {
	if (this->_debug)
		Serial.println("[OnStream] " + String((char*)data));

	this->_dtoInput->Debug(this->_debug);
	this->_dtoInput->BufferRaw((char*)data);
	this->_dtoInput->Deserialize();

	if (this->_dtoInput->IsURL())
		this->_onStreamURL();

	if (this->_dtoInput->IsResponse())
		this->_onStreamResponse();

	if (this->_dtoInput->IsEvent())
		this->_onStreamEvent();

	this->_dtoInput->Reset();
}

void ProtonixDevice::_onStreamURL() {
	if (this->_debug)
		Serial.println("[url] " + this->_dtoInput->URL());
}

void ProtonixDevice::_onStreamResponse() {
	if (this->_debug)
		Serial.println("[response] " + this->_dtoInput->Response());

	this->_device->DeviceOnStreamResponse(this, this->_dtoInput);

	if (this->_dtoInput->Response() == "/api/authorize/mechanism") {
		DTO::DTOResponseAuthorization* dto = new DTO::DTOResponseAuthorization();
		dto->DTOPopulate(this->_dtoInput);

		this->_device->DeviceOnStreamResponseAuthorization(this, dto);

		delete dto;
		dto = nullptr;
	}

	if (this->_dtoInput->Response() == "/api/mechanism/status") {
		DTO::DTOResponseDeviceStatus* dto = new DTO::DTOResponseDeviceStatus();
		dto->DTOPopulate(this->_dtoInput);

		if (dto->Status() != 200)
			this->RequestStreamAuthorize();

		delete dto;
		dto = nullptr;
	}
}

void ProtonixDevice::_onStreamEvent() {
	if (this->_debug)
		Serial.println("[event] " + this->_dtoInput->Event());

	this->_device->DeviceOnStreamEvent(this, this->_dtoInput);

	if (this->_dtoInput->Event() == "/api/mechanism/command/" + this->_device->DeviceID()) {
		DTO::DTOEventCommand* dto = new DTO::DTOEventCommand();
		dto->DTOPopulate(this->_dtoInput);

		this->_device->DeviceOnStreamEventCommand(this, dto);

		delete dto;
		dto = nullptr;
	}
}

ProtonixDTO* ProtonixDevice::DTOInput() {
	return this->_dtoInput;
}

ProtonixDTO* ProtonixDevice::DTOOutput() {
	return this->_dtoOutput;
}

#endif