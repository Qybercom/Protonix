#include <Arduino.h>
#include <Client.h>
#include <WiFiClient.h>

#include "ProtonixURI.h"
#include "ProtonixHTTPClient.h"

using namespace Qybercom::Protonix;

ProtonixHTTPHeader::ProtonixHTTPHeader(String name, String value) {
	this->_name = name;
    this->_value = value;
}

String ProtonixHTTPHeader::Name() {
	return this->_name;
}

String ProtonixHTTPHeader::Value() {
	return this->_value;
}

String ProtonixHTTPHeader::SerializeRequest() {
	return this->_name + ": " + this->_value + "\r\n";
}

ProtonixHTTPHeader* ProtonixHTTPHeader::FromResponse(String header) {
	int posValue = header.indexOf(':');

    String name = header.substring(0, posValue);
    name.trim();

    String value = header.substring(posValue + 1);
    value.trim();

    return new ProtonixHTTPHeader(name, value);
}



ProtonixHTTPFrame::ProtonixHTTPFrame() {
  	this->_version = "HTTP/1.0";
    this->_method = "GET";
	this->_uri = new ProtonixURI();
	this->_headerCurrent = 0;

    this->Debug(false);
}

ProtonixHTTPFrame::ProtonixHTTPFrame(String uri) {
  	this->_version = "HTTP/1.0";
    this->_method = "GET";
	this->_uri = new ProtonixURI(uri);
	this->_headerCurrent = 0;

    this->Debug(false);
    this->HeaderAdd("Host", this->_uri->Host());
}

ProtonixHTTPFrame::ProtonixHTTPFrame(String method, String uri) {
  	this->_version = "HTTP/1.0";
    this->_method = method;
	this->_uri = new ProtonixURI(uri);
	this->_headerCurrent = 0;

    this->Debug(false);
    this->HeaderAdd("Host", this->_uri->Host());
}

ProtonixHTTPFrame* ProtonixHTTPFrame::Debug(bool debug) {
    this->_debug = debug;

    return this;
}

bool ProtonixHTTPFrame::Debug() {
    return this->_debug;
}

ProtonixHTTPFrame* ProtonixHTTPFrame::Version(String version) {
	this->_version = version;

    return this;
}

String ProtonixHTTPFrame::Version() {
    return this->_version;
}

ProtonixURI* ProtonixHTTPFrame::URI() {
	return this->_uri;
}

ProtonixHTTPFrame* ProtonixHTTPFrame::Method(String method) {
	this->_method = method;

    return this;
}

String ProtonixHTTPFrame::Method() {
    return this->_method;
}

String ProtonixHTTPFrame::Status() {
    return this->_status;
}

ProtonixHTTPFrame* ProtonixHTTPFrame::HeaderAdd(String name, String value) {
	this->_headers[this->_headerCurrent] = new ProtonixHTTPHeader(name, value);
    this->_headerCurrent++;

    return this;
}

ProtonixHTTPFrame* ProtonixHTTPFrame::HeaderAdd(ProtonixHTTPHeader* header) {
	this->_headers[this->_headerCurrent] = header;
    this->_headerCurrent++;

    return this;
}

ProtonixHTTPHeader* ProtonixHTTPFrame::Header(String name) {
	int i = 0;

    while (i < this->_headerCurrent) {
    	if (this->_headers[i]->Name() == name)
        	return this->_headers[i];

    	i++;
    }

    return nullptr;
}

unsigned short ProtonixHTTPFrame::HeaderCount() {
	return this->_headerCurrent;
}

ProtonixHTTPFrame* ProtonixHTTPFrame::Body(String body) {
	this->_body = body;

    return this;
}

String ProtonixHTTPFrame::Body() {
	return this->_body;
}

String ProtonixHTTPFrame::SerializeRequest() {
	String raw = this->_method + " " + this->_uri->Path() + " " + this->_version + "\r\n";

    int i = 0;
    while (i < this->_headerCurrent) {
    	raw += this->_headers[i]->SerializeRequest();

    	i++;
    }

	return raw + "\r\n" + this->_body;
}

