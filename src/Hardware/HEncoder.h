#pragma once

#include <Arduino.h>

#include "../Common/index.h"

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

#include "HButton.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			struct HEncoderState {
				short Dir;
				bool Button;

				HEncoderState () : Dir(0), Button(false) { }
				HEncoderState (short dir, bool button) : Dir(dir), Button(button) { }
			};

			class HEncoder : public IProtonixHardware {
				private:
					volatile bool _valA;
					volatile bool _valB;
					volatile short _dir;
					volatile bool _changed;
					Qybercom::Pipes::Filter<short> _filter; // use ~75
					HButton* _button;

					bool _changedPipe ();
					void _signal (Protonix* device);

				public:
					HEncoder (unsigned short pinA, unsigned short pinB, unsigned int checkInterval = 0);
					static HEncoder* Init (unsigned short pinA, unsigned short pinB, unsigned int checkInterval = 0);

					static HEncoder* WithButton (unsigned short pinA, unsigned short pinB, unsigned short pinButton);
					static HEncoder* WithButton (unsigned short pinA, unsigned short pinB, unsigned short pinButton, unsigned int checkInterval);
					static HEncoder* WithButton (unsigned short pinA, unsigned short pinB, unsigned short pinButton, unsigned int checkInterval, unsigned int checkIntervalButton);

					bool ValA ();
					bool ValB ();

					short Dir ();

					bool Changed ();

					HButton* Button ();

					Qybercom::Pipes::Filter<short> &Filter ();

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwarePipeInterrupt (Protonix* device);
					void HardwareOnReset (Protonix* device);
					void HardwareOnCommand (Protonix* device, ProtonixCommand &command);
			};
		}
	}
}