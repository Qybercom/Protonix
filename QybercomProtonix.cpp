#include <Arduino.h>
#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>

#if defined(ESP32)
#include <WiFi.h>
#include <esp_wifi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#error "This ain't a ESP32 or ESP8266!"
#endif

#include "QybercomProtonix.h"

using namespace Qybercom::Protonix;


ProtonixURI::ProtonixURI (String host, uint port) {
	this->Host(host);
	this->Port(port);
}

ProtonixURI::ProtonixURI (String host, uint port, String path) {
	this->Host(host);
	this->Port(port);
	this->Path(path);
}

void ProtonixURI::Scheme (String scheme) {
	this->_scheme = scheme;
}

String ProtonixURI::Scheme () {
	return this->_scheme;
}

void ProtonixURI::Username (String username) {
	this->_username = username;
}

String ProtonixURI::Username () {
	return this->_username;
}

void ProtonixURI::Password (String password) {
	this->_password = password;
}

String ProtonixURI::Password () {
	return this->_password;
}

void ProtonixURI::Host (String host) {
	this->_host = host;
}

String ProtonixURI::Host () {
	return this->_host;
}

void ProtonixURI::Port (uint port) {
	this->_port = port;
}

uint ProtonixURI::Port () {
	return this->_port;
}

void ProtonixURI::Path (String path) {
	this->_path = path;
}

String ProtonixURI::Path () {
	return this->_path;
}




ProtonixTimer::ProtonixTimer () {
	this->_previous = 0;

	this->Interval(0);
	this->Unit(ProtonixTimer::ProtonixTimerUnit::MILLISECONDS);
}

ProtonixTimer::ProtonixTimer (unsigned int interval) {
	this->_previous = 0;

	this->Interval(interval);
	this->Unit(ProtonixTimer::ProtonixTimerUnit::MILLISECONDS);
}

ProtonixTimer::ProtonixTimer (unsigned int interval, ProtonixTimer::ProtonixTimerUnit unit) {
	this->_previous = 0;

	this->Interval(interval);
	this->Unit(unit);
}

unsigned long ProtonixTimer::Previous () {
	return this->_previous;
}

void ProtonixTimer::Interval (int interval) {
	this->_interval = interval;
}

unsigned int ProtonixTimer::Interval () {
	return this->_interval;
}

void ProtonixTimer::Unit (ProtonixTimer::ProtonixTimerUnit unit) {
	this->_unit = unit;
}

ProtonixTimer::ProtonixTimerUnit ProtonixTimer::Unit () {
	return this->_unit;
}

bool ProtonixTimer::Pipe () {
	unsigned long current = 0;

	if (this->_unit == ProtonixTimer::ProtonixTimerUnit::MILLISECONDS)
		current = millis();

	if (this->_unit == ProtonixTimer::ProtonixTimerUnit::MICROSECONDS)
		current = micros();

	long diff = current - this->_previous;
	bool elapsed = diff < 0 || diff >= this->_interval;

	if (elapsed)
		this->_previous = current;

	return elapsed;
}



ProtonixDeviceSensor::ProtonixDeviceSensor () {
	this->ID("");
	this->Value("");
	this->Active(false);
	this->Failure(false);
}

ProtonixDeviceSensor::ProtonixDeviceSensor (String id) {
	this->ID(id);
	this->Value("");
	this->Active(false);
	this->Failure(false);
}

ProtonixDeviceSensor::ProtonixDeviceSensor (String id, String value) {
	this->ID(id);
	this->Value(value);
	this->Active(false);
	this->Failure(false);
}

ProtonixDeviceSensor::ProtonixDeviceSensor (String id, bool active) {
	this->ID(id);
	this->Value("");
	this->Active(active);
	this->Failure(false);
}

ProtonixDeviceSensor::ProtonixDeviceSensor (String id, bool active, bool failure) {
	this->ID(id);
	this->Value("");
	this->Active(active);
	this->Failure(failure);
}

