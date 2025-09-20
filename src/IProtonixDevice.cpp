#include <Arduino.h>

#include "IProtonixDevice.h"
#include "IProtonixHardware.h"

#include "Protonix.h"

#include "Command/index.h"

using namespace Qybercom::Protonix;

void IProtonixDevice::DeviceHandleStdCommand (Protonix* device, IProtonixCommand* command) {
	String name = command->CommandName();

	if (name == "std:action") {
		Command::CStdAction* cmd = (Command::CStdAction*)command;
		String _cmd = cmd->CMD();
		String action = cmd->Name();

		Serial.println("[device] Action '" + _cmd + "':'" + action + "'");

		if (_cmd == "trigger")
			device->ActionTrigger(action);

		if (_cmd == "start")
			device->ActionStart(action);

		if (_cmd == "stop")
			device->ActionStop(action);
	}

	if (name == "std:active") {
		Command::CStdActive* cmd = (Command::CStdActive*)command;

		device->Active(cmd->Active());
	}

	if (name == "std:firmware") {
		Command::CStdFirmware* cmd = (Command::CStdFirmware*)command;

		if (cmd->ActionUpdate()) {
			Serial.println("[device] Firmware update requested");

			device->FirmwareUpdateOTA(cmd->Version());
		}
	}

	if (name == "std:hardware") {
		Command::CStdHardware* cmd = (Command::CStdHardware*)command;
		IProtonixHardware* hardware = device->Hardware(cmd->ID());

		if (hardware != nullptr) {
			Serial.println("[device] Hardware '" + cmd->ID() + "' received command '" + cmd->CMD() + "'");

			hardware->HardwareCommand(device, cmd->CMD());
		}
	}

	if (name == "std:reboot") {
		device->Reboot();
	}

	if (name == "std:registry") {
		Command::CStdRegistry* cmd = (Command::CStdRegistry*)command;

		device->Registry()->SetRaw(cmd->Key(), cmd->Value(), true);
	}

	if (name == "std:sensor") {
		Command::CStdSensor* cmd = (Command::CStdSensor*)command;

		device->SensorSet(cmd->Sensor());
	}
}

bool IProtonixDevice::DeviceHandleCommand (Protonix* device, IProtonixCommand* command) {
	this->DeviceHandleStdCommand(device, command);

	return command->CommandName() == "custom";
}

unsigned int IProtonixDevice::DeviceTick () {
	return 10;
}

void IProtonixDevice::DeviceOnReady (Protonix* device) {
	(void)device;
}

void IProtonixDevice::DeviceOnTick (Protonix* device) {
	(void)device;
}

void IProtonixDevice::DeviceOnAction (Protonix* device, ProtonixAction* action) {
	(void)device;
	(void)action;
}

void IProtonixDevice::DeviceOnLoop (Protonix* device) {
	(void)device;
}

void IProtonixDevice::DeviceOnCommand (Protonix* device, IProtonixCommand* command, IProtonixHardware* hardware) {
	(void)hardware;
	if (!this->DeviceHandleCommand(device, command)) return;

	Command::CCustom* cmd = (Command::CCustom*)command;

	if (cmd != nullptr)
		this->DeviceOnCommandCustom(device, cmd->CMD());
}

void IProtonixDevice::DeviceOnCommandCustom (Protonix* device, String command) {
	(void)device;
}

void IProtonixDevice::DeviceOnSignal (Protonix* device, ProtonixSignal* signal) {
	(void)device;
	(void)signal;
}

#if defined(ESP32)
void IProtonixDevice::DeviceOnDedicatedTask (Protonix* device, unsigned short core) {
	(void)device;
}
#endif