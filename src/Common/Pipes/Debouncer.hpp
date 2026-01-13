#pragma once

#include <Arduino.h>

namespace Qybercom {
	template<typename T>
	class Debouncer {
		private:
			T _current;
			T _target;
			unsigned int _targetCount;
			unsigned int _threshold;

		public:
			Debouncer (T initial, unsigned int threshold = 0) {
				this->_current = initial;
				this->_target = initial;
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
				T out = this->_current;

				if (value == this->_current) {
					this->_target = this->_current;
					this->_targetCount = 0;
				}
				else {
					if (value != this->_target)
						this->_targetCount = 0;

					this->_target = value;
					this->_targetCount = this->_targetCount + 1;

					if (this->_targetCount >= this->_threshold) {
						this->_current = value;
						this->_targetCount = 0;
					}
				}

				return out;
			}
	};
}