ProtonixDeviceSensor::ProtonixDeviceSensor (String id, String value, bool active) {
	this->ID(id);
	this->Value(value);
	this->Active(active);
	this->Failure(false);
}

ProtonixDeviceSensor::ProtonixDeviceSensor (String id, String value, bool active, bool failure) {
	this->ID(id);
	this->Value(value);
	this->Active(active);
	this->Failure(failure);
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




ProtonixDeviceStatus::ProtonixDeviceStatus () {
	//this->_sensors = { };
	this->_sensorCount = 0;
}

void ProtonixDeviceStatus::Summary (String summary) {
	this->_summary = summary;
}

String ProtonixDeviceStatus::Summary () {
	return this->_summary;
}

ProtonixDeviceSensor** ProtonixDeviceStatus::Sensors () {
	return this->_sensors;
}

ProtonixDeviceStatus* ProtonixDeviceStatus::SensorAdd (String id) {
	ProtonixDeviceSensor* sensor = new ProtonixDeviceSensor(id);

	this->_sensors[this->_sensorCount] = sensor;
	this->_sensorCount++;

	return this;
}

unsigned int ProtonixDeviceStatus::SensorCount () {
	//return end(this->_sensors) - begin(this->_sensors);
	//return sizeof(&this->_sensors) / sizeof(&this->_sensors[0]);
	return this->_sensorCount;
}





ProtonixDTO::ProtonixDTO () {
	this->_debug = false;
	this->_bufferOutput = "";
}

void ProtonixDTO::URL (String url) {
	this->_url = url;
}
String ProtonixDTO::URL () {
	return this->_url;
}

void ProtonixDTO::Response (String url) {
	this->_response = url;
}
String ProtonixDTO::Response () {
	return this->_response;
}

void ProtonixDTO::Event (String url) {
	this->_event = url;
}
String ProtonixDTO::Event () {
	return this->_event;
}

void ProtonixDTO::DTO(IProtonixDTO* dto) {
	this->_dto = dto;
}

IProtonixDTO* ProtonixDTO::DTO() {
	return this->_dto;
}

void ProtonixDTO::Data(JsonObject data) {
	this->_data = data;
}

JsonObject ProtonixDTO::Data () {
	return this->_data;
}

bool ProtonixDTO::IsURL () {
	return this->_url.length() != 0;
}

bool ProtonixDTO::IsResponse () {
	return this->_response.length() != 0;
}

bool ProtonixDTO::IsEvent () {
	return this->_event.length() != 0;
}

String ProtonixDTO::Serialize () {
	if (this->IsURL())
		this->_buffer["url"] = this->_url;

	if (this->IsResponse())
		this->_buffer["response"] = this->_response;

	if (this->IsEvent())
		this->_buffer["event"] = this->_event;

	this->_dto->DTOToJSON(this->_buffer);

	serializeJson(this->_buffer, this->_bufferOutput);

	/*this->_bufferOutput = "{";

	if (this->IsURL())
		this->_bufferOutput += "\"url\":\"" + this->_url + "\"";

	if (this->IsResponse())
		this->_bufferOutput += "\"response\":\"" + this->_response + "\"";

	if (this->IsEvent())
		this->_bufferOutput += "\"event\":\"" + this->_event + "\"";

	this->_bufferOutput += ",\"data\":" + this->_dto->DTOSerialize() + "}";*/

	return this->_bufferOutput;
}

bool ProtonixDTO::Deserialize (String raw) {
	DeserializationError err = deserializeJson(this->_buffer, raw);
	
	if (err)
		Serial.println("[dto] json deserialize error: " + String(err.f_str()));

	this->_bufferObj = this->_buffer.as<JsonObject>();

	if (this->_bufferObj.containsKey("url")) {
		const char* u = this->_bufferObj["url"];
		this->_url = (String)u;
	}

	if (this->_bufferObj.containsKey("response")) {
		const char* r = this->_bufferObj["response"];
		this->_response = (String)r;
	}

	if (this->_bufferObj.containsKey("event")) {
		const char* e = this->_bufferObj["event"];
		this->_event = (String)e;
	}

	if (this->_bufferObj.containsKey("data"))
		this->_data = this->_bufferObj["data"];

	return true;
}

void ProtonixDTO::Reset () {
	this->_url = "";
	this->_response = "";
	this->_event = "";
	//this->_data = null;

	this->_buffer.clear();
	this->_bufferOutput = "";
}

void ProtonixDTO::Debug (bool debug) {
	this->_debug = debug;
}

bool ProtonixDTO::Debug () {
	return this->_debug;
}




DTO::DTORequestAuthorization::DTORequestAuthorization () { }

DTO::DTORequestAuthorization::DTORequestAuthorization (String id, String passphrase) {
	this->ID(id);
	this->Passphrase(passphrase);
}

void DTO::DTORequestAuthorization::ID (String id) {
	this->_id = id;
}

String DTO::DTORequestAuthorization::ID () {
	return this->_id;
}

void DTO::DTORequestAuthorization::Passphrase (String passphrase) {
	this->_passphrase = passphrase;
}

String DTO::DTORequestAuthorization::Passphrase() {
	return this->_passphrase;
}

void DTO::DTORequestAuthorization::DTOToJSON (JsonDocument& dto) {
	dto["data"]["id"] = this->_id;
	dto["data"]["passphrase"] = this->_passphrase;
}

void DTO::DTORequestAuthorization::DTOPopulate(ProtonixDTO* dto) {
	JsonObject data = dto->Data();

	if (data.containsKey("id"))
		this->ID(data["id"]);

	if (data.containsKey("passphrase"))
		this->Passphrase(data["passphrase"]);
}

String DTO::DTORequestAuthorization::DTOSerialize () {
	return "{\"id\":\"" + this->_id + "\",\"passphrase\":\"" + this->_passphrase +"\"}";
}



DTO::DTORequestDeviceStatus::DTORequestDeviceStatus () {
	this->Status(new ProtonixDeviceStatus());
}

DTO::DTORequestDeviceStatus::DTORequestDeviceStatus (ProtonixDeviceStatus* status) {
	this->Status(status);
}

void DTO::DTORequestDeviceStatus::Status (ProtonixDeviceStatus* status) {
	this->_status = status;
}

ProtonixDeviceStatus* DTO::DTORequestDeviceStatus::Status () {
	return this->_status;
}

void DTO::DTORequestDeviceStatus::DTOToJSON (JsonDocument& dto) {
	dto["data"]["summary"] = this->_status->Summary();

	ProtonixDeviceSensor** sensors = this->_status->Sensors();
	JsonArray sensors_out = dto["data"].createNestedArray("sensors");

	unsigned int i = 0;
	unsigned int size = this->_status->SensorCount();

	while (i < size) {
		JsonObject sensor = sensors_out.createNestedObject();

		sensor["id"] = sensors[i]->ID();
		sensor["value"] = sensors[i]->Value();
		sensor["active"] = sensors[i]->Active();
		sensor["failure"] = sensors[i]->Failure();

		i++;
	}
}

void DTO::DTORequestDeviceStatus::DTOPopulate (ProtonixDTO* dto) {
}

String DTO::DTORequestDeviceStatus::DTOSerialize () {
	return "{\"summary\":\"" + this->_status->Summary() + "\"}";
}



void DTO::DTOResponseAuthorization::Status (unsigned short status) {
	this->_status = status;
}

unsigned short DTO::DTOResponseAuthorization::Status () {
	return this->_status;
}

void DTO::DTOResponseAuthorization::DTOPopulate (ProtonixDTO* dto) {
	JsonObject data = dto->Data();

	if (data.containsKey("status"))
		this->Status(data["status"]);
}

void DTO::DTOResponseAuthorization::DTOToJSON (JsonDocument& dto) {
	dto["data"]["status"] = this->_status;
}

unsigned short DTO::DTOResponseAuthorization::DTOResponseStatus () {
	return this->_status;
}

String DTO::DTOResponseAuthorization::DTOSerialize () {
	return "{\"status\":" + String(this->_status) + "}";
}



void DTO::DTOResponseMechanismStatus::Status (unsigned short status) {
	this->_status = status;
}

unsigned short DTO::DTOResponseMechanismStatus::Status () {
	return this->_status;
}

void DTO::DTOResponseMechanismStatus::DTOPopulate (ProtonixDTO* dto) {
	JsonObject data = dto->Data();

	if (data.containsKey("status"))
		this->Status(data["status"]);
}

void DTO::DTOResponseMechanismStatus::DTOToJSON (JsonDocument& dto) {
	dto["data"]["status"] = this->_status;
}

unsigned short DTO::DTOResponseMechanismStatus::DTOResponseStatus () {
	return this->_status;
}

String DTO::DTOResponseMechanismStatus::DTOSerialize () {
	return "{\"status\":" + String(this->_status) + "}";
}



void DTO::DTOEventCommand::Name (String name) {
	this->_name = name;
}

String DTO::DTOEventCommand::Name () {
	return this->_name;
}

void DTO::DTOEventCommand::DTOPopulate (ProtonixDTO* dto) {
	JsonObject data = dto->Data();

	if (data.containsKey("command"))
		this->Name(data["command"]);
}

void DTO::DTOEventCommand::DTOToJSON(JsonDocument &dto) {
	dto["data"]["command"] = this->_name;
}

String DTO::DTOEventCommand::DTOSerialize() {
	return "{\"command\":\"" + this->_name + "\"}";
}





Networks::NWiFi::NWiFi (String ssid, String password, String mac, String hostname) {
	this->_ssid = ssid;
	this->_password = password;
	this->_mac = mac;
	this->_hostname = hostname;
}

bool Networks::NWiFi::Connect () {
	uint8_t mac[6] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
	INetwork::ParseMAC(this->_mac, mac);

	// https://randomnerdtutorials.com/esp32-set-custom-hostname-arduino/#comment-741757
	#if defined(ESP32)
	WiFi.setHostname(this->_hostname.c_str());
	esp_wifi_set_mac(WIFI_IF_STA, &mac[0]);
	#elif defined(ESP8266)
	WiFi.hostname(this->_hostname.c_str());
	wifi_set_macaddr(STATION_IF, &mac[0]);
	#else
	#error "This ain't a ESP32 or ESP8266!"
	#endif

	WiFi.disconnect();
	WiFi.mode(WIFI_STA);
	WiFi.begin(this->_ssid, this->_password);

	delay(1000);
	/*
	WiFi.reconnect();
	*/

	return true;
}

bool Networks::NWiFi::Connected () {
	return WiFi.status() == WL_CONNECTED;
}

bool Networks::NWiFi::Disconnect () {
	WiFi.disconnect();

	return true;
}

String Networks::NWiFi::AddressMAC () {
	return this->_mac;
}

String Networks::NWiFi::AddressIP () {
	#if defined(ESP32)
	return String(WiFi.localIP());
	#elif defined(ESP8266)
	return WiFi.localIP().toString();
	#else
	#error "This ain't a ESP32 or ESP8266!"
	#endif
}






void Protocols::PWiFiTCP::Init (ProtonixDevice* device) {
	this->_device = device;
}

bool Protocols::PWiFiTCP::Connect (ProtonixURI*uri) {
	this->_client.connect(uri->Host().c_str(), uri->Port());

	return true;
}

bool Protocols::PWiFiTCP::Connected () {
	return this->_client.connected();
}

void Protocols::PWiFiTCP::Pipe () {
	int available = this->_client.available();

	if (available) {
		this->_bufferPTR = this->_buffer;
		this->_client.read(this->_bufferPTR, available);

		this->_device->OnStream(this->_buffer);
	}
}

void Protocols::PWiFiTCP::Send (String data) {
	uint8_t* buffer = (uint8_t*)data.c_str();

	this->_client.write(buffer, data.length());
}



void Protocols::PWebSocket::Init (ProtonixDevice* device) {
	this->_device = device;
	this->_client.onMessage([&](websockets::WebsocketsMessage message) {
		this->_device->OnStream((unsigned char*)message.data().c_str());
	});
}

bool Protocols::PWebSocket::Connect (ProtonixURI* uri) {
	return this->_client.connect(
		uri->Host(),
		uri->Port(),
		uri->Path()
	);
}

bool Protocols::PWebSocket::Connected () {
	return this->_client.available();
}

void Protocols::PWebSocket::Pipe () {
	this->_client.poll();
}

void Protocols::PWebSocket::Send (String raw) {
	this->_client.send(raw);
}




bool ProtonixDeviceBase::DeviceAutoStatus() {
	return true;
}

void ProtonixDeviceBase::DeviceOnNetworkConnect (ProtonixDevice* device) {
	if (this->_debug)
		Serial.println("[device] NetworkConnect");
}

void ProtonixDeviceBase::DeviceOnProtocolConnect (ProtonixDevice* device) {
	if (this->_debug)
		Serial.println("[device] ProtocolConnect");
}

void ProtonixDeviceBase::DeviceOnStreamResponse (ProtonixDevice* device, ProtonixDTO* dto) {
	if (this->_debug)
		Serial.println("[device] StreamResponse " + dto->Response());
}

void ProtonixDeviceBase::DeviceOnStreamEvent (ProtonixDevice* device, ProtonixDTO* dto) {
	if (this->_debug)
		Serial.println("[device] StreamEvent " + dto->Event());
}

void ProtonixDeviceBase::DeviceOnAuthorization (ProtonixDevice* device, DTO::DTOResponseAuthorization* authorization) {
	if (this->_debug)
		Serial.println("[device:authorize] " + String(authorization->Status() == 200 ? "Success" : "Failure"));
}

ProtonixDeviceStatus* ProtonixDeviceBase::DeviceStatus() {
	return this->_status;
}






ProtonixDevice::ProtonixDevice (IProtonixDevice* device) {
	this->_ready = false;
	this->_networkConnected1 = false;
	this->_networkConnected2 = false;
	this->_protocolConnected1 = false;
	this->_protocolConnected2 = false;

	this->Debug(false);
	this->Device(device);
	this->_timer = new ProtonixTimer(this->_device->DeviceTick());

	this->_dtoInput = new ProtonixDTO();
	this->_dtoOutput = new ProtonixDTO();
	this->_dtoInputResponseAuthorization = new DTO::DTOResponseAuthorization();
	this->_dtoInputResponseMechanismStatus = new DTO::DTOResponseMechanismStatus();
	this->_dtoInputEventCommand = new DTO::DTOEventCommand();
}

void ProtonixDevice::Device (IProtonixDevice* device) {
	this->_device = device;
}

IProtonixDevice* ProtonixDevice::Device () {
	return this->_device;
}

ProtonixTimer* ProtonixDevice::Timer () {
	return this->_timer;
}

void ProtonixDevice::Network (INetwork* network) {
	this->_network = network;
}

INetwork* ProtonixDevice::Network () {
	return this->_network;
}

void ProtonixDevice::Protocol (IProtocol* protocol) {
	this->_protocol = protocol;
}

IProtocol* ProtonixDevice::Protocol () {
	return this->_protocol;
}

void ProtonixDevice::Server (ProtonixURI* uri) {
	this->_uri = uri;
}

ProtonixURI* ProtonixDevice::Server () {
	return this->_uri;
}

void ProtonixDevice::ServerEndpoint (String host, uint port) {
	this->Server(new ProtonixURI(host, port));
}

void ProtonixDevice::ServerEndpoint (String host, uint port, String path) {
	this->Server(new ProtonixURI(host, port, path));
}

bool ProtonixDevice::Connected () {
	return this->_network->Connected() && this->_protocol->Connected();
}

void ProtonixDevice::Debug (bool debug) {
	this->_debug = debug;
}

bool ProtonixDevice::Debug () {
	return this->_debug;
}

void ProtonixDevice::_pipe () {
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
		Serial.println("[WARNING] RECONNECT N:" + String(this->_network->Connected()) + " P:" + String(this->_protocol->Connected()));

		this->_networkConnected1 = false;
		this->_networkConnected2 = false;

		this->_protocolConnected1 = false;
		this->_protocolConnected2 = false;
	}

	this->_protocol->Pipe();
	this->_device->DeviceOnTick(this);

	if (this->_device->DeviceAutoStatus())
		this->RequestStream("/api/mechanism/status", new DTO::DTORequestDeviceStatus(this->_device->DeviceStatus()));
}