ProtonixHTTPFrame* ProtonixHTTPFrame::UnserializeResponse(String response) {
	if (this->_debug) {
        Serial.println("[debug] ProrotnixHTTPFrame::UnserializeResponse:");
        Serial.println(response);
    }

    String buffer = "";
    int posHead = response.indexOf("\r\n");

    if (posHead == -1) {
        Serial.println("[WARNING] ProtonixHTTPFrame::UnserializeResponse: No valid response prompt");
    }
    else {
    	String head = response.substring(0, posHead);
        int posVersion = head.indexOf(' ');

        if (posVersion == -1) {
        	Serial.println("[WARNING] ProtonixHTTPFrame::UnserializeResponse: No version in response");
        }
        else {
        	this->_version = head.substring(0, posVersion);
            this->_status = head.substring(posVersion + 1);

            buffer = response.substring(posHead + 2);
            int posHeaders = buffer.indexOf("\r\n\r\n");
            int posBody = (posHeaders == -1 ? posHead + 2 : posHeaders + 4);

            if (posHeaders == -1) {
              	if (this->_debug)
            		Serial.println("[debug] ProtonixHTTPFrame::UnserializeResponse: No response headers");
            }
            else {
            	String bufferHeaders = buffer.substring(0, posBody - 2);

                if (this->_debug) {
	                Serial.println("[debug] headers");
	                Serial.println(bufferHeaders);
                }

				int posHeader = 0;
				int posHeaderPrev = 0;
                int currentHeader = 0;
                String bufferHeader = "";

                while (posHeader != -1 && currentHeader < PROTONIX_LIMIT_HEADER_LIST) {
                	posHeader = bufferHeaders.indexOf("\r\n", posHeaderPrev);

                    if (posHeader != -1) {
						bufferHeader = bufferHeaders.substring(posHeaderPrev, posHeader);

                        this->HeaderAdd(ProtonixHTTPHeader::FromResponse(bufferHeader));

                        posHeaderPrev = posHeader + 2;
                        currentHeader++;
                    }
                }
            }

            if (posBody == -1) {
            	if (this->_debug)
            		Serial.println("[debug] ProtonixHTTPFrame::UnserializeResponse: No response body");
            }
            else {
            	this->Body(buffer.substring(posBody));
            }
        }
    }

    return this;
}

ProtonixHTTPFrame::~ProtonixHTTPFrame() {
	delete this->_uri;
    this->_uri = nullptr;

	int i = 0;

    while (i < this->_headerCurrent) {
    	delete this->_headers[i];
        this->_headers[i] = nullptr;

    	i++;
    }
}



/*ProtonixHTTPClient::ProtonixHTTPClient(Client& client) {
	this->_client = client;
}*/

ProtonixHTTPClient::ProtonixHTTPClient() {
	this->_connected = false;
    this->_request = nullptr;
    this->_response = nullptr;
    this->_timeoutResponse = 20;
    this->Debug(false);
}

ProtonixHTTPClient* ProtonixHTTPClient::Debug(bool debug) {
    this->_debug = debug;

    return this;
}

bool ProtonixHTTPClient::Debug() {
    return this->_debug;
}

ProtonixHTTPClient* ProtonixHTTPClient::Request(ProtonixHTTPFrame* request) {
	if (this->_request != nullptr) {
        delete this->_request;
        this->_request = nullptr;
	}

  	this->_request = request;

	return this;
}

ProtonixHTTPFrame* ProtonixHTTPClient::Response() {
	return this->_response;
}

bool ProtonixHTTPClient::Send() {
  	bool ok = false;

  	if (this->_response != nullptr) {
        delete this->_response;
        this->_response = nullptr;
	}

    if (!this->_client.connected()) {
    	this->_connected = this->_client.connect(
        	this->_request->URI()->Host(),
        	this->_request->URI()->PortConnection()
        );

    	if (this->_connected) {
			Serial.println("[debug] ProtonixHTTPClient::Send Connected");
    	}
        else {
			Serial.println("[debug] ProtonixHTTPClient::Send FAILURE");
    	}
    }

    if (this->_connected) {
    	String request = this->_request->SerializeRequest();

        if (this->_debug) {
        	Serial.println("[debug] ProtonixHTTPClient::Send Request:");
        	Serial.println(request);
        }

    	ok = this->_client.write(request.c_str());
        this->_client.flush();
    }

	return ok;
}

bool ProtonixHTTPClient::Receive() {
	// TODO: schedule for async receiveing for non-blocking other tasks
	if (!this->_connected) return false;
    delay(this->_timeoutResponse);

    String response = "";

    while (this->_client.available()) {
    	char c = this->_client.read();
        response += c;
    }

    if (this->_response != nullptr) {
    	delete this->_response;
        this->_response = nullptr;
    }

    this->_response = new ProtonixHTTPFrame(); // TODO: add support of handling the requested URL
    this->_response->Debug(this->_debug);
    this->_response->UnserializeResponse(response);

	return true;
}

/*ProtonixHTTPClient* ProtonixHTTPClient::OverWiFi() {
  	WiFiClient client;

	return new ProtonixHTTPClient(client);
}*/

ProtonixHTTPClient* ProtonixHTTPClient::TimeoutResponse(unsigned short timeout) {
	this->_timeoutResponse = timeout;

	return this;
}

unsigned short ProtonixHTTPClient::TimeoutResponse() {
	return this->_timeoutResponse;
}