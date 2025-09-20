#include <Arduino.h>

#include "IAxionDTO.h"

using namespace Qybercom::Protonix;

void Profile::Axion::IAxionDTOResponse::AxionDTOResponseStatus (unsigned short status) {
	this->_status = status;
}

unsigned short Profile::Axion::IAxionDTOResponse::AxionDTOResponseStatus () {
	return this->_status;
}