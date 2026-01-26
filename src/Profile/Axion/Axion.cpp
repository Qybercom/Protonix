#include <Arduino.h>

#include "../../Common/index.h"

#include "../../Protonix.h"
#include "../../ProtonixTimer.h"
#include "../../ProtonixNetworkClient.h"

#include "IAxionDevice.h"

#include "Axion.h"

using namespace Qybercom::Protonix;

Profile::Axion::Axion::Axion (String uriStream, String uriHTTP, unsigned int intervalData, unsigned int intervalConnectStream, unsigned int intervalAuthorize) {
	this->_uriStream = uriStream;
	this->_uriHTTP = uriHTTP;

	this->_clientStream = new ProtonixNetworkClient(this->_uriStream);

	this->_timerConnectStream = new ProtonixTimer(intervalConnectStream);
	this->_timerAuthorize = new ProtonixTimer(intervalAuthorize);
	this->_timerData = new ProtonixTimer(intervalData);

	this->_format = nullptr;

	this->_authorized = false;
	this->_autoConnectStream = true;
	this->_autoAuthorize = true;
	this->_autoData = true;
	this->_dataFirst = true;
	#if defined(ESP32) || defined(ESP8266)
	this->_dataMemory = true;
	#else
	this->_dataMemory = false;
	#endif
	this->_dataHardware = false;

	this->_debug = false;
}

void Profile::Axion::Axion::ProfilePipe (Protonix* device) {
	if (this->_format == nullptr)
		this->_format = Protonix::Instance()->Format("application/json");

	if (!this->_clientStream->Connected()) {
		if (this->_autoConnectStream && this->_timerConnectStream->Pipe())
			this->_clientStream->Connect();

		return;
	}

	String raw = this->_clientStream->Receive();
	if (raw != "") {
		if (this->_debug)
			Serial.println("[Axion:ProfilePipe] " + raw);

		String sep = Random::AZaz09(16);
		raw.replace("}{", "}" + sep + "{");

		List<String> cmds = explode(sep, raw);
		String url = "";

		for (String &cmd : cmds) {
			Value dto = Value::Deserialize(this->_format, cmd);
			//dto.Dump();

			if (dto.HasKey("url")) {
				// normally should not occur
			}

			if (dto.HasKey("response")) {
				url = (String)dto["response"];
				int status = dto["data"]["status"];

				if (this->_debug)
					Serial.println("[Axion:OnStreamResponse] " + url);

				if (url.startsWith("/api/authorize/mechanism")) {
					//if (this->_debug)
						Serial.println("[Axion:OnStreamResponse:Authorization] " + String(status));

					this->_authorized = status == 200;
				}

				if (url.startsWith("/api/mechanism/status")) {
					if (this->_debug)
						Serial.println("[Axion:OnStreamResponse:DeviceData] " + String(status));

					if (status == 403)
						this->_authorized = false;
				}
			}

			if (dto.HasKey("event")) {
				url = (String)dto["event"];

				if (this->_debug)
					Serial.println("[Axion:OnStreamEvent] " + url);

				Profile::Axion::IAxionDevice* dev = (Profile::Axion::IAxionDevice*)device->Device();

				if (url.startsWith("/api/mechanism/command/" + dev->AxionDeviceID())) {
					String cmd = dto["data"]["command"];

					if (this->_debug)
						Serial.println("[Axion:OnStreamEvent:Command] " + cmd);

					if (cmd.startsWith("axion:")) {
						String trail = cmd.substring(6, cmd.length());

						Serial.println("[debug] Axion command: " + trail);

						if (trail == "dataMemory:1")
							this->_dataMemory = true;

						if (trail == "dataMemory:0")
							this->_dataMemory = false;

						if (trail == "dataHardware:1")
							this->_dataHardware = true;

						if (trail == "dataHardware:0")
							this->_dataHardware = false;
					}
					else {
						bool ok = device->CommandRecognizeAndProcess(cmd);

						if (this->_debug || !ok)
							Serial.println("[Axion:OnStreamEvent:Command] Result: " + String(ok));
					}
				}
			}
		}
	}

	if (this->_timerData->Pipe()) {
		//Serial.print("[2.3]"); Serial.println(ESP.getFreeHeap());
		if (this->_authorized) {
			if (this->_autoData)
				this->RequestStreamDeviceData(device);
		}
		else {
			if (this->_autoAuthorize && this->_timerAuthorize->Pipe())
				this->RequestStreamAuthorize(device);
		}
	}
}

String Profile::Axion::Axion::ProfileFirmwareURI (Protonix* device, String platform, String version) {
	Profile::Axion::IAxionDevice* dev = (Profile::Axion::IAxionDevice*)device->Device();

	String url = this->_uriHTTP + "/api/mechanism/firmware/" + dev->AxionDeviceID() + "?platform=";
	String ver = version == "" ? "" : String("&version=" + version);

	return url + platform + ver;
}

ProtonixNetworkClient* Profile::Axion::Axion::ClientStream () {
	return this->_clientStream;
}

ProtonixTimer* Profile::Axion::Axion::TimerConnectStream () {
	return this->_timerConnectStream;
}

ProtonixTimer* Profile::Axion::Axion::TimerAuthorize () {
	return this->_timerAuthorize;
}

ProtonixTimer* Profile::Axion::Axion::TimerData () {
	return this->_timerData;
}

