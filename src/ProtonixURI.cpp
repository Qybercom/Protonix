#include <Arduino.h>

#include "ProtonixURI.h"

using namespace Qybercom::Protonix;



ProtonixURI::ProtonixURI(String host, unsigned int port) {
	this->Host(host);
	this->Port(port);
}

ProtonixURI::ProtonixURI(String host, unsigned int port, String path) {
	this->Host(host);
	this->Port(port);
	this->Path(path);
}

void ProtonixURI::Scheme(String scheme) {
	this->_scheme = scheme;
}

String ProtonixURI::Scheme() {
	return this->_scheme;
}

void ProtonixURI::Username(String username) {
	this->_username = username;
}

String ProtonixURI::Username() {
	return this->_username;
}

void ProtonixURI::Password(String password) {
	this->_password = password;
}

String ProtonixURI::Password() {
	return this->_password;
}

void ProtonixURI::Host(String host) {
	this->_host = host;
}

String ProtonixURI::Host() {
	return this->_host;
}

void ProtonixURI::Port(unsigned int port) {
	this->_port = port;
}

unsigned int ProtonixURI::Port() {
	return this->_port;
}

void ProtonixURI::Path(String path) {
	this->_path = path;
}

String ProtonixURI::Path() {
	return this->_path;
}