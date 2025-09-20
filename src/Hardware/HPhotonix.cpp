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

void Hardware::HPhotonix::HardwareInitPre (Protonix* device) {
	(void)device;


}

void Hardware::HPhotonix::HardwareInitPost (Protonix* device) {
	(void)device;


}

void Hardware::HPhotonix::HardwarePipe (Protonix* device, short core) {
	(void)device;


}

void Hardware::HPhotonix::HardwareCommand (Protonix* device, String command) {
	(void)device;


}