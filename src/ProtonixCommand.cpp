#include <Arduino.h>

#include "Common/Value.h"

#include "IProtonixHardware.hpp"

#include "ProtonixCommand.h"

using namespace Qybercom::Protonix;

ProtonixCommand::ProtonixCommand () {
	_name = "";
	_arguments = Value();
	_hardware = nullptr;
}

ProtonixCommand::ProtonixCommand (const String &name) {
	_name = name;
	_arguments = Value();
	_hardware = nullptr;
}

ProtonixCommand::ProtonixCommand (const String &name, const Value &arguments) {
	_name = name;
	_arguments = arguments;
	_hardware = nullptr;
}

ProtonixCommand &ProtonixCommand::Hardware (IProtonixHardware* hardware) {
	_hardware = hardware;

	return *this;
}

IProtonixHardware* ProtonixCommand::Hardware () const {
	return _hardware;
}

String ProtonixCommand::Name () const {
	return _name;
}

Qybercom::Value &ProtonixCommand::Arguments () {
	return _arguments;
}

const Qybercom::Value &ProtonixCommand::Arguments () const {
	return _arguments;
}

Qybercom::Value ProtonixCommand::Argument (unsigned int i) const {
	return _arguments[i];
}

String ProtonixCommand::Group () const {
	int pos = _name.lastIndexOf(':');

	return pos == -1 ? "" : _name.substring(0, pos);
}

String ProtonixCommand::Command () const {
	int pos = _name.lastIndexOf(':');

	return pos == -1 ? _name : _name.substring(pos + 1);
}

bool ProtonixCommand::IsStd () const {
	return Group() == "std";
}

bool ProtonixCommand::IsCustom () const {
	return Group() == "custom";
}

void ProtonixCommand::Dump () const {
	Serial.println("[Command " + _name + "]");
	Serial.println(" - IsStd: " + String(IsStd()));
	Serial.println(" - IsCustom: " + String(IsCustom()));
	Serial.print(" - Arguments (" + _arguments.TypeName() + "):"); _arguments.Dump();
}

void ProtonixCommand::ValueTypeDeserialize (const String &raw) {
	_name = "";
	_arguments = Qybercom::Value();

	int length = raw.length();
	if (length == 0) return;

	int i = 0;
	int start = 0;
	bool name = false;

	while (i <= length) {
		bool delimiter = (i == length) || (raw[i] == ';');

		if (delimiter) {
			String part = raw.substring(start, i);

			if (name) _arguments.Add(part);
			else {
				_name = part;
				name = true;
			}

			start = i + 1;
		}

		i++;
	}
}

String ProtonixCommand::ValueTypeSerialize () {
	String out = _name;

	for (Qybercom::Value &argument : _arguments)
		out += ";" + argument.ToString();

	return out;
}