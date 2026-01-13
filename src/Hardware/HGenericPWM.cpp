#include <Arduino.h>

#include "HGenericPWM.h"

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

#include "HGenericPWM.h"

using namespace Qybercom::Protonix;

short Hardware::HGenericPWM::_allocate () {
	unsigned short i = 0;

	while (i < 16) {
		if (!Hardware::HGenericPWM::_channels[i]) {
			Hardware::HGenericPWM::_channels[i] = true;
			return i;
		}

		i++;
	}

	return -1;
}

Hardware::HGenericPWM::HGenericPWM (unsigned short pin, bool inverted, unsigned int frequency, unsigned resolution) {
	this->_pin = pin;
	this->_inverted = inverted;
	this->_frequency = frequency;
	this->_resolution = resolution;

	this->_max = (1 << this->_resolution) - 1;
	this->_started = false;
	this->_channel = -1;

	if (!Hardware::HGenericPWM::_init) {
		Hardware::HGenericPWM::_init = true;

		unsigned short i =0;
		while (i < 16) {
			Hardware::HGenericPWM::_channels[i] = false;

			i++;
		}
	}
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
		//if (this->_started)
			analogWriteFreq(this->_frequency);
		#elif defined(ESP32)
		//if (this->_started && this->_channel >= 0)
			//ledcSetup(this->_channel, this->_frequency, this->_resolution);
			ledcAttach(this->_pin, this->_frequency, this->_resolution);
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
		//if (this->_started)
			analogWriteRange(this->_max);
		#elif defined(ESP32)
		//if (this->_started && this->_channel >= 0)
			//ledcSetup(this->_channel, this->_frequency, this->_resolution);
			ledcAttach(this->_pin, this->_frequency, this->_resolution);
		#endif
	}

	return this;
}

unsigned short Hardware::HGenericPWM::Resolution () {
	return this->_resolution;
}

bool Hardware::HGenericPWM::Start () {
	#if defined(__AVR__)
		pinMode(this->_pin, OUTPUT);

		// can be improved - fast PWM for desired pin
		analogWrite(this->_pin, 0);

		this->_started = true;

		return true;
	#elif defined(ESP8266)
		pinMode(this->_pin, OUTPUT);

		analogWriteFreq(this->_frequency);
		analogWriteRange(this->_max);
		analogWrite(this->_pin, this->_inverted ? this->_max : 0);

		this->_started = true;

		return true;
	#elif defined(ESP32)
		/*this->_channel = Hardware::HGenericPWM::_allocate();
		if (this->_channel < 0) return false;

		ledcSetup(this->_channel, this->_frequency, this->_resolution);
		ledcAttachPin(this->_pin, this->_channel);
		ledcWrite(this->_channel, this->_inverted ? this->_max : 0);*/
		ledcAttach(this->_pin, this->_frequency, this->_resolution);
		ledcWrite(this->_pin, this->_inverted ? this->_max : 0);

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
	analogWrite(this->_pin, value);
	#elif defined(ESP8266)
	analogWrite(this->_pin, value);
	#elif defined(ESP32)
	//ledcWrite(this->_channel, value);
	ledcWrite(this->_pin, value);
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