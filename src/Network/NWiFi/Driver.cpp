#include <Arduino.h>

#if defined(ESP32)
#include <WiFi.h>
#include <esp_wifi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#endif

#include "../../IProtonixNetworkDriver.h"
#include "../../IProtonixNetworkClient.h"

#include "Client.h"
#include "Driver.h"

using namespace Qybercom::Protonix;

String Network::NWiFi::Driver::_statusRecognize (int code) {
	switch (code) {
		case -1:  return "__init__";
		case 0:   return "WL_IDLE_STATUS";
		case 1:   return "WL_NO_SSID_AVAIL";
		case 2:   return "WL_SCAN_COMPLETED";
		case 3:   return "WL_CONNECTED";
		case 4:   return "WL_CONNECT_FAILED";
		case 5:   return "WL_CONNECTION_LOST";
		case 6:   return "WL_DISCONNECTED";
		// WiFiNINA/WiFi101
		case 7:   return "WL_AP_LISTENING";
		case 8:   return "WL_AP_CONNECTED";
		case 9:   return "WL_AP_FAILED";
		case 254: return "WL_STOPPED";
		case 255: return "WL_NO_SHIELD";
		default:  return "__unknown__";
	}
}

Network::NWiFi::Driver::Driver (String ssid, String password, String mac, String hostname, bool connectAuto, unsigned int timerConnect) {
	if (!this->_init(true, "wifi", "wlan", mac, hostname, connectAuto, timerConnect)) return;

	this->_ssid = ssid;
	this->_password = password;
	this->_status = -1;
}

bool Network::NWiFi::Driver::NetworkDriverConnect () {
	if (!this->_ready)
		return this->_log("Only 1 NDWiFi driver supported", false);

	this->_log("Connect");
	this->_macParse();

	#if defined(ESP32) || defined(ESP8266)
		WiFi.disconnect(true);
	#endif

	// TODO: add AP mode
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

	return true;
}

bool Network::NWiFi::Driver::NetworkDriverConnected () {
	if (!this->_ready) return false;

	#if defined(ESP32) || defined(ESP8266)
		unsigned int status = WiFi.status();
		bool connected = status == WL_CONNECTED;

		if (status != this->_status)
			this->_log("Connection status: " + this->_statusRecognize(status));

		this->_status = status;

		return connected;
	#else
		return false;
	#endif
}

bool Network::NWiFi::Driver::NetworkDriverDisconnect () {
	if (!this->_ready) return false;

	this->_log("Disconnect");

	this->_connectAuto = false;

	#if defined(ESP32) || defined(ESP8266)
		WiFi.disconnect();
	#endif

	return true;
}

String Network::NWiFi::Driver::NetworkDriverAddressMAC () {
	return this->_mac;
}

String Network::NWiFi::Driver::NetworkDriverAddressIP () {
	#if defined(ESP32)
		return String(WiFi.localIP());
	#elif defined(ESP8266)
		return WiFi.localIP().toString();
	#else
		return "";
	#endif
}

IProtonixNetworkClient* Network::NWiFi::Driver::NetworkDriverAllocateClient () {
	return new Network::NWiFi::Client();
}