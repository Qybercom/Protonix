#pragma once

#include <Arduino.h>
#include <Client.h>

#if defined(ESP32) || defined(ESP8266)
#include <StreamString.h>
#include <WiFiClient.h>
#endif

#define PROTONIX_LIMIT_HEADER_LIST 32

namespace Qybercom {
	namespace Protonix {
		class ProtonixHTTPHeader {
			private:
				String _name;
				String _value;

			public:
				ProtonixHTTPHeader(String name, String value);
				String Name();
				String Value();
				String SerializeRequest();
				static ProtonixHTTPHeader* FromResponse(String header);
		};

		class ProtonixHTTPFrame {
			private:
				String _version;
				ProtonixURI* _uri;
				String _method;
				String _status;
				ProtonixHTTPHeader* _headers[PROTONIX_LIMIT_HEADER_LIST];
				unsigned short _headerCurrent;
				String _body;
				//StreamString _bodyStream;
				bool _debug;

			public:
			  	ProtonixHTTPFrame();
			  	ProtonixHTTPFrame(String uri);
			  	ProtonixHTTPFrame(String method, String uri);

				ProtonixHTTPFrame* Debug(bool debug);
				bool Debug();

				ProtonixHTTPFrame* Version(String version);
				String Version();

				ProtonixURI* URI();

				ProtonixHTTPFrame* Method(String method);
				String Method();

				String Status();

				ProtonixHTTPFrame* HeaderAdd(String name, String value);
				ProtonixHTTPFrame* HeaderAdd(ProtonixHTTPHeader* header);
				ProtonixHTTPHeader* Header(String name);
				// TODO: clear headers list
				unsigned short HeaderCount();

				ProtonixHTTPFrame* Body(String body);
				String Body();

				String SerializeRequest();
				ProtonixHTTPFrame* UnserializeResponse(String response);

				int LengthExpected();

				~ProtonixHTTPFrame();
		};

		class ProtonixHTTPClient/* : public StreamString*/ {
			private:
				Client* _client;
  				//WiFiClient _client;
				ProtonixHTTPFrame* _request;
				ProtonixHTTPFrame* _response;
				bool _connected;
				unsigned short _timeoutResponse;
				bool _debug;

				unsigned int _available;
				unsigned int _received;
				/*void _availableBegin() {
					this->_available = this->LengthExpected();
				}*/

				bool _responseBatch(String batch);
				ProtonixHTTPFrame* _responseReset(String response);
				//bool _receive();
				unsigned int _batchSize;
				//bool(*_batchReady)(String);


			public:
				//static WiFiClient _clientWiFi;
				//ProtonixHTTPClient(Client& client);
				ProtonixHTTPClient(Client* client);
				//ProtonixHTTPClient();

				ProtonixHTTPClient* Debug(bool debug);
				bool Debug();

				ProtonixHTTPClient* Request(ProtonixHTTPFrame* request);
				ProtonixHTTPFrame* Response();
				bool Send();
				bool ReceiveHeaders();
				String ReceiveBody(int batchSize = -1);
				bool ReceiveAvailable();
				//char ReceiveBodyChar();
				//bool Receive();
				//bool Receive(unsigned int batchSize, bool(*onBatchReady)(String));

				ProtonixHTTPClient* TimeoutResponse(unsigned short timeout);
				unsigned short TimeoutResponse();

				//WiFiClient Client();
				//Stream& ResponseStream();
				/*int available() override;
				int read() override;
				size_t readBytes(uint8_t *buffer, size_t length) override;
				int peek() override;*/

				#if defined(ESP32) || defined(ESP8266)
				// https://forum.arduino.cc/t/using-esp32-with-ethernetclient-to-make-a-request-using-httpclient/1041648/7
				static ProtonixHTTPClient* OverWiFi();
				#endif

				~ProtonixHTTPClient();
		};
	}
}