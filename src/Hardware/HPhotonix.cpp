#include <Arduino.h>

#include <QybercomPhotonix.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

#include "HPhotonix.h"

using namespace Qybercom::Photonix;
using namespace Qybercom::Protonix;

Hardware::HPhotonix::HPhotonix (unsigned short pin, unsigned int length) {
	this->_pin = pin;
	this->_length = length;
	this->_photonix = nullptr;
}

Hardware::HPhotonix* Hardware::HPhotonix::Init (unsigned short pin, unsigned int length) {
	return new Hardware::HPhotonix(pin, length);
}

Photonix* Hardware::HPhotonix::Driver () {
	return this->_photonix;
}

String Hardware::HPhotonix::HardwareSummary () {
	return "Photonix";
}

void Hardware::HPhotonix::HardwareInitPost (Protonix* device) {
	(void)device;

	this->_photonix = new Photonix(this->_pin, this->_length);
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

void Hardware::HPhotonix::HardwareOnCommand (Protonix* device, String command) {
	(void)device;

	if (command == "clear")
		this->_photonix->Clear();
}