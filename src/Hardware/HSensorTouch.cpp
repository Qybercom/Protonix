#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../ProtonixDevice.h"

#include "HSensorTouch.h"

using namespace Qybercom::Protonix;

bool Hardware::HSensorTouch::HardwareSPI () {
	return false;
}

void Hardware::HSensorTouch::HardwareInitPre (ProtonixDevice* device) {
	(void)device;


}

void Hardware::HSensorTouch::HardwareInitPost (ProtonixDevice* device) {
	(void)device;


}

void Hardware::HSensorTouch::HardwarePipe (ProtonixDevice* device, short core) {
	(void)device;


}