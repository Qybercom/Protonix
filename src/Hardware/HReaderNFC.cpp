#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include "MFRC522_I2C_Library.h"

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

#include "HReaderNFC.h"

using namespace Qybercom::Protonix;

void Hardware::HReaderNFC::_signal (Protonix* device, String value) {
	if (!this->_config["allowSignal"]) return;

	String current = this->_uuid;

	device->Signal(this->_id, "uuidChanged")->Data(value);

	if (current == "" && value != "")
		device->Signal(this->_id, "tagIn")->Data(value);

	if (current != "" && value == "")
		device->Signal(this->_id, "tagOut");
}

/*void Hardware::HReaderNFC::_channel (unsigned short channel) {
	Wire.beginTransmission(0x70);
	Wire.write(1 << channel);
	Wire.endTransmission();
}*/

Hardware::HReaderNFC::HReaderNFC (String mode, unsigned short pinRST, short dedicatedCore) {
	this->_dedicatedCore = dedicatedCore;

	this->_init = false;
	this->_uuid = "";
	this->_uuidActual = "";
	this->_uuidChanged = false;

	this->_debouncer = new Qybercom::Pipes::Debouncer<String>("");

	this->_i2cReader = nullptr;
	this->_spiReader = nullptr;

	this->_config["mode"] = mode;
	this->_config["pinRST"] = pinRST;
	this->_config["debounce"] = 0;
	this->_config["allowSignal"] = true;
}

Hardware::HReaderNFC* Hardware::HReaderNFC::InitI2C (unsigned short pinRST, int address, short dedicatedCore) {
	Hardware::HReaderNFC* out = new Hardware::HReaderNFC("i2c", pinRST, dedicatedCore);

	out->_config["i2cAddress"] = address;

	return out;
}

Hardware::HReaderNFC* Hardware::HReaderNFC::InitSPI (unsigned short pinSS, unsigned short pinRST, short dedicatedCore) {
	Hardware::HReaderNFC* out = new Hardware::HReaderNFC("spi", pinRST, dedicatedCore);

	out->_config["pinSS"] = pinSS;

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

Qybercom::Pipes::Debouncer<String>* Hardware::HReaderNFC::Debouncer () {
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
	return this->_config["mode"] == "i2c";
}

void Hardware::HReaderNFC::HardwareI2CPre (Protonix* device) {
	(void)device;
	if (!this->HardwareI2C()) return;

	this->_i2cReader = new MFRC522_I2C(
		(int)this->_config["i2cAddress"],
		(int)this->_config["pinRST"]
	);
}

void Hardware::HReaderNFC::HardwareI2CPost (Protonix* device) {
	(void)device;
	if (!this->HardwareI2C()) return;
}

bool Hardware::HReaderNFC::HardwareSPI () {
	return this->_config["mode"] == "spi";
}

void Hardware::HReaderNFC::HardwareSPIPre (Protonix* device) {
	(void)device;
	if (!this->HardwareSPI()) return;

	this->_spiReader = new MFRC522();
	this->_spiReader->PCD_Init(
		(int)this->_config["pinSS"],
		(int)this->_config["pinRST"]
	);

	this->_bridge->BridgePinMode(this->_config["pinSS"], OUTPUT);
	this->_bridge->BridgeDigitalWrite(this->_config["pinSS"], true);
}

void Hardware::HReaderNFC::HardwareSPIPost (Protonix* device) {
	(void)device;
	if (!this->HardwareSPI()) return;

	this->_init = true;
}

void Hardware::HReaderNFC::HardwareInitPost (Protonix* device) {
	if (this->HardwareI2C()) {
		//Hardware::HReaderNFC::_channel(this->_config["pinSS"]);
		this->_i2cReader->PCD_Init();

		this->_init = true;
	}
}

void Hardware::HReaderNFC::HardwarePipe (Protonix* device, short core) {
	(void)core;
	if (!this->_init) return;

	bool card = false;
	String value = "";
	int i = 0;

	unsigned int debounce = this->_config["debounce"];
	this->_debouncer->Threshold(debounce);

	if (this->HardwareI2C() && this->_i2cReader != nullptr) {
		//Hardware::HReaderNFC::_channel(this->_config["pinSS"]);
		this->_i2cReader->PCD_Init();

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

	if (this->HardwareSPI() && this->_spiReader != nullptr) {
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

	value = this->_debouncer->Value(value);
	this->_uuidActual = value;

	if (this->_uuid != value) {
		this->_uuid = value;
		this->_uuidChanged = true;

		this->_signal(device, value);
	}
}

void Hardware::HReaderNFC::HardwareOnReset (Protonix* device) {
	this->_signal(device, this->_uuid);
}

void Hardware::HReaderNFC::HardwareOnCommand (Protonix* device, String command) {
	(void)device;
	(void)command;
}