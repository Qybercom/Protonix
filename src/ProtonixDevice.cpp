#include <Arduino.h>
#include <SPI.h>
#include <SoftwareSerial.h>

#if defined(ESP32) || defined(ESP8266)
#include <StreamString.h>
#endif

#if defined(ESP8266)
#include <flash_hal.h>
#endif

#if defined(ESP8266)
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#elif defined(ESP32)
#include <ESP32httpUpdate.h>
#endif

#include "Common/Utils.hpp"

#include "IProtonixDevice.h"
#include "ProtonixDevice.h"
#include "ProtonixDeviceStatus.h"
#include "ProtonixTimer.h"
#include "ProtonixAction.h"
#include "ProtonixMemory.h"

#include "Hardware/HMesonix.h"

#include "Command/CStdSensor.h"
#include "Command/CCustom.h"

#if defined(ESP32) || defined(ESP8266)
#include "ProtonixHTTPClient.h"

#include "DTO/DTORequestAuthorization.h"
#include "DTO/DTOResponseAuthorization.h"
#include "DTO/DTOResponseDeviceStatus.h"
#include "DTO/DTORequestDeviceStatus.h"
#include "DTO/DTOEventCommand.h"
#endif

using namespace Qybercom;
using namespace Qybercom::Protonix;

List<ProtonixDevice*> ProtonixDevice::_instances;

ProtonixDevice::ProtonixDevice (IProtonixDevice* device) {
	this->_timerUptime = new ProtonixTimer(0);
	this->_timerTick = new ProtonixTimer(0);
	this->_status = new ProtonixDeviceStatus();
	this->_ready = false;

	this->Device(device);
	this->Debug(false);

	this->_memory = new ProtonixMemory();
	this->_memory->EEPROMBegin();
	this->_registry = new ProtonixRegistry(this->_memory);
	this->_registry->Debug(this->_debug); // TODO: runtime switch for every debuggable component

	#if defined(ESP32) || defined(ESP8266)
	this->_timerNetwork = new ProtonixTimer(1000); // TODO: refactor for custom interval
	this->_timerNetworkAuthorize = new ProtonixTimer(1000, false); // TODO: refactor for custom interval
	this->_timerNetworkStatus = new ProtonixTimer(100); // TODO: refactor for custom interval

	this->_networkConnected1 = false;
	this->_networkConnected2 = false;
	this->_protocolConnected1 = false;
	this->_protocolConnected2 = false;
	this->_authorized = false;

	this->_dtoInput = new ProtonixDTO();
	this->_dtoOutput = new ProtonixDTO();
	#endif

	#if defined(AVR)
	pinMode(4, OUTPUT);
	digitalWrite(4, HIGH);
	#endif

	_instances.Add(this);
}

void ProtonixDevice::_interruptProcess () {
	for (IProtonixHardware* hardware : this->_hardware)
		hardware->HardwarePipeInterrupt(this);
}

void ProtonixDevice::_interrupt () {
	for (ProtonixDevice* instance : _instances)
		instance->_interruptProcess();
}

void ProtonixDevice::Device (IProtonixDevice* device) {
	this->_device = device;
	this->_timerTick->Interval(this->_device->DeviceTick());
}

IProtonixDevice* ProtonixDevice::Device () {
	return this->_device;
}

void ProtonixDevice::InterruptAttach (unsigned short pin, int mode) {
	attachInterrupt(digitalPinToInterrupt(pin), _interrupt, mode);
}

void ProtonixDevice::InterruptDetach (unsigned short pin) {
	detachInterrupt(digitalPinToInterrupt(pin));
}

ProtonixTimer* ProtonixDevice::TimerUptime () {
	return this->_timerUptime;
}

ProtonixTimer* ProtonixDevice::TimerTick () {
	return this->_timerTick;
}

#if defined(ESP32) || defined(ESP8266)
ProtonixTimer* ProtonixDevice::TimerNetwork () {
	return this->_timerNetwork;
}

ProtonixTimer* ProtonixDevice::TimerNetworkAuthorize () {
	return this->_timerNetworkAuthorize;
}

ProtonixTimer* ProtonixDevice::TimerNetworkStatus () {
	return this->_timerNetworkStatus;
}
#endif

