#include "Arduino.h"

#include "IProtonixCommand.h"
#include "Protonix.h"

using namespace Qybercom::Protonix;

void IProtonixCommand::_init (String name) {
	this->_name = name;
	this->_buffer = "";
}

long IProtonixCommand::_recognize (String name) {
	long length = this->_name.length();

	String n = name.substring(0, length);
	if (n != this->_name) return -1;

	this->_buffer = name;

	return length + 1;
}

void IProtonixCommand::CommandReset () {
	this->_buffer = "";
}

String IProtonixCommand::CommandName () {
	return this->_name;
}

String IProtonixCommand::CommandBuffer () {
	return this->_buffer;
}