#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"
#include "../ProtonixTimer.h"

#include "../Common/index.h"

#include "HTrigger.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			/*struct HJoystickState {
				Vector2 Position;
				bool Button;
				bool Calibrated;

				HJoystickState (int x, int y, bool button, bool calibrated) : Position(Vector2(x, y)), Button(button), Calibrated(calibrated) { }
			};*/

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
					int _positionX;
					int _positionY;
					bool _positionChanged;
					HButton* _button;
					bool _calibrateAuto;
					ProtonixTimer* _calibrateTimeout;
					bool _calibrated;
					int _gapMaxX;
					int _gapMinX;
					int _gapMaxY;
					int _gapMinY;
					bool _allowSignalValue;
					bool _first;

					void _signalValue (Protonix* device);
					void _signalPosition (Protonix* device);

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

					bool CalibrateAuto ();
					HJoystick* CalibrateAuto (bool calibrate);
					HJoystick* Calibrate ();
					ProtonixTimer* CalibrateTimeout ();
					HJoystick* Calibrate (int gapMinX, int gapMaxX, int gapMinY, int gapMaxY);

					bool AllowSignalValue ();
					HJoystick* AllowSignalValue (bool allow);

					HButton* Button ();

					int ValueX ();
					int ValueY ();

					int PositionX ();
					int PositionY ();

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwarePipeInterrupt (Protonix* device);
					void HardwareOnReset (Protonix* device);
					void HardwareOnCommand (Protonix* device, String command);

					static int Position (int value, int valMin, int valMax, int gapMin, int gapMax);
			};
		}
	}
}