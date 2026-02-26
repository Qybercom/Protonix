#pragma once

#include <Arduino.h>

#include "Common/index.h"

namespace Qybercom {
	namespace Protonix {
		class ProtonixSensor {
			private:
				String _id;
				String _value;
				bool _active;
				bool _failure;
				String _state;

			public:
				ProtonixSensor ();
				ProtonixSensor (String id);
				ProtonixSensor (String id, String value);
				ProtonixSensor (String id, String value, bool active);
				ProtonixSensor (String id, String value, bool active, bool failure);
				ProtonixSensor (String id, String value, bool active, bool failure, String state);
				ProtonixSensor (String id, bool active);
				ProtonixSensor (String id, bool active, bool failure);
				ProtonixSensor (String id, bool active, bool failure, String state);
				ProtonixSensor (String id, String value, String state);

				void ID (String id);
				String ID ();

				void Value (String value);
				String Value ();

				void Active (bool active);
				bool Active ();

				void Failure (bool failure);
				bool Failure ();

				void State (String state);
				String State ();

				String Summary ();

				Qybercom::Value DTO (bool first);

				void Reset ();
		};
	}
}