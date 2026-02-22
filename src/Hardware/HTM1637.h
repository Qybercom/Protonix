#pragma once

#include <Arduino.h>

#include "../Common/index.h"

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			struct TM1637Char {
				private:
					char _value;

				public:
					TM1637Char (char c);
					TM1637Char (char c, bool dot);
					char Value ();
					TM1637Char &Dot (bool on = true);
					static char Encode (char c);
			};

			class HTM1637 : public IProtonixHardware, public IValueListener {
				private:
					bool _init;

					bool _start ();
					bool _stop ();

				public:
					HTM1637 (unsigned short pinCLK, unsigned short pinDT);

					bool Brightness (unsigned short brightness);
					bool Write (char b);
					bool Render (TM1637Char c1, TM1637Char c2, TM1637Char c3, TM1637Char c4);
					bool Render (char c1, char c2, char c3, char c4);
					bool Render (String value);

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					void HardwareInitPost (Protonix* device);
					void HardwarePipe (Protonix* device, short core);

					void ValueListenerSet (Value &value);
			};
		}
	}
}