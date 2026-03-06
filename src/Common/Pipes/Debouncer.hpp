#pragma once

#include <Arduino.h>

namespace Qybercom {
	namespace Pipes {
		template<typename T>
		class Debouncer {
			private:
				volatile T _current;
				volatile T _target;
				volatile unsigned int _targetCount;
				volatile unsigned int _threshold;

			public:
				Debouncer (T initial, unsigned int threshold = 0) {
					((T&)this->_current) = initial;
					((T&)this->_target) = initial;
					this->_targetCount = 0;
					this->_threshold = threshold;
				}

				T Current () {
					return this->_current;
				}

				T Target () {
					return this->_target;
				}

				unsigned int TargetCount () {
					return this->_targetCount;
				}

				unsigned int Threshold () {
					return this->_threshold;
				}

				Debouncer<T>* Threshold (unsigned int threshold) {
					this->_threshold = threshold;

					return this;
				}

				T Value (T value) {
					T out = (T&)this->_current;

					if (value == ((T&)this->_current)) {
						((T&)this->_target) = ((T&)this->_current);
						this->_targetCount = 0;
					}
					else {
						if (value != ((T&)this->_target))
							this->_targetCount = 0;

						((T&)this->_target) = value;
						this->_targetCount = this->_targetCount + 1;

						if (this->_targetCount >= this->_threshold) {
							((T&)this->_current) = value;
							this->_targetCount = 0;
						}
					}

					return out;
				}
		};
	}
}