#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

#include "HBusSerial.h"

#include "HPlayerMP3.h"

using namespace Qybercom::Protonix;

Hardware::HPlayerMP3::HPlayerMP3 (unsigned short pinRX, unsigned short pinTX) {
	this->_player = new Hardware::HBusSerial(pinRX, pinTX);
	this->_player->HardwareConfig().Set("observable", false);
}

// https://geekmatic.in.ua/pdf/Catalex_MP3_board.pdf
bool Hardware::HPlayerMP3::_cmd (char a1, char a2, char a3, char a4) {
	if (this->_player == nullptr) return false;

	//this->_log("CMD: " + String(a1) + ":" + String(a2) + ":" + String(a3) + ":" + String(a4));

	this->_player->Write((char)0x7E);	// Код начала команды
	this->_player->Write((char)0xFF);	// Все модули
	this->_player->Write((char)0x06);	// Команда воспроизведения файла
	this->_player->Write(a1);			// Количество байт данных
	this->_player->Write(a2);			// Индекс папки (0x00 - корневая папка)
	this->_player->Write(a3);			// Номера файла "в папке" либо 00 "на диске"
	this->_player->Write(a4);			// НОМЕР ФАЙЛА В ПАПКЕ MP3
	this->_player->Write((char)0xEF);	// Код конца команды

	return true;
}

Hardware::HBusSerial* Hardware::HPlayerMP3::Port () {
	return this->_player;
}

bool Hardware::HPlayerMP3::Play (int file, int dir) {
	return this->_cmd(
		dir == 0x00 ? (char)0x03 : (char)0x0F,	// play by index in folder (0x0F) or on disk (0x03)
		(char)0x00,
		(char)dir,								// folder index in folder (0x0F) or on disk (0x03)
		(char)file								// file index
	);
}

bool Hardware::HPlayerMP3::Pause () {
	return this->_cmd((char)0x0E, (char)0x00, (char)0x00, (char)0x00);
}

bool Hardware::HPlayerMP3::Stop () {
	return this->_cmd((char)0x16, (char)0x00, (char)0x00, (char)0x00);
}

bool Hardware::HPlayerMP3::VolumeUp () {
	return this->_cmd((char)0x04, (char)0x00, (char)0x00, (char)0x00);
}

bool Hardware::HPlayerMP3::VolumeDown () {
	return this->_cmd((char)0x05, (char)0x00, (char)0x00, (char)0x00);
}

bool Hardware::HPlayerMP3::Volume (short value) {
	char volume = (char)((value * 30) / 100);

	return this->_cmd((char)0x06, (char)0x00, (char)0x00, volume);
}

bool Hardware::HPlayerMP3::Reset () {
	return this->_cmd((char)0x0C, (char)0x00, (char)0x00, (char)0x00);
}

bool Hardware::HPlayerMP3::Wake () {
	return this->_cmd((char)0x0B, (char)0x00, (char)0x00, (char)0x00);
}

String Hardware::HPlayerMP3::HardwareSummary () {
	return "MP3 player";
}

void Hardware::HPlayerMP3::HardwareInitPre (Protonix* device) {
	this->_player->HardwareInitPre(device);

	this->Wake();

	this->_capability("command", "play:<file>:<dir>", "Play file in dir");
	this->_capability("command", "pause", "Pause");
	this->_capability("command", "stop", "Stop");
	this->_capability("command", "volumeUp", "Volume up");
	this->_capability("command", "volumeDown", "Volume down");
	this->_capability("command", "volume:<percent>", "Set volume in percents");
	this->_capability("command", "reset", "Reset");
	this->_capability("command", "wake", "Wake");
}

void Hardware::HPlayerMP3::HardwarePipe (Protonix* device, short core) {
	this->_player->HardwarePipe(device, core);
}

void Hardware::HPlayerMP3::HardwareOnReset (Protonix* device) {
	(void)device;

	this->Stop();
}

void Hardware::HPlayerMP3::HardwareOnCommand (Protonix* device, ProtonixCommand &command) {
	this->_player->HardwareOnCommand(device, command);
}