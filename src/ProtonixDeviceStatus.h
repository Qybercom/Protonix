#pragma once

#include <Arduino.h>

#include "ProtonixDeviceSensor.h"

namespace Qybercom {
	namespace Protonix {
		class ProtonixDeviceStatus {
			private:
				bool _on;
				String _summary;
				ProtonixDeviceSensor* _sensors[16];
				unsigned int _sensorCount;

			public:
				ProtonixDeviceStatus();

				void On(bool on);
				bool On();

				void Summary(String summary);
				String Summary();

				ProtonixDeviceSensor** Sensors();
				unsigned int SensorCount();
				ProtonixDeviceStatus* SensorAdd(String id);
				ProtonixDeviceStatus* SensorSet(String id, String value);
				ProtonixDeviceStatus* SensorSet(String id, String value, bool active);
				ProtonixDeviceStatus* SensorSet(String id, String value, bool active, bool failure);
				ProtonixDeviceStatus* SensorSet(String id, bool active);
				ProtonixDeviceStatus* SensorSet(String id, bool active, bool failure);
				ProtonixDeviceStatus* SensorSet(ProtonixDeviceSensor* sensor);
				ProtonixDeviceStatus* SensorReset();
				ProtonixDeviceStatus* SensorReset(String id);
		};
	}
}