#include <Arduino.h>

#include "HGenericPWM.h"

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

#include "HGenericPWM.h"

using namespace Qybercom::Protonix;

Hardware::HGenericPWM::HGenericPWM (unsigned short pin, bool inverted, unsigned int frequency, unsigned resolution) {
	this->_config["pin"] = pin;
	this->_inverted = inverted;
	this->_frequency = frequency;
	this->_resolution = resolution;

	this->_max = (1 << this->_resolution) - 1;
	this->_started = false;
}

Hardware::HGenericPWM* Hardware::HGenericPWM::Max (unsigned int max) {
	this->_max = max;

	return this;
}

unsigned int Hardware::HGenericPWM::Max () {
	return this->_max;
}

Hardware::HGenericPWM* Hardware::HGenericPWM::Frequency (unsigned int frequency) {
	this->_frequency = frequency;

	if (this->_started) {
		#if defined(__AVR__)

		#elif defined(ESP8266)
			this->_bridge->BridgeAnalogWriteFreq(this->_frequency);
		#elif defined(ESP32)
			ledcAttach((int)this->_config["pin"], this->_frequency, this->_resolution);
		#endif
	}

	return this;
}

unsigned int Hardware::HGenericPWM::Frequency () {
	return this->_frequency;
}

Hardware::HGenericPWM* Hardware::HGenericPWM::Resolution (unsigned short resolution) {
	if (resolution < 1) resolution = 1;
	if (resolution > 16) resolution = 16;

	this->_resolution = resolution;
	this->_max = (1UL << this->_resolution) - 1;

	if (this->_started) {
		#if defined(__AVR__)

		#elif defined(ESP8266)
			analogWriteRange(this->_max);
		#elif defined(ESP32)
			ledcAttach((int)this->_config["pin"], this->_frequency, this->_resolution);
		#endif
	}

	return this;
}

unsigned short Hardware::HGenericPWM::Resolution () {
	return this->_resolution;
}

bool Hardware::HGenericPWM::Start () {
	#if defined(__AVR__)
		this->_bridge->BridgePinMode(this->_config["pin"], OUTPUT);

		// can be improved - fast PWM for desired pin
		this->_bridge->BridgeAnalogWrite(this->_config["pin"], 0);

		this->_started = true;

		return true;
	#elif defined(ESP8266)
		this->_bridge->BridgePinMode(this->_config["pin"], OUTPUT);

		analogWriteFreq(this->_frequency);
		analogWriteRange(this->_max);
		this->_bridge->BridgeAnalogWrite(this->_config["pin"], this->_inverted ? this->_max : 0);

		this->_started = true;

		return true;
	#elif defined(ESP32)
		ledcAttach((int)this->_config["pin"], this->_frequency, this->_resolution);
		ledcWrite((int)this->_config["pin"], this->_inverted ? this->_max : 0);

		this->_started = true;

		return true;
	#else
		return false;
	#endif
}

bool Hardware::HGenericPWM::Write (unsigned int value) {
	if (!this->_started) return false;

	if (value > this->_max) value = this->_max;
	if (this->_inverted) value = this->_max - value;

	#if defined(__AVR__)
	this->_bridge->BridgeAnalogWrite(this->_config["pin"], value);
	#elif defined(ESP8266)
	this->_bridge->BridgeAnalogWrite(this->_config["pin"], value);
	#elif defined(ESP32)
	ledcWrite((int)this->_config["pin"], value);
	#endif

	return true;
}

bool Hardware::HGenericPWM::WritePercent (unsigned short percent) {
	if (percent > 100)
		percent = 100;

	return this->Write((percent * this->_max) / 100);
}

bool Hardware::HGenericPWM::Stop () {
	return this->Write(0);
}