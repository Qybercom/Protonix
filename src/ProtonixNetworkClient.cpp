#include <Arduino.h>

#include "IProtonixNetworkDriver.h"
#include "IProtonixNetworkClient.h"
#include "Protonix.h"
#include "ProtonixURI.h"

#include "ProtonixNetworkClient.h"

using namespace Qybercom::Protonix;

ProtonixNetworkClient::ProtonixNetworkClient (String uri) {
	this->_uri = new ProtonixURI(uri);
	this->_uriOwn = true;
	this->_network = nullptr;
	this->_client = nullptr;
}

ProtonixNetworkClient::ProtonixNetworkClient (ProtonixURI* uri) {
	this->_uri = uri;
	this->_uriOwn = false;
	this->_network = nullptr;
	this->_client = nullptr;
}

IProtonixNetworkDriver* ProtonixNetworkClient::Network () {
	return this->_network;
}

bool ProtonixNetworkClient::Network (IProtonixNetworkDriver* network) {
	if (network == nullptr) return false;

	this->_network = network;
	this->_client = this->_network->NetworkDriverAllocateClient();

	return true;
}

IProtonixNetworkClient* ProtonixNetworkClient::Client () {
	return this->_client;
}

ProtonixNetworkClient* ProtonixNetworkClient::Client (IProtonixNetworkClient* client) {
	this->_client = client;

	return this;
}

bool ProtonixNetworkClient::Connect () {
	if (this->_network == nullptr)
		this->Network(Protonix::Instance()->NetworkDefault());

	return this->_client == nullptr ? false : this->_client->NetworkClientConnect(this->_uri);
}

bool ProtonixNetworkClient::Connected () {
	return this->_client == nullptr ? false : this->_client->NetworkClientConnected();
}

String ProtonixNetworkClient::Receive () {
	return this->_client == nullptr ? "" : this->_client->NetworkClientReceive();
}

bool ProtonixNetworkClient::Send (String data) {
	return this->_client == nullptr ? false : this->_client->NetworkClientSend(data);
}

bool ProtonixNetworkClient::Close () {
	return this->_client == nullptr ? false : this->_client->NetworkClientClose();
}

ProtonixNetworkClient::~ProtonixNetworkClient () {
	if (this->_uriOwn)
		delete this->_uri;
}