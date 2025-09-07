#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../ProtonixDevice.h"

#include "HSensorPhotoResistor.h"

using namespace Qybercom::Protonix;

bool Hardware::HSensorPhotoResistor::HardwareSPI () {
	return false;
}

void Hardware::HSensorPhotoResistor::HardwareInitPre (ProtonixDevice* device) {
	(void)device;


}

void Hardware::HSensorPhotoResistor::HardwareInitPost (ProtonixDevice* device) {
	(void)device;


}

void Hardware::HSensorPhotoResistor::HardwarePipe (ProtonixDevice* device, short core) {
	(void)device;


}