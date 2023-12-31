#include <Arduino.h>

#if defined(ESP32)
#include <WiFi.h>
#include <esp_wifi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#endif

#include "../IProtonixNetwork.h"
#include "NWiFi.h"

using namespace Qybercom::Protonix;



Network::NWiFi::NWiFi(String ssid, String password, String mac, String hostname) {
	this->_ssid = ssid;
	this->_password = password;
	this->_mac = mac;
	this->_hostname = hostname;
}

bool Network::NWiFi::Connect() {
	//uint8_t mac[6] = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
	IProtonixNetwork::ParseMAC(this->_mac, this->_macBuffer);

	#if defined(ESP32) || defined(ESP8266)
		WiFi.disconnect(true);
	#endif

	// https://randomnerdtutorials.com/esp32-set-custom-hostname-arduino/#comment-741757
	#if defined(ESP32)
		WiFi.setHostname(this->_hostname.c_str());
		WiFi.mode(WIFI_STA);
		esp_wifi_set_mac(WIFI_IF_STA, &this->_macBuffer[0]);
	#elif defined(ESP8266)
		WiFi.mode(WIFI_STA);
		wifi_set_macaddr(STATION_IF, &this->_macBuffer[0]);
		WiFi.setHostname(this->_hostname.c_str());
	#else
	#endif

	#if defined(ESP32) || defined(ESP8266)
		WiFi.begin(this->_ssid, this->_password);
	#endif

	delay(1000);
	/*
	WiFi.reconnect();
	*/

	return true;
}

unsigned int __NWiFi_status = 0;

bool Network::NWiFi::Connected() {
	#if defined(ESP32) || defined(ESP8266)
	unsigned int status = WiFi.status();
	bool connected = status == WL_CONNECTED;
	
	if (!connected && status != __NWiFi_status) {
		Serial.print("[WARNING] WiFi status: ");
		Serial.println(status);
	}
	
	__NWiFi_status = status;

	return connected;
	#else
	return false;
	#endif
}

bool Network::NWiFi::Disconnect() {
	#if defined(ESP32) || defined(ESP8266)
	WiFi.disconnect();
	#endif

	return true;
}

String Network::NWiFi::AddressMAC() {
	return this->_mac;
}

String Network::NWiFi::AddressIP() {
	#if defined(ESP32)
	return String(WiFi.localIP());
	#elif defined(ESP8266)
	return WiFi.localIP().toString();
	#else
	return "";
	#endif
}