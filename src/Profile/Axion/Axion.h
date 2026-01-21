#pragma once

#include <Arduino.h>

#include "../../Common/index.h"

#include "../../IProtonixProfile.h"
#include "../../Protonix.h"
#include "../../ProtonixTimer.h"
#include "../../ProtonixNetworkClient.h"

/*#include "IAxionDTO.h"

#include "AxionDTO.h"*/

namespace Qybercom {
	namespace Protonix {
		namespace Profile {
			namespace Axion {
				class Axion : public IProtonixProfile {
					private:
						String _uriStream;
						String _uriHTTP;
						ProtonixNetworkClient* _clientStream;
						ProtonixTimer* _timerConnectStream;
						ProtonixTimer* _timerAuthorize;
						ProtonixTimer* _timerData;
						/*AxionDTO* _dtoInput;
						AxionDTO* _dtoOutput;*/
						Qybercom::IBucketFormat* _format;
						bool _authorized;
						bool _autoConnectStream;
						bool _autoAuthorize;
						bool _autoData;
						bool _dataFirst;
						bool _dataMemory;
						bool _dataHardware;
						bool _debug;

						/*void _onStreamURL (Protonix* device);
						void _onStreamResponse (Protonix* device);
						void _onStreamEvent (Protonix* device);*/

					public:
						Axion (String uriStream, String uriHTTP, unsigned int intervalData = 100, unsigned int intervalConnectStream = 1000, unsigned int intervalAuthorize = 1000);

						void ProfilePipe (Protonix* device);
						String ProfileFirmwareURI (Protonix* device, String platform, String version);

						ProtonixNetworkClient* ClientStream ();

						ProtonixTimer* TimerConnectStream ();
						ProtonixTimer* TimerAuthorize ();
						ProtonixTimer* TimerData ();

						/*AxionDTO* DTOInput ();
						AxionDTO* DTOOutput ();*/

						bool Authorized ();

						bool AutoConnectStream ();
						Axion* AutoConnectStream (bool value);

						bool AutoAuthorize ();
						Axion* AutoAuthorize (bool value);

						bool AutoData ();
						Axion* AutoData (bool value);

						bool DataMemory ();
						Axion* DataMemory (bool value);

						bool DataHardware ();
						Axion* DataHardware (bool value);

						//void RequestStream (Protonix* device, String url, IAxionDTORequest* request);
						void RequestStream (Protonix* device, String url, const Qybercom::Bucket &request);
						void RequestStreamAuthorize (Protonix* device);
						void RequestStreamDeviceData (Protonix* device);

						bool Debug ();
						Axion* Debug (bool debug);
				};
			}
		}
	}
}