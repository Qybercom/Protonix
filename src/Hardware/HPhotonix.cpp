#include <Arduino.h>

#include <QybercomPhotonix.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

#include "HPhotonix.h"

using namespace Qybercom::Photonix;
using namespace Qybercom::Protonix;

bool Hardware::HPhotonix::HardwareSPI () {
	return true;
}

void Hardware::HPhotonix::HardwareSPIPre (Protonix* device) {
	(void)device;


}

void Hardware::HPhotonix::HardwareSPIPost (Protonix* device) {
	(void)device;


}

void Hardware::HPhotonix::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;


}

void Hardware::HPhotonix::HardwareOnCommand (Protonix* device, String command) {
	(void)device;
	(void)command;


}