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
			struct HJoystickState {
				Qybercom::Types::Position Position;
				bool Button;
				bool Calibrated;

				HJoystickState (int x, int y, bool button, bool calibrated) :
					Position(Qybercom::Types::Position(x, y)),
					Button(button),
					Calibrated(calibrated) { }
			};

			class HJoystick : public IProtonixHardware, public IValueListener {
				private:
					HButton* _button;
					int _valueX;
					int _valueY;
					bool _positionChanged;
					int _positionX;
					int _positionY;
					ProtonixTimer* _calibrateTimeout;
					bool _calibrated;
					int _gapMaxX;
					int _gapMinX;
					int _gapMaxY;
					int _gapMinY;
					bool _first;

					void _signalValue (Protonix* device);
					void _signalPosition (Protonix* device);

				public:
					HJoystick (short pinX = -1, short pinY = -1, short pinButton = -1, bool init = false);

					HJoystick* Calibrate ();
					ProtonixTimer* CalibrateTimeout ();
					HJoystick* Calibrate (int gapMinX, int gapMaxX, int gapMinY, int gapMaxY);

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
					void HardwareOnCommand (Protonix* device, const ProtonixCommand &command);

					void ValueListenerSet (Value &value);

					static int Position (int value, int valMin, int valMax, int gapMin, int gapMax);
			};
		}
	}
}