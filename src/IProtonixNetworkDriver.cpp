#include "Arduino.h"

#include "IProtonixNetworkDriver.h"
#include "Protonix.h"

using namespace Qybercom::Protonix;

unsigned short IProtonixNetworkDriver::_id = 0;

bool IProtonixNetworkDriver::_init (bool single, String kind, String name, String mac, String hostname, bool connectAuto, unsigned int timerConnect) {
	this->_ready = false;

	this->_single = single;
	this->_kind = kind;
	this->_name = name + String(_id);
	this->_mac = mac;
	this->_hostname = hostname;
	this->_connected = false;
	this->_connectAttempt = false;
	this->_connectAuto = connectAuto;

	if (!single || _id == 0) {
		this->_ready = true;

		this->_timerConnect = new ProtonixTimer(timerConnect);
		this->_timerConnect->Enabled(true);
	}

	_id = _id + 1;

	return true;
}

void IProtonixNetworkDriver::_macParse () {
	ParseMAC(this->_mac, this->_macBuffer);
}

bool IProtonixNetworkDriver::_log (String message, bool ret) {
	Serial.println("[network:driver:" + this->_kind + "] " + message);

	return ret;
}

String IProtonixNetworkDriver::NetworkDriverName () {
	return this->_name;
}

bool IProtonixNetworkDriver::NetworkDriverConnectAuto () {
	return this->_connectAuto;
}

IProtonixNetworkDriver* IProtonixNetworkDriver::NetworkDriverConnectAuto (bool enabled) {
	this->_connectAuto = enabled;

	return this;
}

ProtonixTimer* IProtonixNetworkDriver::TimerConnect () {
	return this->_timerConnect;
}

bool IProtonixNetworkDriver::NetworkDriverPipe (Protonix* device) {
	if (this->_timerConnect == nullptr) return false;

	if (this->_timerConnect->Pipe()) {
		this->_connected = this->NetworkDriverConnected();

		if (this->_connected)
			this->_connectAttempt = false;

		if (!this->_connected && this->_connectAuto && !this->_connectAttempt) {
			this->_connectAttempt = true;

			this->NetworkDriverConnect();
		}
	}

	return this->_connected;
}

// https://stackoverflow.com/a/12772708/2097055
// https://stackoverflow.com/a/1755042/2097055
void IProtonixNetworkDriver::ParseMAC (String mac, uint8_t out[6]) {
	char buffer[18];
	mac.toCharArray(buffer, 18);

	sscanf(buffer, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &out[0], &out[1], &out[2], &out[3], &out[4], &out[5]);
}