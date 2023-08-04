#include <Arduino.h>
#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <esp_wifi.h>
#include <ArduinoJson.h>

#include "QybercomProtonix.h"


Qybercom::Protonix::ProtonixURI::ProtonixURI (String host, uint port) {
	this->Host(host);
	this->Port(port);
}

Qybercom::Protonix::ProtonixURI::ProtonixURI (String host, uint port, String path) {
	this->Host(host);
	this->Port(port);
	this->Path(path);
}

void Qybercom::Protonix::ProtonixURI::Scheme (String scheme) {
	this->_scheme = scheme;
}

String Qybercom::Protonix::ProtonixURI::Scheme () {
	return this->_scheme;
}

void Qybercom::Protonix::ProtonixURI::Username (String username) {
	this->_username = username;
}

String Qybercom::Protonix::ProtonixURI::Username () {
	return this->_username;
}

void Qybercom::Protonix::ProtonixURI::Password (String password) {
	this->_password = password;
}

String Qybercom::Protonix::ProtonixURI::Password () {
	return this->_password;
}

void Qybercom::Protonix::ProtonixURI::Host (String host) {
	this->_host = host;
}

String Qybercom::Protonix::ProtonixURI::Host () {
	return this->_host;
}

void Qybercom::Protonix::ProtonixURI::Port (uint port) {
	this->_port = port;
}

uint Qybercom::Protonix::ProtonixURI::Port () {
	return this->_port;
}

void Qybercom::Protonix::ProtonixURI::Path (String path) {
	this->_path = path;
}

String Qybercom::Protonix::ProtonixURI::Path () {
	return this->_path;
}




Qybercom::Protonix::ProtonixTimer::ProtonixTimer () {
	this->_previous = 0;

	this->Interval(0);
	this->Unit(Qybercom::Protonix::ProtonixTimer::ProtonixTimerUnit::MILLISECONDS);
	this->Callback([](Qybercom::Protonix::ProtonixTimer* timer) {});
}

Qybercom::Protonix::ProtonixTimer::ProtonixTimer (unsigned int interval) {
	this->_previous = 0;

	this->Interval(interval);
	this->Unit(Qybercom::Protonix::ProtonixTimer::ProtonixTimerUnit::MILLISECONDS);
	this->Callback([](Qybercom::Protonix::ProtonixTimer* timer) {});
}

Qybercom::Protonix::ProtonixTimer::ProtonixTimer (unsigned int interval, Qybercom::Protonix::ProtonixTimer::ProtonixTimerUnit unit) {
	this->_previous = 0;

	this->Interval(interval);
	this->Unit(unit);
	this->Callback([](Qybercom::Protonix::ProtonixTimer* timer) {});
}

Qybercom::Protonix::ProtonixTimer::ProtonixTimer (unsigned int interval, Qybercom::Protonix::ProtonixTimer::ProtonixTimerCallback callback) {
	this->_previous = 0;

	this->Interval(interval);
	this->Unit(Qybercom::Protonix::ProtonixTimer::ProtonixTimerUnit::MILLISECONDS);
	this->Callback(callback);
}

Qybercom::Protonix::ProtonixTimer::ProtonixTimer (unsigned int interval, Qybercom::Protonix::ProtonixTimer::ProtonixTimerUnit unit, Qybercom::Protonix::ProtonixTimer::ProtonixTimerCallback callback) {
	this->_previous = 0;

	this->Interval(interval);
	this->Unit(unit);
	this->Callback(callback);
}

unsigned long Qybercom::Protonix::ProtonixTimer::Previous () {
	return this->_previous;
}

void Qybercom::Protonix::ProtonixTimer::Interval (int interval) {
	this->_interval = interval;
}

unsigned int Qybercom::Protonix::ProtonixTimer::Interval () {
	return this->_interval;
}

void Qybercom::Protonix::ProtonixTimer::Unit (Qybercom::Protonix::ProtonixTimer::ProtonixTimerUnit unit) {
	this->_unit = unit;
}

Qybercom::Protonix::ProtonixTimer::ProtonixTimerUnit Qybercom::Protonix::ProtonixTimer::Unit () {
	return this->_unit;
}

Qybercom::Protonix::ProtonixTimer* Qybercom::Protonix::ProtonixTimer::Callback (Qybercom::Protonix::ProtonixTimer::ProtonixTimerCallback callback) {
	this->_callback = callback;

	return this;
}

void Qybercom::Protonix::ProtonixTimer::Pipe () {
	unsigned long current = 0;

	if (this->_unit == Qybercom::Protonix::ProtonixTimer::ProtonixTimerUnit::MILLISECONDS)
		current = millis();

	if (this->_unit == Qybercom::Protonix::ProtonixTimer::ProtonixTimerUnit::MICROSECONDS)
		current = micros();

	long diff = this->_previous - current;

	if (diff < 0 || diff >= this->_interval)
		this->_callback(this);
}





