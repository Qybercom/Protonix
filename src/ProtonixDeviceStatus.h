#pragma once

#include <Arduino.h>

#include "Common/List.hpp"

#include "ProtonixDeviceSensor.h"

/*#if defined(ESP32) || defined(ESP8266)
#define PROTONIX_LIMIT_SENSORS 256
#else
#define PROTONIX_LIMIT_SENSORS 16
#endif*/

namespace Qybercom {
	namespace Protonix {
		class ProtonixDeviceStatus {
			private:
				String _firmware;
				bool _on;
				String _state;
				String _uptime;
				String _summary;

				/*ProtonixDeviceSensor* _sensors[PROTONIX_LIMIT_SENSORS];
				unsigned int _sensorCount;*/
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

				/*ProtonixDeviceSensor** Sensors();
				unsigned int SensorCount();*/
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
				//ProtonixDeviceSensor* Sensor(unsigned int i);
		};
	}
}
