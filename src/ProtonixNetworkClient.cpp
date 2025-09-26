#include <Arduino.h>

#include "IProtonixNetworkDriver.h"
#include "IProtonixNetworkClient.h"
#include "Protonix.h"
#include "ProtonixURI.h"

#include "ProtonixNetworkClient.h"

using namespace Qybercom::Protonix;

bool ProtonixNetworkClient::_valid () {
	return this->_network != nullptr && this->_network->NetworkDriverConnected() && this->_client != nullptr;
}

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

	return this->_valid() ? this->_client->NetworkClientConnect(this->_uri) : false;
}

bool ProtonixNetworkClient::Connected () {
	return this->_valid() ? this->_client->NetworkClientConnected() : false;
}

String ProtonixNetworkClient::Receive () {
	return this->_valid() ? this->_client->NetworkClientReceive() : "";
}

bool ProtonixNetworkClient::Send (String data) {
	return this->_valid() ? this->_client->NetworkClientSend(data) : false;
}

bool ProtonixNetworkClient::Close () {
	return this->_valid() ? this->_client->NetworkClientClose() : false;
}

ProtonixNetworkClient::~ProtonixNetworkClient () {
	if (this->_uriOwn)
		delete this->_uri;
}