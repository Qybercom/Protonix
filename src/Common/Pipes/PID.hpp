#pragma once

#include <Arduino.h>

namespace Qybercom {
	class PID {
		private:
			float _kp;
			float _ki;
			float _kd;
			float _min;
			float _max;
			float _error;
			float _integral;
			unsigned long _last;

		public:
			PID (float kp = 0.0, float ki = 0.0, float kd = 0.0, float min = 0.0, float max = 0.0) {
				this->_kp = kp;
				this->_ki = ki;
				this->_kd = kd;
				this->_min = min;
				this->_max = max;

				this->_error = 0;
				this->_integral = 0;
				this->_last = 0;
			}

			PID* KP (float kp) {
				this->_kp = kp;

				return this;
			}

			float KP () {
				return this->_kp;
			}

			PID* KI (float ki) {
				this->_ki = ki;

				return this;
			}

			float KI () {
				return this->_ki;
			}

			PID* KD (float kd) {
				this->_kd = kd;

				return this;
			}

			float KD () {
				return this->_kd;
			}

			PID* Min (float min) {
				this->_min = min;

				return this;
			}

			float Min () {
				return this->_min;
			}

			PID* Max (float max) {
				this->_max = max;

				return this;
			}

			float Max () {
				return this->_max;
			}

			float Compute (float input, float target) {
				unsigned long now = millis();

				if (this->_last == 0)
					this->_last = now;

				float dt = (now - this->_last) / 1000.0;

				this->_last = now;

				return this->Compute(input, target, dt);
			}

			float Compute (float input, float target, float dt) {
				float error = target - input;

				float P = this->_kp * error;
				float D = this->_kd * (error - this->_error) / dt;
				float I = this->_ki * this->_integral;

				float output = P + I + D;
				bool i = (output < this->_max && output > this->_min)
					|| (output >= this->_max && error < 0)
					|| (output <= this->_min && error > 0);

				if (i)
					this->_integral += error * dt;

				this->_error = error;

				if (output > this->_max) output = this->_max;
				else if (output < this->_min) output = this->_min;

				return output;
			}
	};
}