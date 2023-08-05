#pragma once

#include <Arduino.h>
#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>

namespace Qybercom {
	namespace Protonix {
		class ProtonixURI {
			private:
				String _scheme;
				String _username;
				String _password;
				String _host;
				uint _port;
				String _path;

			public:
				ProtonixURI(String host, uint port);
				ProtonixURI(String host, uint port, String path);

				void Scheme(String scheme);
				String Scheme();

				void Username(String username);
				String Username();

				void Password(String password);
				String Password();

				void Host(String host);
				String Host();

				void Port(uint port);
				uint Port();

				void Path(String path);
				String Path();
		};

		class ProtonixTimer {
			public:
				enum class ProtonixTimerUnit {
					MICROSECONDS,
					MILLISECONDS
				};

				ProtonixTimer();
				ProtonixTimer(unsigned int interval);
				ProtonixTimer(unsigned int interval, ProtonixTimerUnit unit);

				unsigned long Previous();

				void Interval(int interval);
				unsigned int Interval();

				void Unit(ProtonixTimerUnit unit);
				ProtonixTimerUnit Unit();

				bool Pipe();

			private:
				unsigned long _previous;
				unsigned int _interval;
				ProtonixTimerUnit _unit;
		};

		class ProtonixProtocolDTO {
			private:
				String _url;
				String _response;
				String _event;
				JsonObject _data;
				StaticJsonDocument<512> _dto;

			public:
				ProtonixProtocolDTO();

				void URL(String url);
				String URL();

				void Response(String url);
				String Response();

				void Event(String url);
				String Event();

				void Data(JsonObject data);
				JsonObject Data();

				bool IsURL();
				bool IsResponse();
				bool IsEvent();

				String Serialize();
				bool Deserialize(String raw);
		};

		class ProtonixDevice;

		class INetwork {
			public:
				virtual bool Connect();
				virtual bool Connected();
				virtual String AddressMAC();
				virtual String AddressIP();

				// https://stackoverflow.com/a/12772708/2097055
				// https://stackoverflow.com/a/1755042/2097055
				static void ParseMAC (String mac, uint8_t out[6]) {
					char buffer[18];
					mac.toCharArray(buffer, 18);

					sscanf(buffer, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &out[0], &out[1], &out[2], &out[3], &out[4], &out[5]);
				}
		};

		class IProtocol {
			public:
				virtual void Init(ProtonixDevice* device);
				virtual bool Connect(ProtonixURI* uri);
				virtual bool Connected();
				virtual void Pipe();
		};

		namespace Networks {
			class NWiFi: public INetwork {
				private:
					String _ssid;
					String _password;
					String _mac;
					String _hostname;
				
				public:
					NWiFi(String ssid, String password, String mac, String hostname);

					bool Connect();

					bool Connected();

					String AddressMAC();

					String AddressIP();
			};
		}

		namespace Protocols {
			class PWebSocket: public IProtocol {
				private:
					websockets::WebsocketsClient _client;
					ProtonixDevice* _device;

				public:
					void _input(ProtonixProtocolDTO* dto);
					void Init(ProtonixDevice* device);
					bool Connect(ProtonixURI* uri);
					bool Connected();
					void Pipe();
			};
		}

		class IProtonixDevice {
			public:
				virtual unsigned int DeviceTick();
				virtual String DeviceID();
				virtual String DevicePassphrase();
				virtual void DeviceOnReady(ProtonixDevice* device);
				virtual void DeviceOnNetworkConnect(ProtonixDevice* device);
				virtual void DeviceOnProtocolConnect(ProtonixDevice* device);
				virtual void DeviceOnStreamResponse(ProtonixDevice* device, String url);
				virtual void DeviceOnStreamEvent(ProtonixDevice* device, String url);
		};

		// http://tedfelix.com/software/c++-callbacks.html
		class ProtonixDevice {
			public:
				ProtonixDevice(IProtonixDevice* device);

				void Device(IProtonixDevice* device);
				IProtonixDevice* Device();
				ProtonixTimer* Timer();

				void Network(INetwork* network);
				INetwork* Network();

				void Protocol(IProtocol* protocol);
				IProtocol* Protocol();

				void Server(ProtonixURI* uri);
				ProtonixURI* Server();

				void ServerEndpoint(String host, uint port);
				void ServerEndpoint(String host, uint port, String path);
				
				void Pipe();

				void OnStreamResponse(ProtonixProtocolDTO* dto);
				void OnStreamEvent(ProtonixProtocolDTO* dto);

				//void Command();

			private:
				IProtonixDevice* _device;
				bool _ready;
				ProtonixTimer* _timer;
				INetwork* _network;
				bool _networkConnected1;
				bool _networkConnected2;
				IProtocol* _protocol;
				bool _protocolConnected1;
				bool _protocolConnected2;
				ProtonixURI* _uri;
				void _pipe();
		};
	}
}