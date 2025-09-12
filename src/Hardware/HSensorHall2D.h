#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../ProtonixDevice.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HSensorHall2D : public IProtonixHardware {
				private:
					float _value;
					unsigned short _pinX;
					unsigned short _pinY;
					int _maxX;
					int _maxY;
					int _minX;
					int _minY;

				public:
					HSensorHall2D (unsigned short pinX, unsigned short pinY, int maxX = ANALOG_MAX, int maxY = ANALOG_MAX, int minX = 0, int minY = 0);

					unsigned short PinX ();
					HSensorHall2D* PinX (unsigned short pinX);

					unsigned short PinY ();
					HSensorHall2D* PinY (unsigned short pinY);

					int MaxX ();
					HSensorHall2D* MaxX (int maxX);

					int MaxY ();
					HSensorHall2D* MaxY (int maxY);

					int MinX ();
					HSensorHall2D* MinX (int minX);

					int MinY ();
					HSensorHall2D* MinY (int minY);

					float ValueFloat ();
					float ValueFloat (float offset);
					bool ValueFloatMatch (float offset, float min, float max, bool minEQ = true, bool maxEQ = true);

					int ValueInt ();
					int ValueInt (int offset);
					bool ValueIntMatch (int offset, int min, int max, bool minEQ = true, bool maxEQ = true);

					bool HardwareSPI ();
					void HardwareInitPre (ProtonixDevice* device);
					void HardwareInitPost (ProtonixDevice* device);
					void HardwarePipe (ProtonixDevice* device, short core);
					void HardwareCommand (ProtonixDevice* device, String command);
			};
		}
	}
}