#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HSensorOrientationMPU6050 : public IProtonixHardware {
				private:
					bool _init;

					int _axRaw;
					int _ayRaw;
					int _azRaw;
					int _gxRaw;
					int _gyRaw;
					int _gzRaw;

					float _temperature;
					float _ax;
					float _ay;
					float _az;
					float _gx;
					float _gy;
					float _gz;

					int _offsetGX;
					int _offsetGY;
					int _offsetGZ;

					unsigned long _lastUpdate;

					int _read (char reg);
					bool _readRaw ();

				public:
					HSensorOrientationMPU6050 (char address = 0x68);

					float Temperature ();
					float AccelerometerX ();
					float AccelerometerY ();
					float AccelerometerZ ();
					float GyroscopeX ();
					float GyroscopeY ();
					float GyroscopeZ ();

					bool Calibrate (unsigned int samples = 500, unsigned int timeout = 2);

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					bool HardwareI2C ();
					void HardwareI2CPre (Protonix* device);
					void HardwareI2CPost (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareOnCommand (Protonix* device, ProtonixCommand &command);
			};
		}
	}
}