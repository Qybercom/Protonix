#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../ProtonixDevice.h"

#include "HSensorTrigger.h"

using namespace Qybercom::Protonix;

bool Hardware::HSensorTrigger::HardwareSPI () {
	return false;
}

void Hardware::HSensorTrigger::HardwareInitPre (ProtonixDevice* device) {
	(void)device;


}

void Hardware::HSensorTrigger::HardwareInitPost (ProtonixDevice* device) {
	(void)device;


}

void Hardware::HSensorTrigger::HardwarePipe (ProtonixDevice* device, short core) {
	(void)device;


}