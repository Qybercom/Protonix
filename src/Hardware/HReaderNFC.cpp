#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include "MFRC522_I2C_Library.h"

#include "../IProtonixHardware.h"
#include "../Protonix.h"

#include "HReaderNFC.h"

using namespace Qybercom::Protonix;

void Hardware::HReaderNFC::_channel (unsigned short channel) {
	Wire.beginTransmission(0x70);
	Wire.write(1 << channel);
	Wire.endTransmission();
}

Hardware::HReaderNFC::HReaderNFC (unsigned short pinSS, unsigned short pinRST, unsigned int uuidReadDebounce, short dedicatedCore) {
	this->_init = false;
	this->_pinSS = pinSS;
	this->_pinRST = pinRST;
	this->_uuid = "";
	this->_uuidActual = "";
	this->_uuidChanged = false;
	this->_dedicatedCore = dedicatedCore;

	this->_debouncer.CheckInterval(uuidReadDebounce);

	this->_i2c = false;
	this->_i2cReader = nullptr;

	this->_spi = false;
	this->_spiReader = nullptr;
}

Hardware::HReaderNFC* Hardware::HReaderNFC::InitI2C (unsigned short pinSS, unsigned short pinRST, int address, unsigned int uuidReadDebounce, short dedicatedCore) {
	Hardware::HReaderNFC* out = new Hardware::HReaderNFC(pinSS, pinRST, uuidReadDebounce, dedicatedCore);

	out->_i2c = true;
	out->_i2cAddress = address;

	return out;
}

Hardware::HReaderNFC* Hardware::HReaderNFC::InitSPI (unsigned short pinSS, unsigned short pinRST, unsigned int uuidReadDebounce, short dedicatedCore) {
	Hardware::HReaderNFC* out = new Hardware::HReaderNFC(pinSS, pinRST, uuidReadDebounce, dedicatedCore);

	out->_spi = true;

	return out;
}

String Hardware::HReaderNFC::UUID () {
	return this->_uuid;
}

String Hardware::HReaderNFC::UUIDActual () {
	return this->_uuidActual;
}

bool Hardware::HReaderNFC::UUIDChanged () {
	bool out = this->_uuidChanged;
	this->_uuidChanged = false;

	return out;
}

Qybercom::Debouncer<String> &Hardware::HReaderNFC::Debouncer () {
	return this->_debouncer;
}

String Hardware::HReaderNFC::HardwareSummary () {
	return "NFC reader";
}

void Hardware::HReaderNFC::HardwareInitPre (Protonix* device) {
	(void)device;

	this->_capability("value", "uuid:string", "Current' card UUID");
}

bool Hardware::HReaderNFC::HardwareI2C () {
	return this->_i2c;
}

void Hardware::HReaderNFC::HardwareI2CPre (Protonix* device) {
	(void)device;
	if (!this->_i2c) return;

	this->_i2cReader = new MFRC522_I2C(this->_i2cAddress, this->_pinRST);
}

void Hardware::HReaderNFC::HardwareI2CPost (Protonix* device) {
	(void)device;
	if (!this->_i2c) return;

	Hardware::HReaderNFC::_channel(this->_pinSS);
	this->_i2cReader->PCD_Init();

	this->_init = true;
}

bool Hardware::HReaderNFC::HardwareSPI () {
	return this->_spi;
}

void Hardware::HReaderNFC::HardwareSPIPre (Protonix* device) {
	(void)device;
	if (!this->_spi) return;

	this->_spiReader = new MFRC522();
	this->_spiReader->PCD_Init(this->_pinSS, this->_pinRST);

	pinMode(this->_pinSS, OUTPUT);
	digitalWrite(this->_pinSS, HIGH);
}

void Hardware::HReaderNFC::HardwareSPIPost (Protonix* device) {
	(void)device;
	if (!this->_spi) return;

	this->_init = true;
}

void Hardware::HReaderNFC::HardwarePipe (Protonix* device, short core) {
	(void)core;
	if (!this->_init) return;

	bool card = false;
	String value = "";
	int i = 0;

	if (this->_i2c && this->_i2cReader != nullptr) {
		Hardware::HReaderNFC::_channel(this->_pinSS);

		card = this->_i2cReader->PICC_IsNewCardPresent();
		value = "";

		if (card && this->_i2cReader->PICC_ReadCardSerial()) {
			i = 0;

			while (i < this->_i2cReader->uid.size) {
				value += String(this->_i2cReader->uid.uidByte[i], HEX);

				i++;
			}
		}
	}

	if (this->_spi && this->_spiReader != nullptr) {
		this->_spiReader->PCD_Init();

		card = this->_spiReader->PICC_IsNewCardPresent();
		value = "";

		if (card && this->_spiReader->PICC_ReadCardSerial()) {
			i = 0;

			while (i < this->_spiReader->uid.size) {
				value += String(this->_spiReader->uid.uidByte[i], HEX);

				i++;
			}
		}
	}

	this->_uuidActual = value;

	this->_debouncer.Use(value);

	if (this->_debouncer.Pipe()) {
		value = this->_debouncer.Empty() ? "" : String(this->_debouncer.Actual());

		if (this->_uuid != value) {
			String current = this->_uuid;
			this->_uuid = value;
			this->_uuidChanged = true;

			if (this->_allowSignal) {
				device->Signal(this->_id, "uuidChanged")->Value(this->_uuid);

				if (current == "" && value != "")
					device->Signal(this->_id, "tagIn")->Value(value);

				if (current != "" && value == "")
					device->Signal(this->_id, "tagOut");
			}

			this->_capability("uuid:string", String(this->_uuid));
		}

		this->_debouncer.Reset();
	}
}

void Hardware::HReaderNFC::HardwareOnCommand (Protonix* device, String command) {
	(void)device;
	(void)command;
}