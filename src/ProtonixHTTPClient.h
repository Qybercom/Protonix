#pragma once

#include <Arduino.h>
#include <Client.h>
#include <WiFiClient.h>

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

                ~ProtonixHTTPFrame();
        };

        class ProtonixHTTPClient {
        	private:
                //Client& _client;
  				WiFiClient _client;
                ProtonixHTTPFrame* _request;
                ProtonixHTTPFrame* _response;
                bool _connected;
                unsigned short _timeoutResponse;
                bool _debug;

            public:
                //ProtonixHTTPClient(Client& client);
                ProtonixHTTPClient();

                ProtonixHTTPClient* Debug(bool debug);
                bool Debug();

                ProtonixHTTPClient* Request(ProtonixHTTPFrame* request);
                ProtonixHTTPFrame* Response();
                bool Send();
                bool Receive();

                ProtonixHTTPClient* TimeoutResponse(unsigned short timeout);
                unsigned short TimeoutResponse();

            	//static ProtonixHTTPClient* OverWiFi();
        };
    }
}