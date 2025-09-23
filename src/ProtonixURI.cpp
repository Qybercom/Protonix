#include <Arduino.h>

#include "ProtonixURI.h"

using namespace Qybercom::Protonix;



ProtonixURI::ProtonixURI () {
	this->Scheme("");
	this->Username("");
	this->Password("");
	this->Host("");
	this->Port(0);
	this->Path("");
	this->Query("");
	this->Fragment("");
}

ProtonixURI::ProtonixURI (String uri) {
	this->Scheme("");
	this->Username("");
	this->Password("");
	this->Host("");
	this->Port(0);
	this->Path("");
	this->Query("");
	this->Fragment("");

	int posScheme = uri.indexOf("://");
	if (posScheme == -1) return; // TODO: handle incomplete

	this->Scheme(uri.substring(0, posScheme));

	String buffer = uri.substring(posScheme + 3);
	int posPath = buffer.indexOf('/');

	String authority = posPath == -1 ? buffer.substring(0) : buffer.substring(0, posPath);
	String path = posPath == -1 ? "/" : buffer.substring(posPath);

	buffer = authority;
	int posUser = buffer.indexOf('@');
	if (posUser != -1) {
		String user = buffer.substring(0, posUser);
		buffer = buffer.substring(posUser + 1);

		int posPassword = user.indexOf(':');

		this->Username(posPassword == -1 ? user : user.substring(0, posPassword));
		if (posPassword != -1) this->Password(user.substring(posPassword + 1));
	}

	int posPort = buffer.indexOf(':');

	this->Host(posPort == -1 ? buffer : buffer.substring(0, posPort));
	this->Port(posPort == -1 ? 0 : buffer.substring(posPort + 1).toInt());

	int posQuery = path.indexOf('?');
	this->Path(posQuery == -1 ? path : path.substring(0, posQuery));

	buffer = posQuery == -1 ? "" : path.substring(posQuery + 1);
	int posFragment = buffer.indexOf('#');
	this->Query(posFragment == -1 ? buffer : buffer.substring(0, posFragment));
	if (posFragment != -1) this->Fragment(buffer.substring(posFragment + 1));
}

ProtonixURI::ProtonixURI (String host, unsigned int port) {
	this->Scheme("");
	this->Username("");
	this->Password("");
	this->Host(host);
	this->Port(port);
	this->Path("");
	this->Query("");
	this->Fragment("");
}

ProtonixURI::ProtonixURI (String host, unsigned int port, String path) {
	this->Scheme("");
	this->Username("");
	this->Password("");
	this->Host(host);
	this->Port(port);
	this->Path(path);
	this->Query("");
	this->Fragment("");
}

void ProtonixURI::Scheme (String scheme) {
	this->_scheme = scheme;
}

String ProtonixURI::Scheme () {
	return this->_scheme;
}

void ProtonixURI::Username (String username) {
	this->_username = username;
}

String ProtonixURI::Username () {
	return this->_username;
}

void ProtonixURI::Password (String password) {
	this->_password = password;
}

String ProtonixURI::Password () {
	return this->_password;
}

void ProtonixURI::Host (String host) {
	this->_host = host;
}

String ProtonixURI::Host () {
	return this->_host;
}

void ProtonixURI::Port (unsigned int port) {
	this->_port = port;
}

unsigned int ProtonixURI::Port () {
	return this->_port;
}

unsigned int ProtonixURI::PortConnection () {
	if (this->_port == 0) {
		if (this->_scheme == "http") return 80;
		if (this->_scheme == "https") return 443;
		if (this->_scheme == "ws") return 80;
		if (this->_scheme == "wss") return 443;
	}

	return this->_port;
}

void ProtonixURI::Path (String path) {
	this->_path = path;
}

String ProtonixURI::Path () {
	return this->_path;
}

void ProtonixURI::Query (String query) {
	this->_query = query;
}

String ProtonixURI::Query () {
	return this->_query;
}

String ProtonixURI::Query (bool includePath) {
	return String(includePath ? this->_path : "") + this->_query;
}

void ProtonixURI::Fragment (String fragment) {
	this->_fragment = fragment;
}

String ProtonixURI::Fragment () {
	return this->_fragment;
}

String ProtonixURI::URI () {
	String path = String(this->_path ? this->_path : "/");
	String uri = this->_scheme + "://"
		+ String(this->_username != "" ? this->_username + String(this->_password != "" ? ":" + this->_password : "") + "@" : "")
		+ this->_host + String(this->_port ? ":" + this->_port : "") + path
		+ String(this->_query != "" ? "?" + this->_query : "")
		+ String(this->_fragment != "" ? "#" + this->_fragment : "");

	// TODO: handle trailing slash

	return this->_fragment;
}

bool ProtonixURI::EmptySocket () {
	return this->_host == "" || this->_port == 0;
}