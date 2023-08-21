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

void ProtonixDeviceSensor::Reset () {
	this->Value("");
	this->Active(false);
	this->Failure(false);
}




ProtonixDTO::ProtonixDTO () {
	this->_debug = false;
	this->_bufferRaw = "";
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

bool ProtonixDTO::Serialize () {
	if (this->IsURL())
		this->_buffer["url"] = this->_url;

	if (this->IsResponse())
		this->_buffer["response"] = this->_response;

	if (this->IsEvent())
		this->_buffer["event"] = this->_event;

	this->_dto->DTOSerialize(this->_buffer);

	return serializeJson(this->_buffer, this->_bufferRaw) != 0;
}

bool ProtonixDTO::Deserialize () {
	DeserializationError err = deserializeJson(this->_buffer, this->_bufferRaw);
	
	if (err) {
		Serial.println("[dto] json deserialize error: " + String(err.f_str()));

		return false;
	}

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

void ProtonixDTO::BufferRaw (String data) {
	this->_bufferRaw = data;
}

void ProtonixDTO::BufferRaw(char* data) {
	this->_bufferRaw = String(data);
}

String ProtonixDTO::BufferRaw() {
	return this->_bufferRaw;
}

void ProtonixDTO::Reset () {
	this->_url = "";
	this->_response = "";
	this->_event = "";
	//this->_data = null;

	this->_buffer.clear();
	this->_bufferRaw = "";
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

String DTO::DTORequestAuthorization::Passphrase () {
	return this->_passphrase;
}

void DTO::DTORequestAuthorization::DTOPopulate (ProtonixDTO* dto) {
	JsonObject data = dto->Data();

	if (data.containsKey("id"))
		this->ID(data["id"]);

	if (data.containsKey("passphrase"))
		this->Passphrase(data["passphrase"]);
}

void DTO::DTORequestAuthorization::DTOSerialize(JsonDocument& dto) {
	dto["data"]["id"] = this->_id;
	dto["data"]["passphrase"] = this->_passphrase;
}

DTO::DTORequestAuthorization* DTO::DTORequestAuthorization::Reset (String id, String passphrase) {
	this->ID(id);
	this->Passphrase(passphrase);

	return this;
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

void DTO::DTORequestDeviceStatus::DTOPopulate (ProtonixDTO* dto) {
}

void DTO::DTORequestDeviceStatus::DTOSerialize (JsonDocument& dto) {
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

DTO::DTORequestDeviceStatus* DTO::DTORequestDeviceStatus::Reset (ProtonixDeviceStatus* status) {
	this->Status(status);

	return this;
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

void DTO::DTOResponseAuthorization::DTOSerialize (JsonDocument& dto) {
	dto["data"]["status"] = this->_status;
}

unsigned short DTO::DTOResponseAuthorization::DTOResponseStatus () {
	return this->_status;
}



void DTO::DTOResponseDeviceStatus::Status (unsigned short status) {
	this->_status = status;
}

unsigned short DTO::DTOResponseDeviceStatus::Status () {
	return this->_status;
}

void DTO::DTOResponseDeviceStatus::DTOPopulate (ProtonixDTO* dto) {
	JsonObject data = dto->Data();

	if (data.containsKey("status"))
		this->Status(data["status"]);
}

void DTO::DTOResponseDeviceStatus::DTOSerialize(JsonDocument& dto) {
	dto["data"]["status"] = this->_status;
}

unsigned short DTO::DTOResponseDeviceStatus::DTOResponseStatus () {
	return this->_status;
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

void DTO::DTOEventCommand::DTOSerialize (JsonDocument &dto) {
	dto["data"]["command"] = this->_name;
}





Networks::NWiFi::NWiFi (String ssid, String password, String mac, String hostname) {
	this->_ssid = ssid;
	this->_password = password;
	this->_mac = mac;
	this->_hostname = hostname;
}

bool Networks::NWiFi::Connect () {
	//uint8_t mac[6] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
	INetwork::ParseMAC(this->_mac, this->_macBuffer);

	WiFi.disconnect(true);

	// https://randomnerdtutorials.com/esp32-set-custom-hostname-arduino/#comment-741757
	#if defined(ESP32)
	WiFi.setHostname(this->_hostname.c_str());
	WiFi.mode(WIFI_STA);
	esp_wifi_set_mac(WIFI_IF_STA, &this->_macBuffer[0]);
	#elif defined(ESP8266)
	WiFi.hostname(this->_hostname.c_str());
	WiFi.mode(WIFI_STA);
	wifi_set_macaddr(STATION_IF, &this->_macBuffer[0]);
	#else
	#error "This ain't a ESP32 or ESP8266!"
	#endif

	WiFi.begin(this->_ssid, this->_password);

	delay(1000);
	/*
	WiFi.reconnect();
	*/

	return true;
}

bool Networks::NWiFi::Connected () {
	if (WiFi.status() != WL_CONNECTED) {
		Serial.print("[WARNING] WiFi status: ");
		Serial.println(WiFi.status());
	}

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










ProtonixDeviceStatus::ProtonixDeviceStatus () {
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
	return this->_sensorCount;
}

void ProtonixDeviceStatus::SensorsReset () {
	unsigned int i = 0;

	while (i < this->_sensorCount) {
		this->_sensors[i]->Reset();

		i++;
	}
}

/*
String ProtonixDeviceStatus::SensorsSummary() {
}
*/

void ProtonixDeviceBase::_init () {
	this->_on = false;
	this->_cmd = "";
	this->_status = new ProtonixDeviceStatus();
	this->_debug = false;
}

void ProtonixDeviceBase::_init (bool debug) {
	this->_on = false;
	this->_cmd = "";
	this->_status = new ProtonixDeviceStatus();
	this->_debug = debug;
}

void ProtonixDeviceBase::_cmdStdOn () {
	#if defined(ESP32)
	digitalWrite(2, HIGH);
	#elif defined(ESP8266)
	digitalWrite(2, LOW);
	#else
	#endif
}

void ProtonixDeviceBase::_cmdStdOff () {
	#if defined(ESP32)
	digitalWrite(2, LOW);
	#elif defined(ESP8266)
	digitalWrite(2, HIGH);
	#else
	#endif
}

void ProtonixDeviceBase::_summary (String additional) {
	this->_summary(additional, false);
}

void ProtonixDeviceBase::_summary (bool showMemory) {
	this->_summary("", showMemory);
}

void ProtonixDeviceBase::_summary (String additional, bool showMemory) {
	String sensors = "";
	unsigned int i = 0;
	unsigned int count = this->_status->SensorCount();

	while (i < count) {
		sensors += " [s"
			+ this->_status->Sensors()[i]->ID() + ":"
			+ this->_status->Sensors()[i]->Value() + ""
			+ (this->_status->Sensors()[i]->Active() ? ":active" : "")
			+ (this->_status->Sensors()[i]->Failure() ? ":failure" : "")
			+ "]";

		i++;
	}

	this->_status->Summary(
		"[on:" + String(this->_on ? "yes" : "no") + "] " +
		sensors +
		(showMemory ? " [memory:" + String(esp_get_free_heap_size()) + "]" : "") +
		additional
	);
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

void ProtonixDeviceBase::DeviceOnCommand (ProtonixDevice* device, DTO::DTOEventCommand* command) {
	this->_cmd = command->Name();

	if (this->_debug)
		Serial.println("[device:command] " + this->_cmd);

	if (this->_cmd == "std:on") {
		this->_on = true;
		this->DeviceOnCommandStdOn(device);
	}

	if (this->_cmd == "std:off") {
		this->_on = false;
		this->DeviceOnCommandStdOff(device);
	}

	if (this->_cmd.substring(0, 7) == "custom:") {
		this->DeviceOnCommandCustom(device, this->_cmd.substring(7));
	}

	unsigned int i = 0;
	unsigned int count = this->_status->SensorCount();
	String id = "";

	while (i < count) {
		id = this->_status->Sensors()[i]->ID();

		if (this->_cmd == "std:sensor:test[" + id + ":value]") {
			this->_status->Sensors()[i]->Value("true");
		}

		if (this->_cmd == "std:sensor:test[" + id + ":active]") {
			this->_status->Sensors()[i]->Active(true);
		}

		if (this->_cmd == "std:sensor:test[" + id + ":failure]") {
			this->_status->Sensors()[i]->Failure(true);
		}

		if (this->_cmd == "std:sensor:reset[" + id + "]") {
			this->_status->Sensors()[i]->Value("");
			this->_status->Sensors()[i]->Active(false);
			this->_status->Sensors()[i]->Failure(false);
		}

		i++;
	}
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
	this->_dtoRequestAuthorization = new DTO::DTORequestAuthorization();
	this->_dtoRequestDeviceStatus = new DTO::DTORequestDeviceStatus();
	this->_dtoResponseAuthorization = new DTO::DTOResponseAuthorization();
	this->_dtoResponseDeviceStatus = new DTO::DTOResponseDeviceStatus();
	this->_dtoEventCommand = new DTO::DTOEventCommand();
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
	this->_device->DeviceOnTick(this);

	if (this->_device->DeviceAutoStatus())
		this->RequestStream("/api/mechanism/status", this->_dtoRequestDeviceStatus->Reset(this->_device->DeviceStatus()));
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
	
	this->_dtoOutput->Reset();
}

void ProtonixDevice::RequestStreamAuthorize () {
	this->RequestStream("/api/authorize/mechanism", this->_dtoRequestAuthorization->Reset(
		this->_device->DeviceID(),
		this->_device->DevicePassphrase()
	));
}

void ProtonixDevice::OnStream (unsigned char* data) {
	if (this->_debug)
		Serial.println("[OnStream] " + String((char*)data));

	//this->_dtoInput->Debug(this->_debug);
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

void ProtonixDevice::_onStreamURL () {
	if (this->_debug)
		Serial.println("[url] " + this->_dtoInput->URL());
}

void ProtonixDevice::_onStreamResponse () {
	if (this->_debug)
		Serial.println("[response] " + this->_dtoInput->Response());

	this->_device->DeviceOnStreamResponse(this, this->_dtoInput);

	if (this->_dtoInput->Response() == "/api/authorize/mechanism") {
		this->_dtoResponseAuthorization->DTOPopulate(this->_dtoInput);

		this->_device->DeviceOnAuthorization(this, this->_dtoResponseAuthorization);
	}

	if (this->_dtoInput->Response() == "/api/mechanism/status") {
		this->_dtoResponseDeviceStatus->DTOPopulate(this->_dtoInput);

		if (this->_dtoResponseDeviceStatus->Status() != 200)
			this->RequestStreamAuthorize();
	}
}

void ProtonixDevice::_onStreamEvent () {
	if (this->_debug)
		Serial.println("[event] " + this->_dtoInput->Event());

	this->_device->DeviceOnStreamEvent(this, this->_dtoInput);

	if (this->_dtoInput->Event() == "/api/mechanism/command/" + this->_device->DeviceID()) {
		this->_dtoEventCommand->DTOPopulate(this->_dtoInput);

		this->_device->DeviceOnCommand(this, this->_dtoEventCommand);
	}
}

ProtonixDTO* ProtonixDevice::DTOInput () {
	return this->_dtoInput;
}

ProtonixDTO* ProtonixDevice::DTOOutput() {
	return this->_dtoOutput;
}

DTO::DTORequestAuthorization* ProtonixDevice::DTORequestAuthorization() {
	return this->_dtoRequestAuthorization;
}

DTO::DTORequestDeviceStatus* ProtonixDevice::DTORequestDeviceStatus() {
	return this->_dtoRequestDeviceStatus;
}

DTO::DTOResponseAuthorization* ProtonixDevice::DTOResponseAuthorization () {
	return this->_dtoResponseAuthorization;
}

DTO::DTOResponseDeviceStatus* ProtonixDevice::DTOResponseDeviceStatus() {
	return this->_dtoResponseDeviceStatus;
}

DTO::DTOEventCommand* ProtonixDevice::DTOEventCommand () {
	return this->_dtoEventCommand;
}