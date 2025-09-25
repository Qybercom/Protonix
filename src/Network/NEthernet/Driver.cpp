#include <Arduino.h>

#include <Ethernet.h>

#include "../../IProtonixNetworkDriver.h"
#include "../../IProtonixNetworkClient.h"

#include "Client.h"
#include "Driver.h"

// #define HOST_NAME "MyArdu" // the only way to set hostname...
// https://github.com/arduino-libraries/Ethernet/pull/257/files

using namespace Qybercom::Protonix;

String Network::NEthernet::Driver::_statusRecognize (EthernetLinkStatus code) {
	switch (code) {
		case Unknown: return "UNKNOWN"; break;
		case LinkON: return "LINK_ON"; break;
		case LinkOFF: return "LINK_OFF"; break;
		default: return "__unknown__"; break;
	}
}

Network::NEthernet::Driver::Driver (unsigned short pinSS) {
	if (!this->_init(true, "ethernet", "eth", "", "")) return;

	this->_pinSS = pinSS;
	this->_status = LinkOFF;
}

Network::NEthernet::Driver::Driver (String mac) {
	if (!this->_init(true, "ethernet", "eth", mac, "")) return;

	this->_pinSS = -1;
	this->_status = LinkOFF;
}

Network::NEthernet::Driver::Driver (unsigned short pinSS, String mac) {
	if (!this->_init(true, "ethernet", "eth", mac, "")) return;

	this->_pinSS = pinSS;
	this->_status = LinkOFF;
}

short Network::NEthernet::Driver::PinSS () {
	return this->_pinSS;
}

Network::NEthernet::Driver* Network::NEthernet::Driver::PinSS (unsigned short pinSS) {
	this->_pinSS = pinSS;

	return this;
}

bool Network::NEthernet::Driver::NetworkDriverConnect () {
	if (!this->_ready)
		return this->_log("Only 1 NEthernet driver supported", false);

	this->_log("Connect");
	this->_macParse();

	Ethernet.init(this->_pinSS);

	if (Ethernet.begin(this->_macBuffer) == 0) {
		this->_log("Connection with MAC '" + this->_mac + "' failed");
		// DHCP failed

		if (Ethernet.hardwareStatus() == EthernetNoHardware)
			return this->_log("No ethernet board recognized on pin '" + String(this->_pinSS) + "'", false);

		// try to configure using IP address instead of DHCP:
		//Ethernet.begin(mac, ip, myDns);

		return false;
	}

	return true;
}

bool Network::NEthernet::Driver::NetworkDriverConnected () {
	if (!this->_ready) return false;

	// @important - Ethernet.h is broken - linkStatus() is not returned correctly
	// falling back to IP check...
	EthernetLinkStatus status = Ethernet.linkStatus(); // still need to check...
	status = this->NetworkDriverAddressIP() == "0.0.0.0" ? Unknown : LinkON;

	bool connected = status == LinkON;

	if (status != this->_status)
		this->_log("Connection status: " + this->_statusRecognize(status));

	this->_status = status;

	return connected;
}

bool Network::NEthernet::Driver::NetworkDriverDisconnect () {
	if (!this->_ready) return false;

	return false;
}

String Network::NEthernet::Driver::NetworkDriverAddressMAC () {
	return this->_mac;
}

String Network::NEthernet::Driver::NetworkDriverAddressIP () {
	return this->_ip(Ethernet.localIP());
}

IProtonixNetworkClient* Network::NEthernet::Driver::NetworkDriverAllocateClient () {
	return new Network::NEthernet::Client();
}