#include <Arduino.h>

#include "IProtonixDevice.h"
#include "IProtonixHardware.hpp"

#include "Protonix.h"
#include "ProtonixCommand.h"

using namespace Qybercom::Protonix;

void IProtonixDevice::DeviceHandleCommand (Protonix* device, const ProtonixCommand &command) {
	String name = command.Name();
	Value arguments = command.Arguments();

	Serial.println("[device] Command '" + name + "'");command.Dump();

	if (name == "std:action") {
		String cmd = arguments[0];
		String action = arguments[1];

		Serial.println("[device] Action '" + cmd + "':'" + action + "'");

		if (cmd == "trigger")
			device->ActionTrigger(action);

		if (cmd == "start")
			device->ActionStart(action);

		if (cmd == "stop")
			device->ActionStop(action);
	}

	if (name == "std:on") {
		device->Active(true);
	}

	if (name == "std:off") {
		device->Active(false);
	}

	if (name == "std:active") {
		bool active = arguments[0];

		device->Active(active);
	}

	if (name == "std:firmware") {
		Serial.println("[device] Command:5.1");
		String cmd = arguments[0];
		Serial.println("[device] Firmware '" + cmd + "'");

		if (cmd == "update") {
			String version = arguments[1];
			Serial.println("[device] Firmware update requested for version '" + version + "'");

			device->FirmwareUpdateOTA(version);
		}
	}

	if (name == "std:hardware") {
		String id = arguments[0];

		IProtonixHardware* hardware = device->Hardware(id);

		if (hardware != nullptr) {
			Serial.println("[device] Hardware '" + id + "' received command");

			hardware->HardwareOnCommand(device, command);
		}
	}

	if (name == "std:reboot") {
		device->Reboot();
	}

	if (name == "std:registry") {
		String cmd = arguments[0];

		if (cmd == "raw") {
			String raw = arguments[1];

			Serial.println("[device] Registry received command 'raw':'" + raw + "'");

			device->Registry()->Load(raw);
		}

		if (cmd == "set") {
			String key = arguments[1];
			Value value = Value::Deserialize(
				Protonix::Instance()->Format("application/json"),
				arguments[2].ToString()
			);

			Serial.println("[device] Registry received command 'set' for key '" + key + "' with value '" + value.ToString() + "'");

			device->Registry()->Save(key, value);
		}

		if (cmd == "clear") {
			Serial.println("[device] Registry received command 'clear'");

			device->Registry()->Clear();
		}
	}

	if (name == "std:sensor") {
		String id = arguments[0];

		ProtonixSensor* sensor = device->Sensor(id);

		if (sensor != nullptr) {
			Value value = arguments[1];
			Value active = arguments[2];
			Value failure = arguments[3];
			Value state = arguments[4];

			Serial.print("[device] Sensor '" + id + "' set value:'" + value.ToString() + "'");
			device->Sensor(id)->Value(value);

			if (!active.IsUndefined()) {
				Serial.print(", active:" + active.ToString());
				device->Sensor(id)->Active(active);
			}

			if (!failure.IsUndefined()) {
				Serial.print(", failure:" + failure.ToString());
				device->Sensor(id)->Failure(failure);
			}

			if (!state.IsUndefined()) {
				Serial.print(", state:'" + state.ToString() + "'");
				device->Sensor(id)->State(state);
			}

			Serial.println();
		}
	}

	this->DeviceOnCommand(device, command);
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

void IProtonixDevice::DeviceOnCommand (Protonix* device, const ProtonixCommand &command) {
	(void)device;
	(void)command;
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