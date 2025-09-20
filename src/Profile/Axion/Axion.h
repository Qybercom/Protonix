#pragma once

#include <Arduino.h>

#include "../../IProtonixProfile.h"
#include "../../Protonix.h"
#include "../../ProtonixTimer.h"
#include "../../ProtonixNetworkClient.h"

#include "IAxionDTO.h"

#include "AxionDTO.h"

namespace Qybercom {
	namespace Protonix {
		namespace Profile {
			namespace Axion {
				class Axion : public IProtonixProfile {
					private:
						String _uriStream;
						String _uriHTTP;
						ProtonixNetworkClient* _clientStream;
						AxionDTO* _dtoInput;
						AxionDTO* _dtoOutput;
						ProtonixTimer* _timerConnectStream;
						ProtonixTimer* _timerAuthorize;
						ProtonixTimer* _timerData;
						bool _authorized;
						bool _autoConnectStream;
						bool _autoData;
						bool _autoAuthorize;
						bool _debug;

						void _onStreamURL (Protonix* device);
						void _onStreamResponse (Protonix* device);
						void _onStreamEvent (Protonix* device);

					public:
						Axion (String uriStream, String uriHTTP, unsigned int intervalData = 100, unsigned int intervalConnectStream = 1000, unsigned int intervalAuthorize = 1000);

						void ProfilePipe (Protonix* device);

						ProtonixNetworkClient* ClientStream ();

						ProtonixTimer* TimerConnectStream ();
						ProtonixTimer* TimerAuthorize ();
						ProtonixTimer* TimerData ();

						AxionDTO* DTOInput ();
						AxionDTO* DTOOutput ();

						bool Authorized ();

						bool AutoConnectStream ();
						Axion* AutoConnectStream (bool value);

						bool AutoAuthorize ();
						Axion* AutoAuthorize (bool value);

						bool AutoData ();
						Axion* AutoData (bool value);

						void RequestStream (String url, IAxionDTORequest* request);
						void RequestStreamAuthorize (Protonix* device);
						void RequestStreamDeviceData (Protonix* device);

						bool Debug ();
						Axion* Debug (bool debug);
				};
			}
		}
	}
}