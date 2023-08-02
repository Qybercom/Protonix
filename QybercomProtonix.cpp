#include <Arduino.h>
#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <esp_wifi.h>
#include <ArduinoJson.h>

#include "QybercomProtonix.h"


Qybercom::Protonix::URI::URI (String host, uint port) {
	this->Host(host);
	this->Port(port);
}

Qybercom::Protonix::URI::URI (String host, uint port, String path) {
	this->Host(host);
	this->Port(port);
	this->Path(path);
}

void Qybercom::Protonix::URI::Scheme (String scheme) {
	this->_scheme = scheme;
}

String Qybercom::Protonix::URI::Scheme () {
	return this->_scheme;
}

void Qybercom::Protonix::URI::Username (String username) {
	this->_username = username;
}

String Qybercom::Protonix::URI::Username () {
	return this->_username;
}

void Qybercom::Protonix::URI::Password (String password) {
	this->_password = password;
}

String Qybercom::Protonix::URI::Password () {
	return this->_password;
}

void Qybercom::Protonix::URI::Host (String host) {
	this->_host = host;
}

String Qybercom::Protonix::URI::Host () {
	return this->_host;
}

void Qybercom::Protonix::URI::Port (uint port) {
	this->_port = port;
}

uint Qybercom::Protonix::URI::Port () {
	return this->_port;
}

void Qybercom::Protonix::URI::Path (String path) {
	this->_path = path;
}

String Qybercom::Protonix::URI::Path () {
	return this->_path;
}










Qybercom::Protonix::Networks::NWiFi::NWiFi (String ssid, String password, String mac) {
	this->_ssid = ssid;
	this->_password = password;
	this->_mac = mac;
}

bool Qybercom::Protonix::Networks::NWiFi::Connect () {
	uint8_t mac[6] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
	String buffer = this->_mac;
	buffer.replace(":", "");
	if (buffer.length() != 12) return false;

	uint i = 0;
	while (i < 6) {
		mac[i] = atoi(buffer.substring(i, 2).c_str());

		i++;
	}
	
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





void Qybercom::Protonix::Protocols::PWebSocket::Connect () {

}






Qybercom::Protonix::Device::Device (String id, String passphrase) {
	this->ID(id);
	this->Passphrase(passphrase);
}

void Qybercom::Protonix::Device::ID (String id) {
	this->_id = id;
}

String Qybercom::Protonix::Device::ID () {
	return this->_id;
}

void Qybercom::Protonix::Device::Passphrase (String passphrase) {
	this->_passphrase = passphrase;
}

String Qybercom::Protonix::Device::Passphrase () {
	return this->_passphrase;
}

void Qybercom::Protonix::Device::Network (Qybercom::Protonix::INetwork* network) {
	this->_network = network;
}

Qybercom::Protonix::INetwork* Qybercom::Protonix::Device::Network () {
	return this->_network;
}

void Qybercom::Protonix::Device::Protocol (Qybercom::Protonix::IProtocol* protocol) {
	this->_protocol = protocol;
}

Qybercom::Protonix::IProtocol* Qybercom::Protonix::Device::Protocol () {
	return this->_protocol;
}

void Qybercom::Protonix::Device::Server (Qybercom::Protonix::URI* uri) {
	this->_uri = uri;
}

Qybercom::Protonix::URI* Qybercom::Protonix::Device::Server () {
	return this->_uri;
}

void Qybercom::Protonix::Device::ServerEndpoint (String host, uint port) {
	this->Server(new Qybercom::Protonix::URI(host, port));
}

void Qybercom::Protonix::Device::ServerEndpoint (String host, uint port, String path) {
	this->Server(new Qybercom::Protonix::URI(host, port, path));
}

void Qybercom::Protonix::Device::Pipe () {
	
}

//Qybercom::Protonix::Device* Qybercom::Protonix::Device::OnNetworkConnect (void (*callback)(Qybercom::Protonix::Device*)) {
Qybercom::Protonix::Device* Qybercom::Protonix::Device::OnNetworkConnect(Qybercom::Protonix::Device::NetworkConnectCallback callback) {
	this->_onNetworkConnect = callback;

	return this;
}

//Qybercom::Protonix::Device* Qybercom::Protonix::Device::OnProtocolConnect (void (*callback)(Qybercom::Protonix::Device*)) {
Qybercom::Protonix::Device* Qybercom::Protonix::Device::OnProtocolConnect(Qybercom::Protonix::Device::ProtocolConnectCallback callback) {
	this->_onProtocolConnect = callback;

	return this;
}