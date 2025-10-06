#include <Arduino.h>

#include "HBridgeDefault.h"

using namespace Qybercom::Protonix;

Hardware::HBridgeDefault::HBridgeDefault () {

}

bool Hardware::HBridgeDefault::BridgePinMode (unsigned int pin, int mode) {
	pinMode(pin, mode);

	return true;
}

bool Hardware::HBridgeDefault::BridgeDigitalRead (unsigned int pin) {
	return (bool)digitalRead(pin);
}

bool Hardware::HBridgeDefault::BridgeDigitalWrite (unsigned int pin, bool value) {
	digitalWrite(pin, value);

	return true;
}

int Hardware::HBridgeDefault::BridgeAnalogRead (unsigned int pin) {
	return (int)analogRead(pin);
}

bool Hardware::HBridgeDefault::BridgeAnalogWrite (unsigned int pin, int value) {
	analogWrite(pin, value);

	return true;
}