#include <Arduino.h>

#if defined(ESP32) || defined(ESP8266)
#include <ArduinoWebsockets.h>
#endif

#include "../ProtonixURI.h"
#include "../ProtonixDevice.h"
#include "PWebSocket.h"

using namespace Qybercom::Protonix;



void Protocol::PWebSocket::Init(ProtonixDevice* device) {
	Serial.println("[debug:wifi] memory:init:" + String(device->FreeRAM()));
	this->_device = device;

#if defined(ESP32) || defined(ESP8266)
	// https://github.com/gilmaimon/ArduinoWebsockets/issues/75#issuecomment-635420749
	//this->_client = {};
	if (!this->_init) {
		this->_init = true;
		this->_client.onMessage([&](websockets::WebsocketsMessage message) {
			this->_device->OnStream((unsigned char*)message.data().c_str());
		});
	}
	#endif
}

bool Protocol::PWebSocket::Connect(ProtonixURI* uri) {
	#if defined(ESP32) || defined(ESP8266)
	Serial.println("[debug:wifi] memory:connect:" + String(this->_device->FreeRAM()));
	return this->_client.connect(
		uri->Host(),
		uri->Port(),
		uri->Path()
	);
	#else
	return false;
	#endif
}

bool Protocol::PWebSocket::Connected() {
	#if defined(ESP32) || defined(ESP8266)
	return this->_client.available();
	#else
	return false;
	#endif
}

void Protocol::PWebSocket::Pipe() {
	#if defined(ESP32) || defined(ESP8266)
	this->_client.poll();
	#endif
}

void Protocol::PWebSocket::Send(String raw) {
	#if defined(ESP32) || defined(ESP8266)
	this->_client.send(raw);
	#endif
}