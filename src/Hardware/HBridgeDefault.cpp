#include <Arduino.h>

#include "HBridgeDefault.h"

using namespace Qybercom::Protonix;

Hardware::HBridgeDefault::HBridgeDefault () {

}

bool Hardware::HBridgeDefault::BridgePinDualPolarity (unsigned int pin) {
	(void)pin; // TODO: add feature for per-pin check

	return true;
}

bool Hardware::HBridgeDefault::BridgePinInitInput (unsigned int pin, int initial) {
	return true
		&& this->BridgeDigitalWrite(pin, initial)
		&& this->BridgePinMode(pin, INPUT);
}

bool Hardware::HBridgeDefault::BridgePinInitInputUp (unsigned int pin, int initial) {
	return true
		&& this->BridgeDigitalWrite(pin, initial)
		&& this->BridgePinMode(pin, INPUT_PULLUP);
}

bool Hardware::HBridgeDefault::BridgePinInitInputDown (unsigned int pin, int initial) {
	#if defined(ESP8266) || !defined(INPUT_PULLDOWN)
	return false;
	#else
	return true
		&& this->BridgeDigitalWrite(pin, initial)
		&& this->BridgePinMode(pin, INPUT_PULLDOWN);
	#endif
}

bool Hardware::HBridgeDefault::BridgePinInitOutput (unsigned int pin) {
	return this->BridgePinMode(pin, OUTPUT);
}

bool Hardware::HBridgeDefault::BridgePinMode (unsigned int pin, int mode) {
	pinMode(pin, mode);

	return true;
}

bool Hardware::HBridgeDefault::BridgePinAvailable (unsigned int pin) {
	(void)pin;

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