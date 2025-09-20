#include <Arduino.h>

#include <QybercomMesonix.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

#include "HMesonix.h"

using namespace Qybercom::Mesonix;
using namespace Qybercom::Protonix;

Hardware::HMesonix::HMesonix (unsigned short pinSS, unsigned short pinRST, unsigned short uuidReadDebounce, short dedicatedCore) {
	this->_reader = new Mesonix(27, 4);
	this->_reader->UUIDReadDebounce(uuidReadDebounce);

	this->_dedicatedCore = dedicatedCore;
}

Mesonix* Hardware::HMesonix::Reader () {
	return this->_reader;
}

bool Hardware::HMesonix::HardwareSPI () {
	return true;
}

void Hardware::HMesonix::HardwareInitPre (Protonix* device) {
	(void)device;

	this->_reader->InitMFRC();
}

void Hardware::HMesonix::HardwareInitPost (Protonix* device) {
	(void)device;
}

void Hardware::HMesonix::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;

	//Serial.println("[hardware:mesonix] Pipe on core " + String(core));

	this->_reader->Pipe();
}

void Hardware::HMesonix::HardwareCommand (Protonix* device, String command) {
	(void)device;
	(void)command;
}