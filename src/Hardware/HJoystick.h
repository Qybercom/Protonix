#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.h"
#include "../Protonix.h"

#include "HTrigger.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HJoystick : public IProtonixHardware {
				private:
					short _pinX;
					short _pinY;
					bool _init;
					int _valueX;
					int _valueY;
					int _maxX;
					int _minX;
					int _maxY;
					int _minY;
					HButton* _button;

				public:
					HJoystick (short pinX = -1, short pinY = -1, short pinButton = -1, bool init = false);
					static HJoystick* Init (short pinX = -1, short pinY = -1, short pinButton = -1, bool init = false);

					short PinX ();
					short PinY ();

					bool Init ();

					int MaxX ();
					HJoystick* MaxX (int max);

					int MinX ();
					HJoystick* MinX (int min);

					int MaxY ();
					HJoystick* MaxY (int max);

					int MinY ();
					HJoystick* MinY (int min);

					HButton* Button ();

					int ValueX ();
					int ValueY ();

					int PositionX (int min = -100, int max = 100);
					int PositionY (int min = -100, int max = 100);

					void HardwareInitPre (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwarePipeInterrupt (Protonix* device);
					void HardwareOnCommand (Protonix* device, String command);
			};
		}
	}
}