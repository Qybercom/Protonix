#pragma once

#include <Arduino.h>
#include <ArduinoWebsockets.h>

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

				typedef void(*ProtonixTimerCallback)(ProtonixTimer*);

				ProtonixTimer();
				ProtonixTimer(unsigned int interval);
				ProtonixTimer(unsigned int interval, ProtonixTimerUnit unit);
				ProtonixTimer(unsigned int interval, ProtonixTimerCallback callback);
				ProtonixTimer(unsigned int interval, ProtonixTimerUnit unit, ProtonixTimerCallback callback);

				unsigned long Previous();

				void Interval(int interval);
				unsigned int Interval();

				void Unit(ProtonixTimerUnit unit);
				ProtonixTimerUnit Unit();

				ProtonixTimer* Callback(ProtonixTimerCallback callback);

				void Pipe();

			private:
				unsigned long _previous;
				unsigned int _interval;
				ProtonixTimerUnit _unit;
				ProtonixTimerCallback _callback;
		};

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
				virtual bool Connect(ProtonixURI* uri);
				virtual bool Connected();
				virtual void Pipe();
		};

		namespace Networks {
			class NWiFi: public Qybercom::Protonix::INetwork {
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
			class PWebSocket: public Qybercom::Protonix::IProtocol {
				private:
					websockets::WebsocketsClient _client;

				public:
					bool Connect(ProtonixURI* uri);
					bool Connected();
					void Pipe();
			};
		}

		class ProtonixDevice;
		class IProtonixDevice {
			virtual unsigned int DeviceTick();
			virtual String DeviceID();
			virtual String DevicePassphrase();
			virtual void DeviceOnReady(ProtonixDevice* device);
		};

		// http://tedfelix.com/software/c++-callbacks.html
		class ProtonixDevice {
			public:
				ProtonixDevice(IProtonixDevice* device);

				void Network(INetwork* network);
				INetwork* Network();

				void Protocol(IProtocol* protocol);
				IProtocol* Protocol();

				void Server(ProtonixURI* uri);
				ProtonixURI* Server();

				void ServerEndpoint(String host, uint port);
				void ServerEndpoint(String host, uint port, String path);

				ProtonixTimer* Timer();
				void Pipe();

				/*ProtonixDevice* OnNetworkConnect(NetworkConnectCallback callback);
				ProtonixDevice* OnProtocolConnect(ProtocolConnectCallback callback);*/

				//void Command();

			private:
				INetwork* _network;
				bool _networkConnected;
				IProtocol* _protocol;
				bool _protocolConnected;
				ProtonixURI* _uri;
				String _id;
				String _passphrase;
				ProtonixTimer _timer;
				/*NetworkConnectCallback _onNetworkConnect;
				ProtocolConnectCallback _onProtocolConnect;*/
		};
	}
}