bool Profile::Axion::Axion::Authorized () {
	return this->_authorized;
}

bool Profile::Axion::Axion::AutoConnectStream () {
	return this->_autoConnectStream;
}

Profile::Axion::Axion* Profile::Axion::Axion::AutoConnectStream (bool value) {
	this->_autoConnectStream = value;

	return this;
}

bool Profile::Axion::Axion::AutoAuthorize () {
	return this->_autoAuthorize;
}

Profile::Axion::Axion* Profile::Axion::Axion::AutoAuthorize (bool value) {
	this->_autoAuthorize = value;

	return this;
}

bool Profile::Axion::Axion::AutoData () {
	return this->_autoData;
}

Profile::Axion::Axion* Profile::Axion::Axion::AutoData (bool value) {
	this->_autoData = value;

	return this;
}

void Profile::Axion::Axion::RequestStream (Protonix* device, String url, const Value &request) {
	(void)device;

	String error = "";

	if (this->_clientStream->Connected()) {
		Value dto = Value::Object();

		dto["url"] = url;
		dto["data"] = request;

		String raw = dto.Serialize(this->_format);
		//Serial.println("[debug:request] " + raw);

		this->_clientStream->Send(raw);
	}
	else error = "Device not connected, can not send request";

	if (this->_debug && error != "")
		Serial.println("[Axion:RequestStream] Error: " + error);
}

void Profile::Axion::Axion::RequestStreamAuthorize (Protonix* device) {
	Profile::Axion::IAxionDevice* dev = (Profile::Axion::IAxionDevice*)device->Device();

	String id = dev->AxionDeviceID();
	String passphrase = dev->AxionDevicePassphrase();

	//if (this->_debug)
		Serial.println("[Axion:RequestStreamAuthorize] '" + id + "':'" + passphrase + "'");

	Value dto = Value::Object();

	dto["id"] = id;
	dto["passphrase"] = passphrase;

	this->RequestStream(device, "/api/authorize/mechanism", dto);
}

void Profile::Axion::Axion::RequestStreamDeviceData (Protonix* device) {
	if (this->_debug)
		Serial.println("[Axion:RequestStreamDeviceData]");

	//Serial.print("[axion:request:1]"); Serial.println(ESP.getFreeHeap());
	Value dto = Value::Object();
	#if defined(ESP32) || defined(ESP8266)
	bool first = this->_dataFirst;
	if (first) this->_dataFirst = false;
	#else
	bool first = false;
	#endif

	dto["active"] = device->Active();
	dto["platform"] = device->Platform();
	dto["build"] = device->Build();
	dto["state"] = device->State();
	dto["summary"] = device->Summary();

	#if defined(ESP32) || defined(ESP8266)
	dto["cpu"] = device->CPUFrequency();
	dto["uptime"] = device->TimerUptime()->RunTime();
	dto["firmware"] = device->Firmware();

	if (this->_dataMemory) {
		Value memory = Value::Object();

		memory["ram_free"] = device->Memory()->RAMFree();
		//memory["ram_used"] = device->Memory()->RAMUsed();
		memory["ram_total"] = device->Memory()->RAMTotal();
		//memory["ram_fragmented"] = device->Memory()->RAMFragmented();
		memory["flash_free"] = device->Memory()->FlashFree();
		//memory["flash_used"] = device->Memory()->FlashUsed();
		memory["flash_total"] = device->Memory()->FlashTotal();

		dto["memory"] = memory;
	}

	List<ProtonixSensor*> &sensors = device->Sensors();
	Value sensors_out = Value::Array();
	for (ProtonixSensor* sensor : sensors) {
		Value sItem = Value::Object();

		sItem["id"] = sensor->ID();
		sItem["value"] = sensor->Value();
		sItem["active"] = sensor->Active();
		sItem["failure"] = sensor->Failure();
		sItem["state"] = sensor->State();

		sensors_out.Add(sItem);
	}
	dto["sensors"] = sensors_out;

	if (this->_dataHardware || first) {
		List<IProtonixHardware*> &hardware = device->Hardware();
		Value hardware_out = Value::Array();
		for (IProtonixHardware* hw : hardware) {
			Value hwItem = Value::Object();

			hwItem["id"] = hw->HardwareID();
			hwItem["summary"] = hw->HardwareSummary();

			Value capabilities_out = Value::Array();
			List<ProtonixHardwareCapability*> &capabilities = hw->HardwareCapabilities();
			for (ProtonixHardwareCapability* capability : capabilities) {
				Value capabilityItem = Value::Object();

				capabilityItem["kind"] = capability->Kind();
				capabilityItem["id"] = capability->ID();
				capabilityItem["value"] = capability->Value();
				capabilityItem["comment"] = capability->Comment();

				capabilities_out.Add(capabilityItem);
			}
			hwItem["capabilities"] = capabilities_out;

			hardware_out.Add(hwItem);
		}
		dto["hardware"] = hardware_out;
	}

	//dto["registry"] = device->Registry()->Raw();
	#endif
	//Serial.print("[axion:request:2]"); Serial.println(ESP.getFreeHeap());

	this->RequestStream(device, "/api/mechanism/status", dto);
}

bool Profile::Axion::Axion::Debug () {
	return this->_debug;
}

Profile::Axion::Axion* Profile::Axion::Axion::Debug (bool debug) {
	this->_debug = debug;

	return this;
}