void ProtonixDevice::Pipe () {
	if (!this->_ready) {
		this->_ready = true;
		this->_device->DeviceOnReady(this);
	}

	if (this->_timer->Pipe())
		this->_pipe();
}

void ProtonixDevice::RequestStream (String url, IProtonixDTORequest* request) {
	if (!this->Connected()) {
		if (this->_debug)
			Serial.println("[WARNING] Device not connected, can not send request");

		return;
	}

	this->_dtoOutput->Debug(this->Debug());
	this->_dtoOutput->URL(url);
	this->_dtoOutput->DTO(request);

	String raw = this->_dtoOutput->Serialize();
	this->_dtoOutput->Reset();
	this->_protocol->Send(raw);

	if (this->_debug)
		Serial.println("[request] " + url + " ok: " + raw);
}

void ProtonixDevice::RequestStreamAuthorize () {
	this->RequestStream("/api/authorize/mechanism", new DTO::DTORequestAuthorization(
		this->_device->DeviceID(),
		this->_device->DevicePassphrase()
	));
}

void ProtonixDevice::OnStream (unsigned char* data) {
	if (this->_debug)
		Serial.println("[OnStream] " + String((char*)data));

	//this->_dtoInput->Debug(this->_debug);
	this->_dtoInput->Deserialize(String((char*)data));

	if (this->_dtoInput->IsURL())
		this->_onStreamURL();

	if (this->_dtoInput->IsResponse())
		this->_onStreamResponse();

	if (this->_dtoInput->IsEvent())
		this->_onStreamEvent();

	this->_dtoInput->Reset();
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
		this->_dtoInputResponseAuthorization->DTOPopulate(this->_dtoInput);

		this->_device->DeviceOnAuthorization(this, this->_dtoInputResponseAuthorization);
	}

	if (this->_dtoInput->Response() == "/api/mechanism/status") {
		this->_dtoInputResponseMechanismStatus->DTOPopulate(this->_dtoInput);

		if (this->_dtoInputResponseMechanismStatus->Status() != 200)
			this->RequestStreamAuthorize();
	}
}

void ProtonixDevice::_onStreamEvent () {
	if (this->_debug)
		Serial.println("[event] " + this->_dtoInput->Event());

	this->_device->DeviceOnStreamEvent(this, this->_dtoInput);

	if (this->_dtoInput->Event() == "/api/mechanism/command/" + this->_device->DeviceID()) {
		this->_dtoInputEventCommand->DTOPopulate(this->_dtoInput);

		this->_device->DeviceOnCommand(this, this->_dtoInputEventCommand);
	}
}

ProtonixDTO* ProtonixDevice::DTOInput () {
	return this->_dtoInput;
}

ProtonixDTO* ProtonixDevice::DTOOutput() {
	return this->_dtoOutput;
}

DTO::DTOResponseAuthorization* ProtonixDevice::DTOInputResponseAuthorization () {
	return this->_dtoInputResponseAuthorization;
}

DTO::DTOResponseMechanismStatus* ProtonixDevice::DTOInputResponseMechanismStatus() {
	return this->_dtoInputResponseMechanismStatus;
}

DTO::DTOEventCommand* ProtonixDevice::DTOInputEventCommand () {
	return this->_dtoInputEventCommand;
}