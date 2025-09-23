#pragma once

#include <Arduino.h>

#include "Common/Debouncer.hpp"

#include "../IProtonixHardware.h"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HEncoder : public IProtonixHardware {
				private:
					unsigned short _pinA;
					unsigned short _pinB;
					unsigned short _pinButton;
					volatile bool _valA;
					volatile bool _valB;
					volatile short _dir;
					volatile bool _clicked;
					volatile bool _changed;
					volatile bool _changedButton;
					volatile bool _withButton;
					volatile bool _allowZero;
					Qybercom::Debouncer<short> _debouncer; // use ~75
					Qybercom::Debouncer<bool> _debouncerButton;

					bool _changedPipe ();

				public:
					HEncoder (unsigned short pinA, unsigned short pinB, unsigned int checkInterval = 0);

					static HEncoder* WithButton (unsigned short pinA, unsigned short pinB, unsigned short pinButton);
					static HEncoder* WithButton (unsigned short pinA, unsigned short pinB, unsigned short pinButton, unsigned int checkInterval);
					static HEncoder* WithButton (unsigned short pinA, unsigned short pinB, unsigned short pinButton, unsigned int checkInterval, unsigned int checkIntervalButton);

					unsigned short PinA ();
					unsigned short PinB ();
					unsigned short PinButton ();
					bool ValA ();
					bool ValB ();
					short Dir ();
					bool Clicked ();
					bool Changed ();
					bool ChangedButton ();
					bool WithButton ();

					bool AllowZero ();
					HEncoder* AllowZero (bool allow);

					Qybercom::Debouncer<short> &Debouncer ();
					Qybercom::Debouncer<bool> &DebouncerButton ();

					void HardwareInitPre (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwarePipeInterrupt (Protonix* device);
					void HardwareOnCommand (Protonix* device, String command);
			};
		}
	}
}