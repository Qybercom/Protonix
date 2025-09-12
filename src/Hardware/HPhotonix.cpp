#include <Arduino.h>

#include <QybercomPhotonix.h>

#include "../IProtonixHardware.h"
#include "../ProtonixDevice.h"

#include "HPhotonix.h"

using namespace Qybercom::Photonix;
using namespace Qybercom::Protonix;

bool Hardware::HPhotonix::HardwareSPI () {
	return true;
}

void Hardware::HPhotonix::HardwareInitPre (ProtonixDevice* device) {
	(void)device;


}

void Hardware::HPhotonix::HardwareInitPost (ProtonixDevice* device) {
	(void)device;


}

void Hardware::HPhotonix::HardwarePipe (ProtonixDevice* device, short core) {
	(void)device;


}

void Hardware::HPhotonix::HardwareCommand (ProtonixDevice* device, String command) {
	(void)device;


}