#include <Arduino.h>

#include "../Common/index.h"

#include "../Protonix.h"

#include "HKeypad.h"

using namespace Qybercom::Protonix;

Hardware::HKeypad::Key::Key (String label, unsigned short row, unsigned short column) {
	this->Label = label;
	this->Row = row;
	this->Column = column;
	this->Active = false;
}

void Hardware::HKeypad::_signal (Protonix* device, String key, bool active) {
	if (!this->_config["allowSignal"]) return;

	if (active) device->Signal(this->_id, "pressed")->Data(key);
	else device->Signal(this->_id, "released")->Data(key);
}

Hardware::HKeypad::HKeypad () {
	this->_init = false;

	this->_config.Listener(this);
	this->_config["allowSignal"] = true;
}

List<Hardware::HKeypad::Key*> &Hardware::HKeypad::keys () {
	return this->_keys;
}

String Hardware::HKeypad::HardwareSummary () {
	return "Keypad";
}

void Hardware::HKeypad::HardwareInitPost (Protonix* device) {
	(void)device;

	for (unsigned short row : this->_rows) {
		this->_bridge->BridgePinInitOutput(row);
		this->_bridge->BridgeDigitalWrite(row, false);
	}

	for (unsigned short column : this->_columns) {
		this->_bridge->BridgePinInitInputUp(column);
		this->_bridge->BridgeDigitalWrite(column, true);
	}

	//this->_capability("value", "button:string", "");
	this->_init = true;
}

void Hardware::HKeypad::HardwarePipe (Protonix* device, short core) {
	(void)device;
	(void)core;

	unsigned short i = 0;
	unsigned short j = 0;

	for (unsigned short row : this->_rows) {
		for (unsigned short r : this->_rows)
			this->_bridge->BridgeDigitalWrite(r, true);

		this->_bridge->BridgeDigitalWrite(row, false);

		j = 0;
		for (unsigned short column : this->_columns) {
			bool value = this->_bridge->BridgeDigitalRead(column);

			//this->_log("r:" + String(row) + " c:" + String(column) + " v:" + String(value));

			for (Key* key : this->_keys) {
				if (key->Row != i) continue;
				if (key->Column != j) continue;

				//if (value)
					//Serial.println("Key: " + key->Label);

				if (key->Active != value)
					this->_signal(device, key->Label, value);

				key->Active = value;
			}

			j++;
		}

		i++;
	}
}

void Hardware::HKeypad::HardwarePipeInterrupt (Protonix* device) {
	(void)device;
}

void Hardware::HKeypad::HardwareOnReset (Protonix* device) {
	(void)device;
}

void Hardware::HKeypad::HardwareOnCommand (Protonix* device, ProtonixCommand &command) {
	(void)device;
	(void)command;

	// TODO: implement sending commands
}

void Hardware::HKeypad::ValueListenerSet (Value &value) {
	String key = value.Key();
	String val = value;

	if (key == "columns") {
		this->_columns.Clear();

		Qybercom::List<String> columns = explode(" ", val);

		for (String &column : columns)
			this->_columns.Add(column.toInt());
	}

	if (key == "rows") {
		this->_rows.Clear();

		Qybercom::List<String> rows = explode(" ", val);

		for (String &row : rows)
			this->_rows.Add(row.toInt());
	}

	if (key.startsWith("key")) {
		Qybercom::List<String> coordinates = explode(" ", val);

		if (coordinates.Count() == 2)
			this->_keys.Add(new Hardware::HKeypad::Key(
				key.substring(3),
				coordinates[0].toInt(),
				coordinates[1].toInt()
			));
	}
}

Hardware::HKeypad* Hardware::HKeypad::C4R1 () {
	Hardware::HKeypad* out = new Hardware::HKeypad();

	out->_config["rows"] = "0 1 2 3";
	out->_config["columns"] = "4";
	out->_config["key1"] = "0 0";
	out->_config["key2"] = "0 1";
	out->_config["key3"] = "0 2";
	out->_config["key4"] = "0 3";

	return out;
}

Hardware::HKeypad* Hardware::HKeypad::C3R4 () {
	Hardware::HKeypad* out = new Hardware::HKeypad();

	out->_config["rows"] = "0 1 2 3";
	out->_config["columns"] = "4 5 6";
	out->_config["key1"] = "0 0";
	out->_config["key2"] = "0 1";
	out->_config["key3"] = "0 2";
	out->_config["key4"] = "1 0";
	out->_config["key5"] = "1 1";
	out->_config["key6"] = "1 2";
	out->_config["key7"] = "2 0";
	out->_config["key8"] = "2 1";
	out->_config["key9"] = "2 2";
	out->_config["key*"] = "3 0";
	out->_config["key0"] = "3 1";
	out->_config["key#"] = "3 2";

	return out;
}

Hardware::HKeypad* Hardware::HKeypad::C4R4 () {
	Hardware::HKeypad* out = new Hardware::HKeypad();

	out->_config["rows"] = "0 1 2 3";
	out->_config["columns"] = "4 5 6 7";
	out->_config["key1"] = "0 0";
	out->_config["key2"] = "0 1";
	out->_config["key3"] = "0 2";
	out->_config["keyA"] = "0 3";
	out->_config["key4"] = "1 0";
	out->_config["key5"] = "1 1";
	out->_config["key6"] = "1 2";
	out->_config["keyB"] = "1 3";
	out->_config["key7"] = "2 0";
	out->_config["key8"] = "2 1";
	out->_config["key9"] = "2 2";
	out->_config["keyC"] = "2 3";
	out->_config["key*"] = "3 0";
	out->_config["key0"] = "3 1";
	out->_config["key#"] = "3 2";
	out->_config["keyD"] = "3 3";

	return out;
}