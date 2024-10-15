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

#include "IProtonixDevice.h"
#include "ProtonixDevice.h"
#include "ProtonixDeviceStatus.h"
#include "ProtonixTimer.h"
#include "ProtonixAction.h"
#include "ProtonixMemory.h"

#include "Hardware/HReaderMFRC.h"

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

using namespace Qybercom::Protonix;



ProtonixDevice::ProtonixDevice(IProtonixDevice* device) {
	this->_timer = new ProtonixTimer();
	this->_status = new ProtonixDeviceStatus();
	this->_ready = false;
	this->_portCount = 0;
	this->_actionCursorList = 0;
	this->_actionCursorBacklog = 0;
	this->_actionCursorCurrent = 0;
	this->Device(device);
	this->Debug(false);

    this->_memory = new ProtonixMemory();
    this->_memory->EEPROMBegin();
    this->_registry = new ProtonixRegistry(this->_memory);
    this->_registry->Debug(this->_debug); // TODO: runtime switch for every debuggable component

	#if defined(ESP32) || defined(ESP8266)
	this->_networkConnected1 = false;
	this->_networkConnected2 = false;
	this->_protocolConnected1 = false;
	this->_protocolConnected2 = false;

	this->_dtoInput = new ProtonixDTO();
	this->_dtoOutput = new ProtonixDTO();
	#endif

	#if defined(AVR)
	pinMode(4, OUTPUT);
	digitalWrite(4, HIGH);
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

ProtonixMemory* ProtonixDevice::Memory() {
	return this->_memory;
}

ProtonixRegistry* ProtonixDevice::Registry() {
	return this->_registry;
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

int ProtonixDevice::FreeFlash() {
	#if defined(ESP32) || defined(ESP8266)
		return ESP.getFreeSketchSpace();
	#elif defined(AVR)
		return 0;
	#else
		return 0;
	#endif
}

void ProtonixDevice::Reboot() {
	#if defined(ESP32) || defined(ESP8266)
		ESP.restart();
	#elif defined(AVR)
		digitalWrite(4, LOW);
	#else
	#endif
}

IProtonixHardware* ProtonixDevice::Hardware(String id) {
	unsigned int i = 0;

	while (i < this->_hardwareCount) {
		if (id == this->_hardware[i]->HardwareID())
			return this->_hardware[i];

		i++;
	}

	return nullptr;
}

ProtonixDevice* ProtonixDevice::Hardware(String id, IProtonixHardware* hardware) {
	hardware->HardwareID(id);

    this->_hardware[this->_hardwareCount] = hardware;
	this->_hardwareCount++;

	return this;
}

ProtonixDevicePort* ProtonixDevice::Port(String name) {
	unsigned int i = 0;

	while (i < this->_portCount) {
		if (name == this->_ports[i]->Name())
			return this->_ports[i];
		
		i++;
	}
	
	return nullptr;
}

ProtonixDevicePort* ProtonixDevice::Port(ProtonixDevicePort* port) {
	this->_ports[this->_portCount] = port;
	this->_portCount++;

	return port;
}

ProtonixDevicePort* ProtonixDevice::Port(String name, unsigned int pinTX, unsigned int pinRX) {
	return this->Port(new ProtonixDevicePort(name, pinTX, pinRX));
}

ProtonixDevicePort* ProtonixDevice::Port(String name, unsigned int pinTX, unsigned int pinRX, bool blocking) {
	return this->Port(new ProtonixDevicePort(name, pinTX, pinRX, blocking));
}

ProtonixDevicePort* ProtonixDevice::Port(String name, unsigned int pinTX, unsigned int pinRX, bool blocking, bool observable) {
	return this->Port(new ProtonixDevicePort(name, pinTX, pinRX, blocking, observable));
}

ProtonixDevicePort* ProtonixDevice::Port(String name, unsigned int pinTX, unsigned int pinRX, unsigned int speed) {
	return this->Port(new ProtonixDevicePort(name, pinTX, pinRX, speed));
}

ProtonixDevicePort* ProtonixDevice::Port(String name, unsigned int pinTX, unsigned int pinRX, unsigned int speed, bool blocking) {
	return this->Port(new ProtonixDevicePort(name, pinTX, pinRX, speed, blocking));
}

ProtonixDevicePort* ProtonixDevice::Port(String name, unsigned int pinTX, unsigned int pinRX, unsigned int speed, bool blocking, bool observable) {
	return this->Port(new ProtonixDevicePort(name, pinTX, pinRX, speed, blocking, observable));
}

ProtonixDevicePort* ProtonixDevice::Port(String name, unsigned int pinTX, unsigned int pinRX, unsigned int speed, unsigned int timeout) {
	return this->Port(new ProtonixDevicePort(name, pinTX, pinRX, speed, timeout));
}

ProtonixDevicePort* ProtonixDevice::Port(String name, unsigned int pinTX, unsigned int pinRX, unsigned int speed, unsigned int timeout, bool blocking) {
	return this->Port(new ProtonixDevicePort(name, pinTX, pinRX, speed, timeout, blocking));
}

ProtonixDevicePort* ProtonixDevice::Port(String name, unsigned int pinTX, unsigned int pinRX, unsigned int speed, unsigned int timeout, bool blocking, bool observable) {
	return this->Port(new ProtonixDevicePort(name, pinTX, pinRX, speed, timeout, blocking, observable));
}

ProtonixDevicePort* ProtonixDevice::PortDefault() {
	return this->Port(new ProtonixDevicePort());
}

ProtonixDevicePort* ProtonixDevice::PortDefault(bool blocking) {
	return this->Port(new ProtonixDevicePort(blocking));
}

ProtonixDevicePort* ProtonixDevice::PortDefault(bool blocking, bool observable) {
	return this->Port(new ProtonixDevicePort(blocking, observable));
}

ProtonixDevicePort* ProtonixDevice::PortDefault(unsigned int speed) {
	return this->Port(new ProtonixDevicePort(speed));
}

ProtonixDevicePort* ProtonixDevice::PortDefault(unsigned int speed, bool blocking) {
	return this->Port(new ProtonixDevicePort(speed, blocking));
}

ProtonixDevicePort* ProtonixDevice::PortDefault(unsigned int speed, bool blocking, bool observable) {
	return this->Port(new ProtonixDevicePort(speed, blocking, observable));
}

ProtonixDevicePort* ProtonixDevice::PortDefault(unsigned int speed, unsigned int timeout) {
	return this->Port(new ProtonixDevicePort(speed, timeout));
}

ProtonixDevicePort* ProtonixDevice::PortDefault(unsigned int speed, unsigned int timeout, bool blocking) {
	return this->Port(new ProtonixDevicePort(speed, timeout, blocking));
}

ProtonixDevicePort* ProtonixDevice::PortDefault(unsigned int speed, unsigned int timeout, bool blocking, bool observable) {
	return this->Port(new ProtonixDevicePort(speed, timeout, blocking, observable));
}

ProtonixAction* ProtonixDevice::Action(String name) {
	int i = 0;

	while (i < this->_actionCursorList) {
		if (this->_actionList[i]->Name() == name)
			return this->_actionList[i];

		i++;
	}

	return nullptr;
}

bool ProtonixDevice::ActionRegister(ProtonixAction* action) {
	if (this->_actionCursorList == PROTONIX_LIMIT_ACTION_LIST) return false;

	this->_actionList[this->_actionCursorList] = action;
	this->_actionCursorList++;

	return true;
}

bool ProtonixDevice::ActionRegister(String name) {
	return this->ActionRegister(new ProtonixAction(name));
}

bool ProtonixDevice::ActionRegister(String name, unsigned int interval) {
	return this->ActionRegister(new ProtonixAction(name, interval));
}

bool ProtonixDevice::ActionRegister(String name, unsigned int interval, int stepEnd) {
	return this->ActionRegister(new ProtonixAction(name, interval, stepEnd));
}

bool ProtonixDevice::ActionRegister(String name, unsigned int interval, int stepBegin, int stepEnd) {
	return this->ActionRegister(new ProtonixAction(name, interval, stepBegin, stepEnd));
}

bool ProtonixDevice::ActionRegister(String name, unsigned int interval, int stepBegin, int stepEnd, int step) {
	return this->ActionRegister(new ProtonixAction(name, interval, stepBegin, stepEnd, step));
}

bool ProtonixDevice::ActionTrigger(String name) {
	if (this->_actionCursorBacklog == PROTONIX_LIMIT_ACTION_BACKLOG) return false;

	this->_actionBacklog[this->_actionCursorBacklog] = name;
	this->_actionCursorBacklog++;

	return true;
}

void ProtonixDevice::ActionReset() {
	int i = 0;

	i = 0;
	while (i < PROTONIX_LIMIT_ACTION_BACKLOG) { //this->_actionCursorBacklog) {
		this->_actionBacklog[i] = "";

		i++;
	}

	i = 0;
	while (i < this->_actionCursorList) {
		this->_actionList[i]->Reset();

		i++;
	}

	this->_actionCursorBacklog = 0;
	this->_actionCursorCurrent = 0;
}

#if defined(ESP32) || defined(ESP8266)
void ProtonixDevice::_pipeNetwork() {
	if (this->_network == nullptr || this->_protocol == nullptr) return;
	
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
		}

		if (!this->_protocol->Connected()) return;

		this->_protocolConnected1 = true;

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
}
#endif

void ProtonixDevice::_pipeActions() {
	if (this->_actionCursorCurrent == PROTONIX_LIMIT_ACTION_BACKLOG)
		this->_actionCursorCurrent = 0;

	if (this->_actionBacklog[this->_actionCursorCurrent] == "") {
		this->_actionCursorCurrent++;

		return;
	}

	int i = 0;
	//bool finished = true;

	while (i < this->_actionCursorList) {
		if (this->_actionList[i]->Name() == this->_actionBacklog[this->_actionCursorCurrent]) {
			if (this->_actionList[i]->PipeStart()) {
				if (this->_actionList[i]->Pipe()) {
					this->_device->DeviceOnAction(this, this->_actionList[i]);

					this->_actionList[i]->PipeEnd();
				}
			}

			if (this->_actionList[i]->Completed()) {
				this->_actionBacklog[this->_actionCursorCurrent] = "";
				this->_actionCursorCurrent++;

				this->_actionList[i]->Reset();
			}

			//finished = false;
		}

		i++;
	}
	
	/*
	if (finished) {
		this->_actionCursorBacklog = 0;
		this->_actionCursorCurrent = 0;
	}
	*/
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

        i = 0;
        while (i < this->_hardwareCount) {
            this->_hardware[i]->HardwareInitPre();

            i++;
        }

        SPI.begin();

        i = 0;
        while (i < this->_hardwareCount) {
            this->_hardware[i]->HardwareInitPost();

            i++;
        }

		this->_device->DeviceOnReady(this);
	}
	
	#if defined(ESP32) || defined(ESP8266)
	this->_pipeNetwork();
	#endif

	i = 0;

	while (i < this->_hardwareCount) {
		this->_hardware[i]->HardwarePipe();

		i++;
	}

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

	this->_pipeActions();
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

bool ProtonixDevice::SerialCommandSensor(String port, String id, String value, bool active, bool failure, String state) {
	ProtonixDeviceSensor* sensor = new ProtonixDeviceSensor(id, value, active, failure, state);
	
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

bool ProtonixDevice::SerialCommandSensor(String port, String id, bool active, bool failure, String state) {
	ProtonixDeviceSensor* sensor = new ProtonixDeviceSensor(id, active, failure, state);
	
	bool ok = this->SerialCommandSensor(port, sensor);
	
	delete sensor;
	sensor = nullptr;

	return ok;
}

bool ProtonixDevice::SerialCommandSensor(String port, String id, String value, String state) {
	ProtonixDeviceSensor* sensor = new ProtonixDeviceSensor(id, value, state);
	
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

void ProtonixDevice::ServerBaseURI(String uri) {
	this->_serverBaseURI = uri;
}

bool ProtonixDevice::Connected() {
	return true
		&& this->_network != nullptr
		&& this->_protocol != nullptr
		&& this->_network->Connected()
		&& this->_protocol->Connected();
}

void ProtonixDevice::RequestStream(String url, IProtonixDTORequest* request) {
	String error = "";
	
	if (this->Connected()) {
		this->_dtoOutput->Debug(this->_debug);
		this->_dtoOutput->URL(url);
		this->_dtoOutput->DTO(request);
		
		if (this->_dtoOutput->Serialize()) this->_protocol->Send(this->_dtoOutput->BufferRaw());
		else error = "Cannot serialize request";
	}
	else error = "Device not connected, can not send request";
	
	if (this->_debug && error != "")
		Serial.println("[WARNING] " + error);
	
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

/*bool _onUpdateOTABatch(String) {

}*/

bool ProtonixDevice::FirmwareUpdateOTA() {
	#if defined(ESP32)
    	t_httpUpdate_return out = ESPhttpUpdate.update(this->_serverBaseURI + "/api/mechanism/firmware/" + this->_device->DeviceID() + "?platform=esp32");

    	return out == HTTP_UPDATE_OK;
	#elif defined(ESP8266)
		WiFiClient client;
    	t_httpUpdate_return out = ESPhttpUpdate.update(client, this->_serverBaseURI + "/api/mechanism/firmware/" + this->_device->DeviceID() + "?platform=esp8266");

    	return out == HTTP_UPDATE_OK;
    #else
		return false;
    #endif
}

/*
bool ProtonixDevice::FirmwareUpdateOTA(void(*onProgress)(int, int)) {
	ProtonixHTTPClient* http = ProtonixHTTPClient::OverWiFi();
    http->Debug(this->_debug);

    ProtonixHTTPFrame* request = new ProtonixHTTPFrame("GET", this->_serverBaseURI + "/api/mechanism/firmware/" + this->_device->DeviceID());
    http->Request(request);

    bool ok = http->Send();

    if (!ok) Serial.println("[WARNING] FirmwareUpdateOTA failed: can not connect to HTTP server for meta");
    else {
        Serial.println("[ota:http] sent");

        ok = http->ReceiveHeaders();

        if (!ok) Serial.println("[WARNING] FirmwareUpdateOTA failed: can not receive HTTP headers");
        else {
            Serial.println("[ota:http] receiveHeaders");

            ok = this->_memory->FlashFirmwareBegin(http->Response()->LengthExpected());

            if (!ok) Serial.println("[WARNING] FirmwareUpdateOTA failed: can not begin firmware flashing sequence");
            else {
                Serial.println("[ota:flash] begin");

                int i = 1;
                unsigned int bufferSize = this->_memory->FlashBufferSize();

                while (http->ReceiveAvailable()) {
                    String batch = http->ReceiveBody(bufferSize);

                    //Serial.println("[ota:http] receiveBody " + String(i));// + ": " + batch);

                    ok = this->_memory->FlashWrite(batch);

                    if (!ok) Serial.println("[WARNING] FirmwareUpdateOTA failed: flash write failed at step " + String(i));
                    else {
                        //Serial.println("[ota:flash] write " + String(i));// + ": " + batch);
                    }

                    i++;
                }

                ok = this->_memory->FlashEnd();

                if (!ok) Serial.println("[WARNING] FirmwareUpdateOTA failed: can not end flashing sequence");
                else {
                    Serial.println("[ota:flash] end");
                }
            }
        }
    }

    return ok;
}
*/

/*
bool ProtonixDevice::FirmwareUpdateOTA(void(*onProgress)(int, int)) {
	ProtonixHTTPClient* http = ProtonixHTTPClient::OverWiFi();
    http->Debug(this->_debug);
    //http->TimeoutResponse(5000);

	ProtonixHTTPFrame* request = new ProtonixHTTPFrame("GET", this->_serverBaseURI + "/api/mechanism/firmware/" + this->_device->DeviceID());
    //ProtonixHTTPFrame* request = new ProtonixHTTPFrame("GET", "http://arduino.evolutex.ru/cdn/mechanism-15-esp8266.ino.bin");
    request->Debug(this->_debug);

    http->Request(request);

	bool ok = http->Send();

    if (!ok) Serial.println("[WARNING] FirmwareUpdateOTA failed: can not connect to HTTP server for meta");
    else {
		ok = http->Receive();

        if (!ok) Serial.println("[WARNING] FirmwareUpdateOTA failed: can not receive response from HTTP server for meta");
        else {
			//Serial.println("[debug] http recv " + String(ok) + ": `" + http->Response()->Version() + "` `" + http->Response()->Status() + "` `" + http->Response()->Body() + "`");
            if (http->Response()->Status() != "200 OK") Serial.println("[WARNING] FirmwareUpdateOTA failed: can not find suitable firmware image on the HTTP server");
            else {
            	/*String firmware = http->Response()->Body();

                ok = this->FirmwareUpdate(firmware, onProgress);*
                ok = this->FirmwareUpdate(*http, onProgress);
            }
        }
    }

    delete request;
    request = nullptr;

    delete http;
    http = nullptr;

    return ok;
}
*/

/*
bool ProtonixDevice::FirmwareUpdate(String firmware, void(*onProgress)(int, int)) {
}
*
 */

/*
bool ProtonixDevice::FirmwareUpdate(Stream& stream, void(*onProgress)(int, int)) {
	//StreamString stream = StreamString(firmware);
    StreamString error;

    //String md5 = "TESTMD5";
    unsigned int size = stream.available();
    int cmd = U_FLASH; // U_FS
    bool result = false;

    //Serial.println("[debug] firmware: " + String(firmware.length()));
    Serial.println("[debug] stream: " + String(size));
    Serial.println("[debug] flash: " + String(ProtonixDevice::FreeFlash()));

    if (onProgress)
    	Update.onProgress(onProgress);

    if (!Update.begin(size, cmd)) this->_updateError("begin", error);
    else {
      	Serial.println("[debug] FirmwareUpdate begin");

    	if (onProgress)
    		onProgress(0, size);

        // TODO: think of necessarity
    	//if (!Update.setMD5(md5.c_str()))
      		//this->_updateError("md5", error);

        if (Update.writeStream(stream) != size) this->_updateError("writeStream", error);
        else {
        	Serial.println("[debug] FirmwareUpdate writeStream");

    		if (onProgress)
    			onProgress(size, size);

            if (!Update.end()) this->_updateError("end", error);
            else {
        		Serial.println("[debug] FirmwareUpdate end");

                result = true;
        	}
        }
	}

    //delete stream;
    //stream = nullptr;

    return result;
}

void ProtonixDevice::_updateError(String step, StreamString &error) {
    Update.printError(error);
    Serial.println("[WARNING] FirmwareUpdate error (" + step + "): " + String(error.c_str()));
}
*/
#endif