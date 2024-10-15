#pragma once

#include <Arduino.h>

namespace Qybercom {
	namespace Protonix {
		class IProtonixHardware {
        	protected:
                String _id;

			public:
                enum class Type {
                	SPI
                };

                IProtonixHardware* HardwareID(String id) {
                	this->_id = id;

                    return this;
                }

                String HardwareID() {
                	return this->_id;
                }

				virtual Type HardwareType();

				virtual void HardwareInitPre();
				virtual void HardwareInitPost();

                virtual void HardwarePipe();

            protected:
            	Type _type;
		};
	}
}