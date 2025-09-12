#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../ProtonixDevice.h"

#include "HPlayerMP3.h"

using namespace Qybercom::Protonix;

Hardware::HPlayerMP3::HPlayerMP3 (String port) {
	this->_port = port;
}

// https://geekmatic.in.ua/pdf/Catalex_MP3_board.pdf
void Hardware::HPlayerMP3::_cmd (byte a1, byte a2, byte a3, byte a4) {
	if (this->_player == nullptr) return;
	//Serial.println("[hardware:playerMP3] CMD: " + String(a1) + ":" + String(a2) + ":" + String(a3) + ":" + String(a4));

	this->_player->Write((byte)0x7E);	// Код начала команды
	this->_player->Write((byte)0xFF);	// Все модули
	this->_player->Write((byte)0x06);	// Команда воспроизведения файла
	this->_player->Write(a1);			// Количество байт данных
	this->_player->Write(a2);			// Индекс папки (0x00 - корневая папка)
	this->_player->Write(a3);			// Номера файла "в папке" либо 00 "на диске"
	this->_player->Write(a4);			// НОМЕР ФАЙЛА В ПАПКЕ MP3
	this->_player->Write((byte)0xEF);	// Код конца команды
}

void Hardware::HPlayerMP3::Play (int file, int dir) {
	this->_cmd(
		dir == 0x00 ? (byte)0x03 : (byte)0x0F,	// play by index in folder (0x0F) or on disk (0x03)
		(byte)0x00,
		(byte)dir,								// folder index in folder (0x0F) or on disk (0x03)
		(byte)file								// file index
	);
}

void Hardware::HPlayerMP3::Pause () {
	this->_cmd((byte)0x0E, (byte)0x00, (byte)0x00, (byte)0x00);
}

void Hardware::HPlayerMP3::Stop () {
	this->_cmd((byte)0x16, (byte)0x00, (byte)0x00, (byte)0x00);
}

void Hardware::HPlayerMP3::Reset () {
	this->_cmd((byte)0x0C, (byte)0x00, (byte)0x00, (byte)0x00);
}

void Hardware::HPlayerMP3::Wake () {
	this->_cmd((byte)0x0B, (byte)0x00, (byte)0x00, (byte)0x00);
}

bool Hardware::HPlayerMP3::HardwareSPI () {
	return false;
}

void Hardware::HPlayerMP3::HardwareInitPre (ProtonixDevice* device) {
	(void)device;

	this->_player = device->Port(this->_port);

	if (this->_player == nullptr) {
		Serial.println("[hardware:playerMP3] InitPre on port '" + this->_port + "' - no port configured");

		return;
	}

	this->_player->Observable(false);

	this->Wake();

	Serial.println("[hardware:playerMP3] InitPre on port '" + this->_port + "'");
}

void Hardware::HPlayerMP3::HardwareInitPost (ProtonixDevice* device) {
	(void)device;


}

void Hardware::HPlayerMP3::HardwarePipe (ProtonixDevice* device, short core) {
	(void)device;


}

void Hardware::HPlayerMP3::HardwareCommand (ProtonixDevice* device, String command) {
	(void)device;

	
}