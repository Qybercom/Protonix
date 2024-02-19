#pragma once

#include <Arduino.h>

namespace Qybercom {
	namespace Protonix {
		class ProtonixDeviceSensor {
			private:
				String _id;
				String _value;
				bool _active;
				bool _failure;
				String _state;

			public:
				ProtonixDeviceSensor();
				ProtonixDeviceSensor(String id);
				ProtonixDeviceSensor(String id, String value);
				ProtonixDeviceSensor(String id, String value, bool active);
				ProtonixDeviceSensor(String id, String value, bool active, bool failure);
				ProtonixDeviceSensor(String id, String value, bool active, bool failure, String state);
				ProtonixDeviceSensor(String id, bool active);
				ProtonixDeviceSensor(String id, bool active, bool failure);
				ProtonixDeviceSensor(String id, bool active, bool failure, String state);
				ProtonixDeviceSensor(String id, String value, String state);

				void ID(String id);
				String ID();

				void Value(String value);
				String Value();

				void Active(bool active);
				bool Active();

				void Failure(bool failure);
				bool Failure();

				void State(String state);
				String State();

				String Summary();
				void Reset();
		};
	}
}