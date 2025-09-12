#pragma once

#include <Arduino.h>

#include "Common/Debouncer.hpp"

#include "../IProtonixHardware.h"
#include "../ProtonixDevice.h"

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
					Qybercom::Debouncer<short> _debouncer;
					Qybercom::Debouncer<bool> _debouncerButton;

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
					bool Changed (bool allowZero = false);
					bool ChangedButton ();
					bool WithButton ();
					Qybercom::Debouncer<short> &Debouncer ();
					Qybercom::Debouncer<bool> &DebouncerButton ();

					bool HardwareSPI ();
					void HardwareInitPre (ProtonixDevice* device);
					void HardwareInitPost (ProtonixDevice* device);
					void HardwarePipe (ProtonixDevice* device, short core);
					void HardwarePipeInterrupt (ProtonixDevice* device);
					void HardwareCommand (ProtonixDevice* device, String command);
			};
		}
	}
}