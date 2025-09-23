#include <Arduino.h>

#include "../../ProtonixURI.h"

#include "Client.h"

using namespace Qybercom::Protonix;

::Client* Network::NEthernet::Client::NetworkClientClient () {
	return this->_client;
}

Network::NEthernet::Client::Client () {
	this->_client = new EthernetClient();
}

bool Network::NEthernet::Client::NetworkClientConnect (ProtonixURI* uri) {
	if (uri == nullptr || uri->EmptySocket()) return false;

	bool out = this->_client->connect(uri->Host().c_str(), uri->Port());

	if (out) {
		this->_client->setConnectionTimeout(10);
	}

	return out;
}

bool Network::NEthernet::Client::NetworkClientConnected () {
	return this->_client->connected();
}

String Network::NEthernet::Client::NetworkClientReceive () {
	int available = this->_client->available();

	int i = 0;
	while (i < 2048) {
		this->_buffer[i] = '\0';

		i++;
	}

	this->_bufferPTR = this->_buffer;

	if (available)
		this->_client->read(this->_bufferPTR, available);

	return String((char*)this->_buffer);
}

bool Network::NEthernet::Client::NetworkClientSend (String data) {
	uint8_t* buffer = (uint8_t*)data.c_str();

	bool out = (bool)this->_client->write(buffer, data.length());
	this->_client->flush();

	return out;
}

bool Network::NEthernet::Client::NetworkClientClose () {
	this->_client->stop();

	return true;
}

Network::NEthernet::Client::~Client () {
	delete this->_client;
	this->_client = nullptr;
}