#pragma once

#include <Arduino.h>

#include "List.hpp"

namespace Qybercom {
	template<typename T>
	class Debouncer {
		private:
			struct Value {
				volatile T Data;
				volatile unsigned long Count;

				Value (const T &data) : Data(data), Count(0) {}
			};

			volatile unsigned long _checkLast;
			volatile unsigned int _checkInterval;
			List<Value> _values;

		public:
			Debouncer (unsigned int checkInterval = 0) {
				this->_checkLast = 0;
				this->_checkInterval = checkInterval;
			}

			unsigned long CheckLast () {
				return this->_checkLast;
			}

			unsigned int CheckInterval () {
				return this->_checkInterval;
			}

			Debouncer<T> &CheckInterval (unsigned int checkInterval) {
				this->_checkInterval = checkInterval;

				return *this;
			}

			Debouncer<T> &Use (T value) {
				for (Value &val : this->_values) {
					if (val.Data != value) continue;

					val.Count++;

					return *this;
				}

				Value val{value};
				val.Count = 1;

				this->_values.Add(val);

				return *this;
			}

			bool Pipe () {
				unsigned long now = micros();
				bool out = (now - this->_checkLast) > this->_checkInterval;

				if (out)
					this->_checkLast = now;

				return out;
			}

			T Actual () {
				Value out = this->_values.First();

				for (Value &val : this->_values)
					if (val.Count > out.Count)
						out = val;

				return out.Data;
			}

			bool Empty () {
				return this->_values.Count() == 0;
			}

			Debouncer<T> &Reset () {
				//this->_values.Clear();
				for (Value &val : this->_values)
					val.Count = 0;

				return *this;
			}

			List<Value> &Values () {
				return this->_values;
			}
	};
}