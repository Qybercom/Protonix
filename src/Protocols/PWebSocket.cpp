#include <Arduino.h>

#if defined(ESP32) || defined(ESP8266)
#include <ArduinoWebsockets.h>
#endif

#include "../ProtonixURI.h"
#include "../ProtonixDevice.h"
#include "PWebSocket.h"

using namespace Qybercom::Protonix;



void Protocol::PWebSocket::Init(ProtonixDevice* device) {
	this->_device = device;

	#if defined(ESP32) || defined(ESP8266)
	this->_client.onMessage([&](websockets::WebsocketsMessage message) {
		this->_device->OnStream((unsigned char*)message.data().c_str());
	});
	#endif
}

bool Protocol::PWebSocket::Connect(ProtonixURI* uri) {
	#if defined(ESP32) || defined(ESP8266)
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