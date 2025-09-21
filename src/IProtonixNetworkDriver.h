#pragma once

#include <Arduino.h>
#include "Client.h"

#include "IProtonixNetworkClient.h"
#include "ProtonixTimer.h"

namespace Qybercom {
	namespace Protonix {
		class Protonix;

		class IProtonixNetworkDriver {
			protected:
				static unsigned short _id;

				bool _single;
				String _kind;
				String _name;
				String _mac;
				uint8_t _macBuffer[6];
				String _hostname;
				ProtonixTimer* _timerConnect;
				bool _ready;
				bool _connected;
				bool _connectAttempt;
				bool _connectAuto;

				bool _init (bool single, String kind, String name, String mac, String hostname, bool connectAuto = true, unsigned int timerConnect = 1000);
				void _macParse ();
				String _ip (IPAddress ip);
				bool _log (String message, bool ret = true);

			public:
				ProtonixTimer* TimerConnect ();

				String NetworkDriverName ();
				bool NetworkDriverConnectAuto ();
				IProtonixNetworkDriver* NetworkDriverConnectAuto (bool enabled);
				bool NetworkDriverPipe (Protonix* device);

				virtual bool NetworkDriverConnect () = 0;
				virtual bool NetworkDriverConnected () = 0;
				virtual bool NetworkDriverDisconnect () = 0;
				virtual String NetworkDriverAddressMAC () = 0;
				virtual String NetworkDriverAddressIP () = 0;
				virtual IProtonixNetworkClient* NetworkDriverAllocateClient () = 0;

				virtual ~IProtonixNetworkDriver () { };

				static void ParseMAC (String mac, uint8_t out[6]);
		};
	}
}