Qybercom::Protonix::Networks::NWiFi::NWiFi (String ssid, String password, String mac, String hostname) {
	this->_ssid = ssid;
	this->_password = password;
	this->_mac = mac;
	this->_hostname = hostname;
}

bool Qybercom::Protonix::Networks::NWiFi::Connect () {
	uint8_t mac[6] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
	Qybercom::Protonix::INetwork::ParseMAC(this->_mac, mac);

	// https://randomnerdtutorials.com/esp32-set-custom-hostname-arduino/#comment-741757
	WiFi.setHostname(this->_hostname.c_str());
	WiFi.mode(WIFI_STA);
	esp_wifi_set_mac(WIFI_IF_STA, &mac[0]);
	WiFi.begin(this->_ssid, this->_password);

	return true;
}

bool Qybercom::Protonix::Networks::NWiFi::Connected () {
	return WiFi.status() == WL_CONNECTED;
}

String Qybercom::Protonix::Networks::NWiFi::AddressMAC () {
	return this->_mac;
}

String Qybercom::Protonix::Networks::NWiFi::AddressIP () {
	return String(WiFi.localIP());
}





bool Qybercom::Protonix::Protocols::PWebSocket::Connect (Qybercom::Protonix::ProtonixURI* uri) {
	return this->_client.connect(
		uri->Host(),
		uri->Port(),
		uri->Path()
	);
}

bool Qybercom::Protonix::Protocols::PWebSocket::Connected () {
	return this->_client.available();
}

void Qybercom::Protonix::Protocols::PWebSocket::Pipe() {
	this->_client.poll();
}






Qybercom::Protonix::ProtonixDevice::ProtonixDevice (Qybercom::Protonix::IProtonixDevice* device) {
	this->_networkConnected = false;
	this->_protocolConnected = false;
}

//void Qybercom::Protonix::ProtonixDevice::ID (String id) {
//	this->_id = id;
//}
//
//String Qybercom::Protonix::ProtonixDevice::ID () {
//	return this->_id;
//}
//
//void Qybercom::Protonix::ProtonixDevice::Passphrase (String passphrase) {
//	this->_passphrase = passphrase;
//}
//
//String Qybercom::Protonix::ProtonixDevice::Passphrase () {
//	return this->_passphrase;
//}

void Qybercom::Protonix::ProtonixDevice::Network (Qybercom::Protonix::INetwork* network) {
	this->_network = network;
}

Qybercom::Protonix::INetwork* Qybercom::Protonix::ProtonixDevice::Network () {
	return this->_network;
}

void Qybercom::Protonix::ProtonixDevice::Protocol (Qybercom::Protonix::IProtocol* protocol) {
	this->_protocol = protocol;
}

Qybercom::Protonix::IProtocol* Qybercom::Protonix::ProtonixDevice::Protocol () {
	return this->_protocol;
}

void Qybercom::Protonix::ProtonixDevice::Server (Qybercom::Protonix::ProtonixURI* uri) {
	this->_uri = uri;
}

Qybercom::Protonix::ProtonixURI* Qybercom::Protonix::ProtonixDevice::Server () {
	return this->_uri;
}

void Qybercom::Protonix::ProtonixDevice::ServerEndpoint (String host, uint port) {
	this->Server(new Qybercom::Protonix::ProtonixURI(host, port));
}

void Qybercom::Protonix::ProtonixDevice::ServerEndpoint (String host, uint port, String path) {
	this->Server(new Qybercom::Protonix::ProtonixURI(host, port, path));
}

void Qybercom::Protonix::ProtonixDevice::Pipe () {
	/*if (this->_networkConnected && this->_protocolConnected) this->_protocol->Pipe();
	else {
		if (!this->_networkConnected || !this->_network->Connected()) {
			if (!this->_networkConnected) {
				Serial.println("[network:connect]");
				this->_network->Connect();
				this->_networkConnected = true;
			}
			Serial.print(".");

			if (this->_network->Connected()) {
				Serial.println("[network:connected]");
				this->_onNetworkConnect(this);
			}
		}

		if (!this->_protocolConnected || !this->_protocol->Connected()) {
			this->_protocol->Connect(this->_uri);
			this->_protocolConnected = true;

			this->_onProtocolConnect(this);
		}
	}

	delay(tick);*/
}

//Qybercom::Protonix::ProtonixDevice* Qybercom::Protonix::ProtonixDevice::OnNetworkConnect (Qybercom::Protonix::ProtonixDevice::NetworkConnectCallback callback) {
//	this->_onNetworkConnect = callback;
//
//	return this;
//}
//
//Qybercom::Protonix::ProtonixDevice* Qybercom::Protonix::ProtonixDevice::OnProtocolConnect (Qybercom::Protonix::ProtonixDevice::ProtocolConnectCallback callback) {
//	this->_onProtocolConnect = callback;
//
//	return this;
//}