ProtonixDeviceStatus* ProtonixDevice::Status () {
	return this->_status;
}

void ProtonixDevice::Summary (String additional) {
	String state = this->_status->State();
	String summary = ""
		 + String("[on:") + String(this->_status->On() ? "yes" : "no") + String("] ")
		 + String("[uptime:") + String(this->_status->Uptime()) + String("] ")
		 + String("[memory:") + String(this->FreeRAM()) + String("] ")
		 + String("[state:" + state + "]");

	List<ProtonixDeviceSensor*> &sensors = this->_status->Sensors();

	for (ProtonixDeviceSensor* sensor : sensors)
		summary += " " + sensor->Summary();

	this->_status->Summary(summary + additional);
}

bool ProtonixDevice::Ready () {
	return this->_ready;
}

unsigned int ProtonixDevice::Tick () {
	return this->_device->DeviceTick();
}

void ProtonixDevice::Debug (bool debug) {
	this->_debug = debug;
}

bool ProtonixDevice::Debug () {
	return this->_debug;
}

ProtonixMemory* ProtonixDevice::Memory () {
	return this->_memory;
}

ProtonixRegistry* ProtonixDevice::Registry () {
	return this->_registry;
}

extern int __heap_start, * __brkval;

int ProtonixDevice::FreeRAM () {
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

int ProtonixDevice::FreeFlash () {
	#if defined(ESP32) || defined(ESP8266)
	return ESP.getFreeSketchSpace();
	#elif defined(AVR)
	return 0;
	#else
	return 0;
	#endif
}

void ProtonixDevice::Reboot () {
	#if defined(ESP32) || defined(ESP8266)
	ESP.restart();
	#elif defined(AVR)
	digitalWrite(4, LOW);
	#else
	#endif
}

List<IProtonixHardware*> &ProtonixDevice::Hardware () {
	return this->_hardware;
}

IProtonixHardware* ProtonixDevice::Hardware (String id) {
	for (IProtonixHardware* hardware : this->_hardware)
		if (hardware->HardwareID() == id) return hardware;

	return nullptr;
}

ProtonixDevice* ProtonixDevice::Hardware (String id, IProtonixHardware* hardware) {
	hardware->HardwareID(id);

	this->_hardware.Add(hardware);

	return this;
}

List<ProtonixDevicePort*> &ProtonixDevice::Ports () {
	return this->_ports;
}

ProtonixDevicePort* ProtonixDevice::Port (String name) {
	for (ProtonixDevicePort* port : this->_ports)
		if (port->Name() == name) return port;

	return nullptr;
}

ProtonixDevicePort* ProtonixDevice::Port (ProtonixDevicePort* port) {
	this->_ports.Add(port);

	return port;
}

ProtonixDevicePort* ProtonixDevice::Port (String name, unsigned int pinTX, unsigned int pinRX) {
	return this->Port(new ProtonixDevicePort(name, pinTX, pinRX));
}

ProtonixDevicePort* ProtonixDevice::Port (String name, unsigned int pinTX, unsigned int pinRX, bool blocking) {
	return this->Port(new ProtonixDevicePort(name, pinTX, pinRX, blocking));
}

ProtonixDevicePort* ProtonixDevice::Port (String name, unsigned int pinTX, unsigned int pinRX, bool blocking, bool observable) {
	return this->Port(new ProtonixDevicePort(name, pinTX, pinRX, blocking, observable));
}

ProtonixDevicePort* ProtonixDevice::Port (String name, unsigned int pinTX, unsigned int pinRX, unsigned int speed) {
	return this->Port(new ProtonixDevicePort(name, pinTX, pinRX, speed));
}

ProtonixDevicePort* ProtonixDevice::Port (String name, unsigned int pinTX, unsigned int pinRX, unsigned int speed, bool blocking) {
	return this->Port(new ProtonixDevicePort(name, pinTX, pinRX, speed, blocking));
}

ProtonixDevicePort* ProtonixDevice::Port (String name, unsigned int pinTX, unsigned int pinRX, unsigned int speed, bool blocking, bool observable) {
	return this->Port(new ProtonixDevicePort(name, pinTX, pinRX, speed, blocking, observable));
}

ProtonixDevicePort* ProtonixDevice::Port (String name, unsigned int pinTX, unsigned int pinRX, unsigned int speed, unsigned int timeout) {
	return this->Port(new ProtonixDevicePort(name, pinTX, pinRX, speed, timeout));
}

ProtonixDevicePort* ProtonixDevice::Port (String name, unsigned int pinTX, unsigned int pinRX, unsigned int speed, unsigned int timeout, bool blocking) {
	return this->Port(new ProtonixDevicePort(name, pinTX, pinRX, speed, timeout, blocking));
}

ProtonixDevicePort* ProtonixDevice::Port (String name, unsigned int pinTX, unsigned int pinRX, unsigned int speed, unsigned int timeout, bool blocking, bool observable) {
	return this->Port(new ProtonixDevicePort(name, pinTX, pinRX, speed, timeout, blocking, observable));
}

ProtonixDevicePort* ProtonixDevice::PortDefault () {
	return this->Port(new ProtonixDevicePort());
}

ProtonixDevicePort* ProtonixDevice::PortDefault (bool blocking) {
	return this->Port(new ProtonixDevicePort(blocking));
}

ProtonixDevicePort* ProtonixDevice::PortDefault (bool blocking, bool observable) {
	return this->Port(new ProtonixDevicePort(blocking, observable));
}

ProtonixDevicePort* ProtonixDevice::PortDefault (unsigned int speed) {
	return this->Port(new ProtonixDevicePort(speed));
}

ProtonixDevicePort* ProtonixDevice::PortDefault (unsigned int speed, bool blocking) {
	return this->Port(new ProtonixDevicePort(speed, blocking));
}

ProtonixDevicePort* ProtonixDevice::PortDefault (unsigned int speed, bool blocking, bool observable) {
	return this->Port(new ProtonixDevicePort(speed, blocking, observable));
}

ProtonixDevicePort* ProtonixDevice::PortDefault (unsigned int speed, unsigned int timeout) {
	return this->Port(new ProtonixDevicePort(speed, timeout));
}

ProtonixDevicePort* ProtonixDevice::PortDefault (unsigned int speed, unsigned int timeout, bool blocking) {
	return this->Port(new ProtonixDevicePort(speed, timeout, blocking));
}

ProtonixDevicePort* ProtonixDevice::PortDefault (unsigned int speed, unsigned int timeout, bool blocking, bool observable) {
	return this->Port(new ProtonixDevicePort(speed, timeout, blocking, observable));
}

List<ProtonixAction*> &ProtonixDevice::Actions () {
	return this->_actions;
}

ProtonixAction* ProtonixDevice::Action (String name) {
	for (ProtonixAction* action : this->_actions)
		if (action->Name() == name) return action;

	return nullptr;
}

ProtonixAction* ProtonixDevice::ActionRegister (ProtonixAction* action) {
	this->_actions.Add(action);

	return action;
}

ProtonixAction* ProtonixDevice::ActionRegister (String name) {
	return this->ActionRegister(new ProtonixAction(name));
}

ProtonixAction* ProtonixDevice::ActionRegister (String name, unsigned int interval) {
	return this->ActionRegister(new ProtonixAction(name, interval));
}

ProtonixAction* ProtonixDevice::ActionRegister (String name, unsigned int interval, int stepEnd) {
	return this->ActionRegister(new ProtonixAction(name, interval, stepEnd));
}

ProtonixAction* ProtonixDevice::ActionRegister (String name, unsigned int interval, int stepBegin, int stepEnd) {
	return this->ActionRegister(new ProtonixAction(name, interval, stepBegin, stepEnd));
}

ProtonixAction* ProtonixDevice::ActionRegister (String name, unsigned int interval, int stepBegin, int stepEnd, int step) {
	return this->ActionRegister(new ProtonixAction(name, interval, stepBegin, stepEnd, step));
}

bool ProtonixDevice::ActionTrigger (String name) {
	ProtonixAction* action = this->Action(name);
	if (action == nullptr) return false;

	if (this->_debug)
		Serial.println("[action] Triggered '" + name + "' (" + String(action->Summary()) + ")");

	/*action->Start();
	action->Queued(true);*/

	this->_actionQueue.Add(name);

	return true;
}

bool ProtonixDevice::ActionStart (String name) {
	ProtonixAction* action = this->Action(name);
	if (action == nullptr) return false;

	if (this->_debug)
		Serial.println("[action] Started '" + name + "' (" + String(action->Summary()) + ")");

	action->Start();

	return true;
}

bool ProtonixDevice::ActionPlay (String name) {
	ProtonixAction* action = this->Action(name);
	if (action == nullptr) return false;

	if (this->_debug)
		Serial.println("[action] Played '" + name + "' (" + String(action->Summary()) + ")");

	action->Play();

	return true;
}

bool ProtonixDevice::ActionPause (String name) {
	ProtonixAction* action = this->Action(name);
	if (action == nullptr) return false;

	if (this->_debug)
		Serial.println("[action] Paused '" + name + "' (" + String(action->Summary()) + ")");

	action->Pause();

	return true;
}

bool ProtonixDevice::ActionStop (String name) {
	ProtonixAction* action = this->Action(name);
	if (action == nullptr) return false;

	if (this->_debug)
		Serial.println("[action] Stopped '" + name + "' (" + String(action->Summary()) + ")");

	action->Stop();

	return true;
}

bool ProtonixDevice::ActionPipe (ProtonixAction* action) {
	if (action == nullptr) return false;
	if (!action->PipePre()) return false;

	this->_device->DeviceOnAction(this, action);

	action->PipePost();

	return true;
}

void ProtonixDevice::ActionReset () {
	ProtonixAction* action = nullptr;

	for (String act : this->_actionQueue) {
		action = this->Action(act);

		if (action != nullptr)
			action->Reset();
	}

	this->_actionQueue.Clear();

	/*for (ProtonixAction* action : this->_actions)
		action->Reset();*/
}

#if defined(ESP32) || defined(ESP8266)
void ProtonixDevice::_pipeNetwork () {
	if (this->_network == nullptr || this->_protocol == nullptr) return;
		
	if (!this->Connected()) {
		this->_timerNetwork->Enabled(true);

		this->_authorized = false;
	}
	
	if (this->_timerNetwork->Pipe()) {
		if (!this->_networkConnected1 || !this->_networkConnected2) {
			if (!this->_networkConnected1) {
				if (this->_debug)
					Serial.println("[network:connect 1]");

				this->_network->NetworkConnect();

				if (this->_debug)
					Serial.println("[network:connect 2]");

				this->_networkConnected1 = true;
			}

			if (!this->_network->NetworkConnected()) return;

			if (this->_debug)
				Serial.println("[network:connected]");

			this->_networkConnected2 = true;

			this->_device->DeviceOnNetworkConnect(this);
		}

		if (!this->_protocolConnected1 || !this->_protocolConnected2) {
			if (!this->_protocolConnected1) {
				if (this->_debug)
					Serial.println("[protocol:connect 1]");

				this->_protocol->ProtocolInit(this);

				if (this->_debug)
					Serial.println("[protocol:connect 2]");

				this->_protocol->ProtocolConnect(this->_uri);

				if (this->_debug)
					Serial.println("[protocol:connect 3]");
			}

			if (!this->_protocol->ProtocolConnected()) return;

			this->_protocolConnected1 = true;

			if (this->_debug)
				Serial.println("[protocol:connected]");

			this->_protocolConnected2 = true;

			this->_device->DeviceOnProtocolConnect(this);
		}

		if (!this->Connected()) {
			bool onlineN = this->_network->NetworkConnected();
			bool onlineP = this->_protocol->ProtocolConnected();

			Serial.println("[info] Reconnect N:" + String(onlineN) + " P:" + String(onlineP));

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

		this->_timerNetwork->Enabled(false);
		this->_timerNetworkAuthorize->Enabled(true);
	}

	if (this->_timerNetworkAuthorize->Pipe()) {
		this->RequestStreamAuthorize();
	}

	this->_protocol->ProtocolPipe();
}
#endif

void ProtonixDevice::_pipeActions () {
	if (this->_actionQueue.Count() != 0) {
		ProtonixAction* action = this->Action(this->_actionQueue.First());

		if (action != nullptr) {
			if (!action->Queued()) {
				action->Queued(true);
				action->Start();
			}

			this->ActionPipe(action);

			if (action->Completed()) {
				action->Reset();

				this->_actionQueue.PopFirst();
			}
		}
	}

	for (ProtonixAction* action : this->_actions) {
		if (action->Queued()) continue;

		this->ActionPipe(action);
	}
}

void ProtonixDevice::Pipe () {
	this->_timerUptime->Pipe();
	this->_status->Uptime(this->_timerUptime->RunTime());

	if (!this->_ready) {
		this->_ready = true;

		for (ProtonixDevicePort* port : this->_ports)
			port->Init(this);

		bool spi = false;
		short core = -1;

		for (IProtonixHardware* hardware : this->_hardware) {
			spi |= hardware->HardwareSPI();

			#if defined(ESP32)
			core = hardware->HardwareDedicatedCore();
			if (core == 0) this->DedicateTaskCore0();
			if (core == 1) this->DedicateTaskCore1();
			#endif

			hardware->HardwareInitPre(this);
		}

		if (spi)
			SPI.begin();

		for (IProtonixHardware* hardware : this->_hardware) {
			hardware->HardwareInitPost(this);
		}

		this->_device->DeviceOnReady(this);
	}

	#if defined(ESP32) || defined(ESP8266)
	this->_pipeNetwork();
	#endif

	for (IProtonixHardware* hardware : this->_hardware) {
		#if defined(ESP32)
		if (hardware->HardwareDedicatedCore() != -1) continue;
		#endif

		hardware->HardwarePipe(this, -1);
	}

	for (ProtonixDevicePort* port : this->_ports)
		port->Pipe(this);

	this->_device->DeviceOnLoop(this);

	if (this->_timerTick->Pipe()) {
		this->_device->DeviceOnTick(this);
	}

	#if defined(ESP32) || defined(ESP8266)
	if (this->_timerNetworkStatus->Pipe()) {
		if (this->_device->DeviceAutoStatus() && this->_authorized) {
			this->RequestStream("/api/mechanism/status", new DTO::DTORequestDeviceStatus(this->_status, this->_registry->Raw()));
		}
	}
	#endif

	this->_pipeActions();
}

void ProtonixDevice::OnSerial (ProtonixDevicePort* port, IProtonixCommand* command) {
	this->_device->DeviceOnSerialCommand(this, port, command);
}

bool ProtonixDevice::SerialCommand (String port, IProtonixCommand* command) {
	bool ok = true;

	for (ProtonixDevicePort* port : this->_ports)
		ok &= port->Send(command);

	return ok;
}

bool ProtonixDevice::SerialCommandCustom (String port, String command) {
	Command::CCustom* cmd = new Command::CCustom(command);

	bool ok = this->SerialCommand(port, cmd);

	delete cmd;
	cmd = nullptr;

	return ok;
}

bool ProtonixDevice::SerialCommandSensor (String port, ProtonixDeviceSensor* sensor) {
	Command::CStdSensor* cmd = new Command::CStdSensor(sensor);

	bool ok = this->SerialCommand(port, cmd);

	delete cmd;
	cmd = nullptr;

	return ok;
}

bool ProtonixDevice::SerialCommandSensor (String port, String id, String value) {
	ProtonixDeviceSensor* sensor = new ProtonixDeviceSensor(id, value);

	bool ok = this->SerialCommandSensor(port, sensor);

	delete sensor;
	sensor = nullptr;

	return ok;
}

bool ProtonixDevice::SerialCommandSensor (String port, String id, String value, bool active) {
	ProtonixDeviceSensor* sensor = new ProtonixDeviceSensor(id, value, active);

	bool ok = this->SerialCommandSensor(port, sensor);

	delete sensor;
	sensor = nullptr;

	return ok;
}

bool ProtonixDevice::SerialCommandSensor (String port, String id, String value, bool active, bool failure) {
	ProtonixDeviceSensor* sensor = new ProtonixDeviceSensor(id, value, active, failure);

	bool ok = this->SerialCommandSensor(port, sensor);

	delete sensor;
	sensor = nullptr;

	return ok;
}

bool ProtonixDevice::SerialCommandSensor (String port, String id, String value, bool active, bool failure, String state) {
	ProtonixDeviceSensor* sensor = new ProtonixDeviceSensor(id, value, active, failure, state);

	bool ok = this->SerialCommandSensor(port, sensor);

	delete sensor;
	sensor = nullptr;

	return ok;
}

bool ProtonixDevice::SerialCommandSensor (String port, String id, bool active) {
	ProtonixDeviceSensor* sensor = new ProtonixDeviceSensor(id, active);

	bool ok = this->SerialCommandSensor(port, sensor);

	delete sensor;
	sensor = nullptr;

	return ok;
}

bool ProtonixDevice::SerialCommandSensor (String port, String id, bool active, bool failure) {
	ProtonixDeviceSensor* sensor = new ProtonixDeviceSensor(id, active, failure);

	bool ok = this->SerialCommandSensor(port, sensor);

	delete sensor;
	sensor = nullptr;

	return ok;
}

bool ProtonixDevice::SerialCommandSensor (String port, String id, bool active, bool failure, String state) {
	ProtonixDeviceSensor* sensor = new ProtonixDeviceSensor(id, active, failure, state);

	bool ok = this->SerialCommandSensor(port, sensor);

	delete sensor;
	sensor = nullptr;

	return ok;
}

bool ProtonixDevice::SerialCommandSensor (String port, String id, String value, String state) {
	ProtonixDeviceSensor* sensor = new ProtonixDeviceSensor(id, value, state);

	bool ok = this->SerialCommandSensor(port, sensor);

	delete sensor;
	sensor = nullptr;

	return ok;
}

bool ProtonixDevice::SerialStatus (String port) {
	bool ok = true;

	List<ProtonixDeviceSensor*> &sensors = this->_status->Sensors();

	for (ProtonixDeviceSensor* sensor : sensors)
		ok &= this->SerialCommandSensor(port, sensor);

	return ok;
}

#if defined(ESP32) || defined(ESP8266)
void ProtonixDevice::Network (IProtonixNetwork* network) {
	this->_network = network;
}

IProtonixNetwork* ProtonixDevice::Network () {
	return this->_network;
}

bool ProtonixDevice::NetworkConnected () {
	return this->_networkConnected1 && this->_networkConnected2;
}

void ProtonixDevice::Protocol (IProtonixProtocol* protocol) {
	this->_protocol = protocol;
}

IProtonixProtocol* ProtonixDevice::Protocol () {
	return this->_protocol;
}

bool ProtonixDevice::ProtocolConnected () {
	return this->_protocolConnected1 && this->_protocolConnected2;
}

void ProtonixDevice::Server (ProtonixURI* uri) {
	this->_uri = uri;
}

ProtonixURI* ProtonixDevice::Server () {
	return this->_uri;
}

void ProtonixDevice::ServerEndpoint (String host, unsigned int port) {
	this->Server(new ProtonixURI(host, port));
}

void ProtonixDevice::ServerEndpoint (String host, unsigned int port, String path) {
	this->Server(new ProtonixURI(host, port, path));
}

void ProtonixDevice::ServerBaseURI (String uri) {
	this->_serverBaseURI = uri;
}

bool ProtonixDevice::Connected () {
	return true
		&& this->_network != nullptr
		&& this->_protocol != nullptr
		&& this->_network->NetworkConnected()
		&& this->_protocol->ProtocolConnected();
}

void ProtonixDevice::RequestStream (String url, IProtonixDTORequest* request) {
	String error = "";

	if (this->Connected()) {
		this->_dtoOutput->Debug(this->_debug);
		this->_dtoOutput->URL(url);
		this->_dtoOutput->DTO(request);

		if (this->_dtoOutput->Serialize()) this->_protocol->ProtocolSend(this->_dtoOutput->BufferRaw());
		else error = "Cannot serialize request";
	}
	else error = "Device not connected, can not send request";

	if (this->_debug && error != "")
		Serial.println("[warn] " + error);

	delete request;
	request = nullptr;

	this->_dtoOutput->Reset();
}

void ProtonixDevice::RequestStreamAuthorize () {
	String id = this->_device->DeviceID();
	String passphrase = this->_device->DevicePassphrase();

	if (this->_debug)
		Serial.println("[info] Authorization request: '" + id + "':'" + passphrase + "'");

	this->RequestStream("/api/authorize/mechanism", new DTO::DTORequestAuthorization(id, passphrase));
}

void ProtonixDevice::OnStream (unsigned char* data) {
	String raw = String((char*)data);

	if (this->_debug)
		Serial.println("[OnStream] " + raw);

	raw.replace("}{", "}}-{{");
	//Serial.println("[debug] 1: " + raw);

	List<String>* cmds = explode("}-{", raw);
	//Serial.println("[debug] 2: " + String(cmds->Count()));

	for (String& cmd : *cmds) {
		//Serial.println("[debug] 3: " + cmd);

		this->_dtoInput->Debug(this->_debug);
		this->_dtoInput->BufferRaw(cmd);
		this->_dtoInput->Deserialize();

		if (this->_dtoInput->IsURL())
			this->_onStreamURL();

		if (this->_dtoInput->IsResponse())
			this->_onStreamResponse();

		if (this->_dtoInput->IsEvent())
			this->_onStreamEvent();

		this->_dtoInput->Reset();
	}

	delete cmds;
	cmds = nullptr;
}

void ProtonixDevice::_onStreamURL () {
	if (this->_debug)
		Serial.println("[url] " + this->_dtoInput->URL());
}

void ProtonixDevice::_onStreamResponse () {
	if (this->_debug)
		Serial.println("[response] " + this->_dtoInput->Response());

	this->_device->DeviceOnStreamResponse(this, this->_dtoInput);

	if (this->_dtoInput->Response() == "/api/authorize/mechanism") {
		DTO::DTOResponseAuthorization* dto = new DTO::DTOResponseAuthorization();
		dto->DTOPopulate(this->_dtoInput);

		int status = dto->Status();

		Serial.println("[info] Authorization response: " + String(status));

		this->_authorized = status == 200;
		this->_timerNetworkAuthorize->Enabled(!this->_authorized);

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

void ProtonixDevice::_onStreamEvent () {
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

ProtonixDTO* ProtonixDevice::DTOInput () {
	return this->_dtoInput;
}

ProtonixDTO* ProtonixDevice::DTOOutput () {
	return this->_dtoOutput;
}

bool ProtonixDevice::FirmwareUpdateOTA (String version) {
	String url = this->_serverBaseURI + "/api/mechanism/firmware/" + this->_device->DeviceID() + "?platform=";
	String ver = version == "" ? "" : String("&version=" + version);

	#if defined(ESP32)
		t_httpUpdate_return out = ESPhttpUpdate.update(url + "esp32" + ver);

		return out == HTTP_UPDATE_OK;
	#elif defined(ESP8266)
		WiFiClient client;
		t_httpUpdate_return out = ESPhttpUpdate.update(client, url + "esp8266" + ver);

		return out == HTTP_UPDATE_OK;
	#else
		return false;
	#endif
}
#endif

#if defined(ESP32)
void ProtonixDevice::_dedicateTask (TaskHandle_t* handle, unsigned short core, unsigned short priority) {
	xTaskCreatePinnedToCore(ProtonixDevice::_dedicatedTask, "DedicatedTask", 4096, this, priority, handle, core);
}

void ProtonixDevice::_dedicatedTask (void* param) {
	ProtonixDevice* device = (ProtonixDevice*)param;
	bool run = true;
	unsigned short core = xPortGetCoreID();

	while (run) {
		for (IProtonixHardware* hardware : device->_hardware)
			if (hardware->HardwareDedicatedCore() == core)
				hardware->HardwarePipe(device, core);

		device->_device->DeviceOnDedicatedTask(core);

		vTaskDelay(10 / portTICK_PERIOD_MS);
	}

	vTaskDelete(NULL);
}

void ProtonixDevice::DedicateTaskCore0 () {
	if (_dedicatedHandleCore0 == nullptr)
		this->_dedicateTask(&_dedicatedHandleCore0, 0);
}

void ProtonixDevice::DedicateTaskCore1 () {
	if (_dedicatedHandleCore1 == nullptr)
		this->_dedicateTask(&_dedicatedHandleCore1, 1);
}
#endif