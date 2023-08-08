#include <Arduino.h>
#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <esp_wifi.h>
#include <ArduinoJson.h>

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

	long diff = this->_previous - current;

	return diff < 0 || diff >= this->_interval;
}


ProtonixDTO::ProtonixDTO () {
	this->_debug = false;
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
	String out;
	StaticJsonDocument<1024> dto;

	if (this->IsURL())
		dto["url"] = this->_url;

	if (this->IsResponse())
		dto["response"] = this->_response;

	if (this->IsEvent())
		dto["event"] = this->_event;

	this->_dto->DTOToJSON(dto);

	serializeJson(dto, out);

	return out;
}

bool ProtonixDTO::Deserialize (String raw) {
	//StaticJsonDocument<2048> buffer;
	//DynamicJsonDocument buffer(2048);
	/*if (!this->_bufferInit) {
		DynamicJsonDocument __buffer__(2048);
		this->_buffer = __buffer__;
		this->_bufferInit = true;
	}*/

	DeserializationError err = deserializeJson(this->_buffer, raw);
	
	if (err) {
		Serial.print(F("deserializeJson() failed with code "));
		Serial.println(err.f_str());
	}

	JsonObject dto = this->_buffer.as<JsonObject>();

	if (dto.containsKey("url")) {
		const char* url = dto["url"];
		this->_url = (String)url;
	}

	if (dto.containsKey("response")) {
		const char* url = dto["response"];
		this->_response = (String)url;
	}

	if (dto.containsKey("event")) {
		const char* url = dto["event"];
		this->_event = (String)url;
	}

	if (dto.containsKey("data"))
		this->_data = dto["data"];

	return true;
}

void ProtonixDTO::Debug (bool debug) {
	this->_debug = debug;
}

bool ProtonixDTO::Debug () {
	return this->_debug;
}



//JsonObject IProtonixDTO::_alloc(unsigned long capacity) {
//	StaticJsonDocument<1024> doc;
//
//	return doc.to<JsonObject>();
//}

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

void DTO::DTOResponseAuthorization::DTOToJSON(JsonDocument& dto) {
	dto["data"]["status"] = this->_status;
}

unsigned short DTO::DTOResponseAuthorization::DTOResponseStatus () {
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

void DTO::DTOEventCommand::DTOToJSON(JsonDocument &dto) {
	dto["data"]["command"] = this->_name;
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
	WiFi.setHostname(this->_hostname.c_str());
	WiFi.mode(WIFI_STA);
	esp_wifi_set_mac(WIFI_IF_STA, &mac[0]);
	WiFi.begin(this->_ssid, this->_password);

	return true;
}

bool Networks::NWiFi::Connected () {
	return WiFi.status() == WL_CONNECTED;
}

String Networks::NWiFi::AddressMAC () {
	return this->_mac;
}

String Networks::NWiFi::AddressIP () {
	return String(WiFi.localIP());
}





void Protocols::PWebSocket::_input (ProtonixDTO* dto) {
	if (dto->IsURL()) {
		if (dto->Debug())
			Serial.println("[PWebSocket::onMessage URL] " + dto->URL());
	}

	if (dto->IsResponse()) {
		if (dto->Debug())
			Serial.println("[PWebSocket::onMessage Response] " + dto->Response());

		this->_device->OnStreamResponse(dto);
	}

	if (dto->IsEvent()) {
		if (dto->Debug())
			Serial.println("[PWebSocket::onMessage Event] " + dto->Event());

		this->_device->OnStreamEvent(dto);
	}
}

void Protocols::PWebSocket::Init (ProtonixDevice* device) {
	this->_device = device;
	this->_client.onMessage([&](websockets::WebsocketsMessage message) {
		bool debug = this->_device->Debug();

		if (debug)
			Serial.println("[PWebSocket::onMessage] " + message.data());

		ProtonixDTO* dto = new ProtonixDTO();
		dto->Debug(debug);
		dto->Deserialize(message.data());

		this->_input(dto);
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




ProtonixDevice::ProtonixDevice (IProtonixDevice* device) {
	this->_ready = false;
	this->_networkConnected1 = false;
	this->_networkConnected2 = false;
	this->_protocolConnected1 = false;
	this->_protocolConnected2 = false;

	this->Debug(false);
	this->Device(device);
	this->_timer = new ProtonixTimer(this->_device->DeviceTick());
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

		this->RequestStream("/api/authorize/mechanism", new DTO::DTORequestAuthorization(
			this->_device->DeviceID(),
			this->_device->DevicePassphrase()
		));
	}

	this->_protocol->Pipe();
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
	ProtonixDTO* dto = new ProtonixDTO();

	dto->Debug(this->Debug());
	dto->URL(url);
	dto->DTO(new DTO::DTORequestAuthorization(
		this->_device->DeviceID(),
		this->_device->DevicePassphrase()
	));

	String raw = dto->Serialize();
	this->_protocol->Send(raw);

	if (this->_debug)
		Serial.println("[request] " + url + " ok: " + raw);
}

void ProtonixDevice::OnStreamResponse (ProtonixDTO* dto) {
	if (this->_debug)
		Serial.println("[response] " + dto->Response());

	this->_device->DeviceOnStreamResponse(this, dto);
}

void ProtonixDevice::OnStreamEvent (ProtonixDTO* dto) {
	if (this->_debug)
		Serial.println("[event] " + dto->Event());

	this->_device->DeviceOnStreamEvent(this, dto);

	if (dto->Event() == "/api/mechanism/command/" + this->_device->DeviceID()) {
		if (this->_debug)
			Serial.println("[command] " + dto->Event());

		DTO::DTOEventCommand* command = new DTO::DTOEventCommand();
		command->DTOPopulate(dto);

		this->_device->DeviceOnCommand(this, command);
	}
}