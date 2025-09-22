#include <Arduino.h>
/*#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include "MFRC522_I2C_Library.h"*/

#include <QybercomMesonix.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

#include "HMesonix.h"

using namespace Qybercom::Mesonix;
using namespace Qybercom::Protonix;

Hardware::HMesonix::HMesonix (unsigned short pinSS, unsigned short pinRST, unsigned short uuidReadDebounce, short dedicatedCore) {
	this->_reader = new Mesonix(27, 4);
	this->_reader->UUIDReadDebounce(uuidReadDebounce);

	this->_init = false;

	this->_dedicatedCore = dedicatedCore;
}

Mesonix* Hardware::HMesonix::Reader () {
	return this->_reader;
}

bool Hardware::HMesonix::HardwareSPI () {
	return true;
}

void Hardware::HMesonix::HardwareSPIPre (Protonix* device) {
	(void)device;

	this->_reader->InitMFRC();
}

void Hardware::HMesonix::HardwareSPIPost (Protonix* device) {
	(void)device;

	this->_init = true;
}

void Hardware::HMesonix::HardwarePipe (Protonix* device, short core) {
	(void)core;
	if (!this->_init) return;

	String uuid = this->_reader->UUID();

	this->_reader->Pipe();

	if (this->_allowSignal && this->_reader->UUIDChanged()) {
		device->Signal(this->_id, "uuidChanged")->ValueString(this->_reader->UUID());
		// TODO: add cardIn/cardOut signals
	}
}

void Hardware::HMesonix::HardwareOnCommand (Protonix* device, String command) {
	(void)device;
	(void)command;
}