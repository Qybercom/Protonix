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

			public:
				ProtonixDeviceSensor();
				ProtonixDeviceSensor(String id);
				ProtonixDeviceSensor(String id, String value);
				ProtonixDeviceSensor(String id, String value, bool active);
				ProtonixDeviceSensor(String id, String value, bool active, bool failure);
				ProtonixDeviceSensor(String id, bool active);
				ProtonixDeviceSensor(String id, bool active, bool failure);

				void ID(String id);
				String ID();

				void Value(String value);
				String Value();

				void Active(bool active);
				bool Active();

				void Failure(bool failure);
				bool Failure();

				String Summary();
				void Reset();
		};
	}
}