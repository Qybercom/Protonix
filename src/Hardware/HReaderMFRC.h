#include <Arduino.h>
#include <MFRC522.h>

#include "../IProtonixHardware.h"

namespace Qybercom {
	namespace Protonix {
        namespace Hardware {
			class HReaderMFRC : public IProtonixHardware {
    			private:
		            int _pinSS;
		            int _pinRST;
    				MFRC522 _reader;
                    String _cardSerial;

	            public:
            		HReaderMFRC(int pinSS, int pinRST);

	                HReaderMFRC* PinSS(int pinSS);
	                int PinSS();

	                HReaderMFRC* PinRST(int pinRST);
	                int PinRST();

                    byte AntennaGain();
                    String Version();

                    String CardSerial();

	                IProtonixHardware::Type HardwareType();
					void HardwareInitPre();
					void HardwareInitPost();
                    void HardwarePipe();
			};
        }
	}
}