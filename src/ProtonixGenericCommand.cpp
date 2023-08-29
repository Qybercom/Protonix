#include <Arduino.h>

#include "ProtonixGenericCommand.h"

using namespace Qybercom::Protonix;



void ProtonixGenericCommand::_init (String name) {
	this->_name = name;
	this->_output = "";
}

String ProtonixGenericCommand::CommandName () {
	return this->_name;
}

String ProtonixGenericCommand::CommandOutput () {
	return this->_output;
}