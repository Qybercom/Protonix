#pragma once

#include <Arduino.h>

#include "Common/Debouncer.hpp"

#include "../IProtonixHardware.h"
#include "../Protonix.h"

#include "HButton.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			struct HEncoderState {
				short Dir;
				bool Button;

				HEncoderState (short dir, bool button) : Dir(dir), Button(button) { }
			};

			class HEncoder : public IProtonixHardware {
				private:
					unsigned short _pinA;
					unsigned short _pinB;
					volatile bool _valA;
					volatile bool _valB;
					volatile short _dir;
					volatile bool _changed;
					volatile bool _allowZero;
					Qybercom::Debouncer<short> _debouncer; // use ~75
					HButton* _button;

					bool _changedPipe ();

				public:
					HEncoder (unsigned short pinA, unsigned short pinB, unsigned int checkInterval = 0);
					static HEncoder* Init (unsigned short pinA, unsigned short pinB, unsigned int checkInterval = 0);

					static HEncoder* WithButton (unsigned short pinA, unsigned short pinB, unsigned short pinButton);
					static HEncoder* WithButton (unsigned short pinA, unsigned short pinB, unsigned short pinButton, unsigned int checkInterval);
					static HEncoder* WithButton (unsigned short pinA, unsigned short pinB, unsigned short pinButton, unsigned int checkInterval, unsigned int checkIntervalButton);

					unsigned short PinA ();
					unsigned short PinB ();

					bool ValA ();
					bool ValB ();

					short Dir ();

					bool Changed ();

					HButton* Button ();

					bool AllowZero ();
					HEncoder* AllowZero (bool allow);

					Qybercom::Debouncer<short> &Debouncer ();

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwarePipeInterrupt (Protonix* device);
					void HardwareOnCommand (Protonix* device, String command);
			};
		}
	}
}