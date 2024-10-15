#include <Arduino.h>
// Standard SPI library comes bundled with the Arduino IDE
#include <SPI.h>
// Download and install from https://github.com/miguelbalboa/rfid
#include <MFRC522.h>

#include "../IProtonixHardware.h"

#include "HReaderMFRC.h"

using namespace Qybercom::Protonix;

Hardware::HReaderMFRC::HReaderMFRC(int pinSS, int pinRST) {
	this->PinSS(pinSS);
    this->PinRST(pinRST);
}

Hardware::HReaderMFRC* Hardware::HReaderMFRC::PinSS(int pinSS) {
	this->_pinSS = pinSS;

    return this;
}

int Hardware::HReaderMFRC::PinSS() {
	return this->_pinSS;
}

Hardware::HReaderMFRC* Hardware::HReaderMFRC::PinRST(int pinRST) {
	this->_pinRST = pinRST;

    return this;
}

int Hardware::HReaderMFRC::PinRST() {
	return this->_pinRST;
}

byte Hardware::HReaderMFRC::AntennaGain() {
	return this->_reader.PCD_GetAntennaGain();
}

String Hardware::HReaderMFRC::Version() {
	byte v = this->_reader.PCD_ReadRegister(MFRC522::PCD_Register::VersionReg);

    if (v == 0x88) return "(clone)";
    if (v == 0x90) return "v0.0";
    if (v == 0x91) return "v1.0";
    if (v == 0x92) return "v2.0";
    if (v == 0x12) return "counterfeit chip";
    if ((v == 0x00) || (v == 0xFF)) return "(err)";

    return "(unknown)";
}

String Hardware::HReaderMFRC::CardSerial() {
	return this->_cardSerial;
}

IProtonixHardware::Type Hardware::HReaderMFRC::HardwareType() {
	return IProtonixHardware::Type::SPI;
}

void Hardware::HReaderMFRC::HardwareInitPre() {
	pinMode(this->_pinSS, OUTPUT);
	digitalWrite(this->_pinSS, HIGH);
}

void Hardware::HReaderMFRC::HardwareInitPost() {
	this->_reader.PCD_Init(this->_pinSS, this->_pinRST);
}

void Hardware::HReaderMFRC::HardwarePipe() {
	this->_reader.PCD_Init();
	this->_cardSerial = "";

	if (this->_reader.PICC_IsNewCardPresent() && this->_reader.PICC_ReadCardSerial()) {
		int i = 0;

		while (i < this->_reader.uid.size) {
			this->_cardSerial += String(this->_reader.uid.uidByte[i], HEX);

			i++;
		}
	}

	// Halt PICC
	this->_reader.PICC_HaltA();
	// Stop encryption on PCD
	this->_reader.PCD_StopCrypto1();
}