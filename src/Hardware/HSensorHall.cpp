#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../ProtonixDevice.h"

#include "HSensorHall.h"

using namespace Qybercom::Protonix;

bool Hardware::HSensorHall::HardwareSPI () {
	return false;
}

void Hardware::HSensorHall::HardwareInitPre (ProtonixDevice* device) {
	(void)device;


}

void Hardware::HSensorHall::HardwareInitPost (ProtonixDevice* device) {
	(void)device;


}

void Hardware::HSensorHall::HardwarePipe (ProtonixDevice* device, short core) {
	(void)device;


}