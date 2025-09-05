#include <Arduino.h>

#include "../ProtonixURI.h"
#include "../ProtonixDevice.h"
#include "PWiFiTCP.h"

using namespace Qybercom::Protonix;



void Protocol::PWiFiTCP::Init(ProtonixDevice* device) {
	this->_device = device;
}

bool Protocol::PWiFiTCP::Connect(ProtonixURI* uri) {
	#if defined(ESP32) || defined(ESP8266)
	this->_client.connect(uri->Host().c_str(), uri->Port());
	return true;
	#else
	return false;
	#endif
}

bool Protocol::PWiFiTCP::Connected() {
	#if defined(ESP32) || defined(ESP8266)
	return this->_client.connected();
	#else
	return false;
	#endif
}

void Protocol::PWiFiTCP::Pipe() {
	#if defined(ESP32) || defined(ESP8266)
	int available = this->_client.available();

	if (available) {
		int i = 0;
		while (i < 2048) {
			this->_buffer[i] = '\0';

			i++;
		}

		this->_bufferPTR = this->_buffer;
		this->_client.read(this->_bufferPTR, available);

		this->_device->OnStream(this->_buffer);
	}
	#endif
}

void Protocol::PWiFiTCP::Send(String data) {
	#if defined(ESP32) || defined(ESP8266)
	uint8_t* buffer = (uint8_t*)data.c_str();

	this->_client.write(buffer, data.length());
	#endif
}