#pragma once

#include <Arduino.h>

#include "../IProtonixHardware.hpp"
#include "../Protonix.h"

namespace Qybercom {
	namespace Protonix {
		namespace Hardware {
			class HSensorHall2D : public IProtonixHardware {
				private:
					float _value;

					void _signal (Protonix* device);

				public:
					HSensorHall2D (unsigned short pinX, unsigned short pinY, int maxX = QYBERCOM_PROTONIX_ANALOG_MAX, int maxY = QYBERCOM_PROTONIX_ANALOG_MAX, int minX = 0, int minY = 0);

					float ValueFloat ();
					float ValueFloat (float offset);
					bool ValueFloatMatch (float offset, float min, float max, bool minEQ = true, bool maxEQ = true);

					int ValueInt ();
					int ValueInt (int offset);
					bool ValueIntMatch (int offset, int min, int max, bool minEQ = true, bool maxEQ = true);

					String HardwareSummary ();
					void HardwareInitPre (Protonix* device);
					void HardwarePipe (Protonix* device, short core);
					void HardwareOnReset (Protonix* device);
					void HardwareOnCommand (Protonix* device, ProtonixCommand &command);
			};
		}
	}
}