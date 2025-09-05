#pragma once

#include <Arduino.h>

#include "Common/List.hpp"

#include "ProtonixDeviceSensor.h"

namespace Qybercom {
	namespace Protonix {
		class ProtonixDeviceStatus {
			private:
				String _firmware;
				bool _on;
				String _state;
				String _uptime;
				String _summary;

				List<ProtonixDeviceSensor*> _sensors;

			public:
				ProtonixDeviceStatus();

				void Firmware(String firmware);
				String Firmware();

				void On(bool on);
				bool On();

				void State(String state);
				String State();

				void Uptime(String uptime);
				String Uptime();

				void Summary(String summary);
				String Summary();

				List<ProtonixDeviceSensor*> &Sensors ();
				ProtonixDeviceStatus* SensorAdd(String id);
				ProtonixDeviceStatus* SensorSet(String id, String value);
				ProtonixDeviceStatus* SensorSet(String id, String value, bool active);
				ProtonixDeviceStatus* SensorSet(String id, String value, bool active, bool failure);
				ProtonixDeviceStatus* SensorSet(String id, String value, bool active, bool failure, String state);
				ProtonixDeviceStatus* SensorSet(String id, bool active);
				ProtonixDeviceStatus* SensorSet(String id, bool active, bool failure);
				ProtonixDeviceStatus* SensorSet(String id, bool active, bool failure, String state);
				ProtonixDeviceStatus* SensorSet(String id, String value, String state);
				ProtonixDeviceStatus* SensorSet(ProtonixDeviceSensor* sensor);
				ProtonixDeviceStatus* SensorReset();
				ProtonixDeviceStatus* SensorReset(String id);
				ProtonixDeviceSensor* Sensor(String id);
		};
	}
}