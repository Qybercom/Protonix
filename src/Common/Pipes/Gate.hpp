#pragma once

#include <Arduino.h>

namespace Qybercom {
	namespace Pipes {
		class Gate {
			private:
				volatile bool _active;
				volatile unsigned long _timer;
				volatile unsigned long _timeout;

			public:
				Gate (unsigned long timeout = 0) {
					this->_active = false;
					this->_timer = 0;
					this->_timeout = timeout;
				}

				bool Active () {
					return this->_active;
				}

				unsigned long Timer () {
					return this->_timer;
				}

				unsigned long Timeout () {
					return this->_timeout;
				}

				Gate* Timeout (unsigned long timeout) {
					this->_timeout = timeout;

					return this;
				}

				Gate* Reset () {
					this->_active = false;
					this->_timer = 0;

					return this;
				}

				bool Check (bool condition) {
					bool out = false;

					if (condition) {
						if (!this->_active) {
							this->_active = true;
							this->_timer = millis();
						}
						else {
							long diff = millis() - this->_timer;

							out = diff >= this->_timeout;
						}
					}
					else {
						this->_active = false;
						this->_timer = 0;
					}

					return out;
				}
		};
	}
}