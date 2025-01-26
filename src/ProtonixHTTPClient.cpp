#include <Arduino.h>
#include <Client.h>

#if defined(ESP32) || defined(ESP8266)
#include <StreamString.h>
#include <WiFiClient.h>
#endif

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

int ProtonixHTTPFrame::LengthExpected() {
	ProtonixHTTPHeader* header = this->Header("Content-Length");

	if (header == nullptr) {
		if (this->_debug)
			Serial.println("[debug] ProtonixHTTPFrame::LengthExpected: No 'Vontent-Length' header found");

		return -1;
	}

	return header->Value().toInt();
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
	this->_client = &client;
 */

ProtonixHTTPClient::ProtonixHTTPClient(Client* client) {
	this->_client = client;
	this->_connected = false;
	this->_request = nullptr;
	this->_response = nullptr;
	this->_available = 0;
	this->_received = 0;
	this->_timeoutResponse = 40;
	this->Debug(false);
}

/*ProtonixHTTPClient::ProtonixHTTPClient() {
	this->_connected = false;
	this->_request = nullptr;
	this->_response = nullptr;
	this->_timeoutResponse = 40;
	this->Debug(false);
}*/

bool ProtonixHTTPClient::_responseBatch(String batch) {
	this->_response->Body(batch);

	return true;
}

ProtonixHTTPFrame* ProtonixHTTPClient::_responseReset(String response) {
	if (this->_response != nullptr) {
		delete this->_response;
		this->_response = nullptr;
	}

	this->_response = new ProtonixHTTPFrame(); // TODO: add support of handling the requested URL
	this->_response->Debug(this->_debug);
	this->_response->UnserializeResponse(response);

	return this->_response;
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

	if (this->_client == nullptr) {
		this->_connected = false;

		return false;
	}

	//if (!(*this->_client).connected()) {
	if (!this->_client->connected()) {
		String host = this->_request->URI()->Host();
		unsigned int port = this->_request->URI()->PortConnection();

		if (this->_debug)
			Serial.println("[debug] ProtonixHTTPClient::Send Connect `" + host + "`:`" + port + "`");

		this->_connected = this->_client->connect(host.c_str(), port);

		if (this->_connected) {
			if (this->_debug)
				Serial.println("[debug] ProtonixHTTPClient::Send Connected");
		}
		else {
			Serial.println("[WARNING] ProtonixHTTPClient::Send: can not send request");
		}
	}

	if (this->_connected) {
		//this->_client->setTimeout(60000); // 60 sec
		String request = this->_request->SerializeRequest();

		if (this->_debug) {
			Serial.println("[debug] ProtonixHTTPClient::Send Request:");
			Serial.println(request);
		}

		ok = this->_client->write((uint8_t*)request.c_str(), request.length());
		this->_client->flush();
	}

	return ok;
}

/*
bool ProtonixHTTPClient::Receive() {
	// TODO: schedule for async receiveing for non-blocking other tasks
	if (!this->_connected) return false;
	delay(this->_timeoutResponse);

	String response = "";
	unsigned int length = 0;
	char c;
	bool started = false;

	// TODO: check for headers, for Content-Length, to attempting reading till Content-Length reached

	//while (this->_client->available()) {
	while (!started || c != EOF) {
		started = true;
		c = this->_client->read();
		length++;
		//response += c;
	}

	Serial.println("[debug] HTTP response length " + String(length));

	this->_responseReset(response);

	return true;
}

bool ProtonixHTTPClient::ReceiveHeaders() {
}

 */
/*
bool ProtonixHTTPClient::Receive() {
	if (!this->_connected) return false;
	delay(this->_timeoutResponse);

	Serial.println("[debug] HTTP response 1");

	String response = "";
	String body = "";
	bool received = false;
	bool receivedHeaders = false;
	unsigned int responseLength = 0;
	unsigned int lengthExpected = 0;
	unsigned int lengthCurrent = 0;
	char c = NULL;

	//this->_bodyStream = StreamString();

	while (!received) {
		c = this->_client->read();
		response += c;
		responseLength++;

		if (responseLength > 4 && response.substring(responseLength - 4) == "\r\n\r\n") {
			receivedHeaders = true;
			this->_responseReset(response);
			lengthExpected = this->_response->LengthExpected();
			this->_available = lengthExpected;
			Serial.println("[debug] HTTP response headers");
			//continue;
			break;
		}

		if (receivedHeaders) {
			//body += c;
			Serial.print(c);
			lengthCurrent++;

			if (lengthCurrent == lengthExpected) {
				received = true;
				Serial.println("[debug] HTTP response body:");
				Serial.println(body);
			}
		}
	}

	//Serial.println("[debug] HTTP response 2: `" + String(received) + "`");
	//return false;


	//Serial.println("[debug] HTTP response 3: `" + response + "`");

	/*
	Serial.println("[debug] HTTP response length expected " + String(lengthExpected));

	//return false;
	while (lengthCurrent < lengthExpected) {
		if (this->_client->available()) {
			c = this->_client->read();

		}

		lengthCurrent++;
	}

	Serial.println("[debug] HTTP response length current " + String(lengthCurrent));
	Serial.println("[debug] HTTP response body:");
	Serial.println(body);*

	//return false;
	return true;
}
*/

bool ProtonixHTTPClient::ReceiveHeaders() {
	if (!this->_connected) return false;
	delay(this->_timeoutResponse);

	if (this->_debug)
		Serial.println("[debug] HTTP response headers 1");

	String response = "";
	bool received = false;
	unsigned int responseLength = 0;
	char c;

	this->_available = this->_client->available();
	this->_received = 0;

	if (this->_debug)
		Serial.println("[debug] HTTP response headers 2");

	while (!received) {
		c = this->_client->read();
		response += c;
		responseLength++;

		if (responseLength > 4 && response.substring(responseLength - 4) == "\r\n\r\n") {
			this->_responseReset(response);

			int contentLength = this->_response->LengthExpected();
			if (contentLength > -1) this->_available = contentLength;

			received = true;
			break;
		}
	}

	if (this->_debug)
		Serial.println("[debug] HTTP response headers 3");

	//delay(5); // need for correct buffering
	delay(this->_timeoutResponse);

	return received;
}

String ProtonixHTTPClient::ReceiveBody(int batchSize) {
	if (!this->_connected) return "";

	unsigned int diff = this->_available - this->_received;
	unsigned int available = diff < batchSize ? diff : batchSize;
	if (batchSize < 1) available = this->_available;

	unsigned int i = 0;
	char c;
	String out = "";

	while (i < available) {
		c = this->_client->read();
		out += c;

		this->_received++;

		i++;
	}

	if (batchSize < 1)
		this->_response->Body(out);

	return out;
}

bool ProtonixHTTPClient::ReceiveAvailable() {
	return this->_available > this->_received;
}

/*
//WORKING SOLUTION
bool ProtonixHTTPClient::_receive() {
	if (!this->_connected) return false;
	//delay(this->_timeoutResponse);

	if (this->_debug)
		Serial.println("[debug] HTTP response");

	String response = "";
	bool received = false;
	unsigned int responseLength = 0;
	char c = NULL;

	int available = this->_client->available();

	while (!received) {
		c = this->_client->read();
		response += c;
		responseLength++;

		if (responseLength > 4 && response.substring(responseLength - 4) == "\r\n\r\n") {
			this->_responseReset(response);

			int availableExpected = this->_response->LengthExpected();
			if (availableExpected > -1) available = availableExpected;

			received = true;
			break;
		}
	}

	if (this->_debug)
		Serial.println("[debug] HTTP response headers");

	if (this->_batchSize == 0)
		this->_batchSize = available;

	unsigned int batches = this->_batchSize == 0 ? 1 : ceil((float)available / (float)this->_batchSize);
	unsigned int i = 0;
	unsigned int batchLength = 0;
	String batch = "";

	delay(5); // need for correct buffering

	while (i < batches) {
		batchLength = 0;

		while (batchLength < this->_batchSize) {
			c = this->_client->read();
			batch += c;
			batchLength++;
		}

		if (this->_batchReady == nullptr) this->_response->Body(batch);
		else if (!this->_batchReady(batch)) return false;

		i++;
	}

	this->_client->stop(); // TODO: need for fixing crash delayed request on same TCP connection

	return true;
}

bool ProtonixHTTPClient::Receive() {
	this->_batchSize = 0;
	this->_batchReady = nullptr;

	return this->_receive();
}


bool ProtonixHTTPClient::Receive(unsigned int batchSize, bool(*batchReady)(String)) {
	this->_batchSize = batchSize;
	this->_batchReady = batchReady;

	return this->_receive();
}
*/

/*
int ProtonixHTTPClient::available() {
	Serial.println("[debug] available `" + String(this->_available) + "`");
	return this->_available;
}

int ProtonixHTTPClient::read() {
	int out = this->_client->read();

	//this->_available--;

	return out;
}

size_t ProtonixHTTPClient::readBytes(uint8_t *buffer, size_t length) {
	Serial.println("[debug] attempt to read: `" + String(length) + "` `" + String(this->_available) + "`");
	if (this->_available == 0) return 0;

	size_t out = this->_client->readBytes(buffer, length > this->_available ? this->_available : length);

	this->_available -= length;

	return out;
}

int ProtonixHTTPClient::peek() {
	return this->read();
}
*/

ProtonixHTTPClient* ProtonixHTTPClient::TimeoutResponse(unsigned short timeout) {
	this->_timeoutResponse = timeout;

	return this;
}

unsigned short ProtonixHTTPClient::TimeoutResponse() {
	return this->_timeoutResponse;
}

#if defined(ESP32) || defined(ESP8266)
ProtonixHTTPClient* ProtonixHTTPClient::OverWiFi() {
	return new ProtonixHTTPClient(new WiFiClient());
}
#endif

/*
ProtonixHTTPClient* ProtonixHTTPClient::OverWiFi() {
  	//WiFiClient client;

	return new ProtonixHTTPClient(ProtonixHTTPClient::_clientWiFi);
}
*/

/*
WiFiClient ProtonixHTTPClient::Client() {
	return (*this->_client);
}
*/

/*Stream& ProtonixHTTPClient::ResponseStream() {
	return (*this->_client);
}*/

ProtonixHTTPClient::~ProtonixHTTPClient() {
	delete this->_request;
	this->_request = nullptr;

	delete this->_response;
	this->_response = nullptr;

	if (this->_client != nullptr) {
		delete this->_client;
		this->_client = nullptr;
	}
}