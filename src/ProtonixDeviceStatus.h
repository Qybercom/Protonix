#pragma once

#include <Arduino.h>

#include "ProtonixDeviceSensor.h"

#if defined(ESP32) || defined(ESP8266)
#define PROTONIX_LIMIT_SENSORS 256
#else
#define PROTONIX_LIMIT_SENSORS 16
#endif

namespace Qybercom {
	namespace Protonix {
		class ProtonixDeviceStatus {
			private:
				bool _on;
				String _summary;
				ProtonixDeviceSensor* _sensors[PROTONIX_LIMIT_SENSORS];
				unsigned int _sensorCount;
				String _state;

			public:
				ProtonixDeviceStatus();

				void On(bool on);
				bool On();

				void State(String state);
				String State();

				void Summary(String summary);
				String Summary();

				ProtonixDeviceSensor** Sensors();
				unsigned int SensorCount();
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
				ProtonixDeviceSensor* Sensor(unsigned int i);
		};
	}
}
