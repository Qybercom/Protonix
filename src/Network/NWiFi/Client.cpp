#include <Arduino.h>

#include "../../ProtonixURI.h"

#include "Client.h"

using namespace Qybercom::Protonix;

::Client* Network::NWiFi::Client::NetworkClientClient () {
	return this->_client;
}

Network::NWiFi::Client::Client () {
	#if defined(ESP32) || defined(ESP8266)
	this->_client = new WiFiClient();
	#else
	this->_client = nullptr;
	#endif
}

bool Network::NWiFi::Client::NetworkClientConnect (ProtonixURI* uri) {
	#if defined(ESP32) || defined(ESP8266)
		#if defined(ESP32)
		bool out = this->_client->connect(uri->Host().c_str(), uri->Port(), 10);
		#else
		bool out = this->_client->connect(uri->Host().c_str(), uri->Port());
		#endif

		if (out) {
			this->_client->setNoDelay(true);
			this->_client->setTimeout(10);
		}

		return out;
	#else
		(void)uri;

		return false;
	#endif
}

bool Network::NWiFi::Client::NetworkClientConnected () {
	#if defined(ESP32) || defined(ESP8266)
	return this->_client->connected();
	#else
	return false;
	#endif
}

String Network::NWiFi::Client::NetworkClientReceive () {
	#if defined(ESP32) || defined(ESP8266)
	int available = this->_client->available();

	int i = 0;
	while (i < 2048) {
		this->_buffer[i] = '\0';

		i++;
	}

	this->_bufferPTR = this->_buffer;

	if (available)
		this->_client->read(this->_bufferPTR, available);
	#endif

	return String((char*)this->_buffer);
}

bool Network::NWiFi::Client::NetworkClientSend (String data) {
	#if defined(ESP32) || defined(ESP8266)
		uint8_t* buffer = (uint8_t*)data.c_str();

		return (bool)this->_client->write(buffer, data.length());
	#else
		(void)data;

		return false;
	#endif
}

bool Network::NWiFi::Client::NetworkClientClose () {
	#if defined(ESP32) || defined(ESP8266)
	this->_client->stop();

	return true;
	#else
	return false;
	#endif
}

Network::NWiFi::Client::~Client () {
	#if defined(ESP32) || defined(ESP8266)
	delete this->_client;
	this->_client = nullptr;
	#endif
}