#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../ProtonixDevice.h"

#include "HSensorMicrophone.h"

using namespace Qybercom::Protonix;

bool Hardware::HSensorMicrophone::HardwareSPI () {
	return false;
}

void Hardware::HSensorMicrophone::HardwareInitPre (ProtonixDevice* device) {
	(void)device;


}

void Hardware::HSensorMicrophone::HardwareInitPost (ProtonixDevice* device) {
	(void)device;


}

void Hardware::HSensorMicrophone::HardwarePipe (ProtonixDevice* device, short core) {
	(void)device;


}