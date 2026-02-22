#include <Arduino.h>

#include <QybercomPhotonix.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

#include "HPhotonix.h"

using namespace Qybercom::Photonix;
using namespace Qybercom::Protonix;

Hardware::HPhotonix::HPhotonix (unsigned short pin, unsigned int length) {
	this->_config["pin"] = pin;
	this->_config["length"] = length;

	this->_photonix = new Photonix(this->_config["pin"], this->_config["length"]);
}

Photonix* Hardware::HPhotonix::Driver () {
	return this->_photonix;
}

String Hardware::HPhotonix::HardwareSummary () {
	return "Photonix";
}

void Hardware::HPhotonix::HardwareInitPost (Protonix* device) {
	(void)device;

	this->_photonix->Initialized();
}

void Hardware::HPhotonix::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;


}

void Hardware::HPhotonix::HardwareOnReset (Protonix* device) {
	(void)device;

	this->_photonix->Clear();
}

void Hardware::HPhotonix::HardwareOnCommand (Protonix* device, ProtonixCommand &command) {
	(void)device;
	String cmd = command.Argument(1);

	if (cmd == "clear")
		this->_photonix->Clear();
}