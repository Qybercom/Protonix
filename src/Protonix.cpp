#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

#if defined(ESP32)
#include <StreamString.h>
#include <ESP32httpUpdate.h>
#elif defined(ESP8266)
#include <StreamString.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#elif defined(AVR)
#include <avr/wdt.h>
#endif

#include "Common/index.h"

#include "IProtonixDevice.h"
#include "IProtonixProfile.h"
#include "Protonix.h"
#include "ProtonixTimer.h"
#include "ProtonixAction.h"
#include "ProtonixSignal.h"
#include "ProtonixMemory.h"
#include "ProtonixRegistry.h"
#include "ProtonixSensor.h"

#include "Hardware/HBridgeDefault.h"

using namespace Qybercom::Protonix;

Protonix* Protonix::Protonix::_instance;

Protonix::Protonix (IProtonixDevice* device, IProtonixProfile* profile) {
	Random::Init();

	this->_timerUptime = new ProtonixTimer(0);
	this->_timerTick = new ProtonixTimer(0);
	this->_timerInitDelay = new ProtonixTimer(0);

	this->_ready = false;

	this->Debug(false);

	this->_formats.Add(new Qybercom::Formats::FormatJSON());

	this->_memory = new ProtonixMemory();
	this->_memory->EEPROMBegin();
	this->_registry = new ProtonixRegistry(this->_memory);
	this->_registry->Debug(this->_debug); // TODO: runtime switch for every debuggable component

	this->Hardware("", new Hardware::HBridgeDefault());

	this->_networkDefault = nullptr;

	this->_active = true;

	#if defined(ARDUINO_ARCH_ESP32)
		// @note - seems like on average board the result is always generic `esp32`
		#if defined(CONFIG_IDF_TARGET_ESP32)
			this->_platform = "esp32";
		#elif defined(CONFIG_IDF_TARGET_ESP32S2)
			this->_platform = "esp32_s2";
		#elif defined(CONFIG_IDF_TARGET_ESP32S3)
			this->_platform = "esp32_s3";
		#elif defined(CONFIG_IDF_TARGET_ESP32C3)
			this->_platform = "esp32_c3";
		#elif defined(CONFIG_IDF_TARGET_ESP32C6)
			this->_platform = "esp32_c6";
		#elif defined(CONFIG_IDF_TARGET_ESP32H2)
			this->_platform = "esp32_h2";
		#else
			this->_platform = "esp32";
		#endif
	#elif defined(ARDUINO_ARCH_ESP8266)
		this->_platform = "esp8266";
	#elif defined(ARDUINO_ARCH_AVR)
		#if defined(ARDUINO_AVR_UNO)
			this->_platform = "arduino_uno";
		#elif defined(ARDUINO_AVR_NANO)
			this->_platform = "arduino_nano";
		#elif defined(ARDUINO_AVR_MEGA2560)
			this->_platform = "arduino_mega";
		#elif defined(ARDUINO_AVR_LEONARDO)
			this->_platform = "arduino_leonardo";
		#elif defined(__AVR_ATmega328P__)
			this->_platform = "arduino_atmega328p";
		#else
			this->_platform = "arduino_avr";
		#endif
	#elif defined(ARDUINO_ARCH_SAM)
		this->_platform = "arduino_due";
	#elif defined(ARDUINO_ARCH_SAMD)
		this->_platform = "arduino_samd";
	#elif defined(ARDUINO_ARCH_RP2040)
		this->_platform = "rp2040";
	#else
		this->_platform = "";
	#endif

	String month = String(__DATE__).substring(0, 3);

	if (month == "Jan") month = "01";
	if (month == "Feb") month = "02";
	if (month == "Mar") month = "03";
	if (month == "Apr") month = "04";
	if (month == "May") month = "05";
	if (month == "Jun") month = "06";
	if (month == "Jul") month = "07";
	if (month == "Aug") month = "08";
	if (month == "Sep") month = "09";
	if (month == "Oct") month = "10";
	if (month == "Nov") month = "11";
	if (month == "Dec") month = "12";

	this->_build = ""
		+ String(__DATE__).substring(7, 11) + "-"
		+ month + "-"
		+ String(__DATE__).substring(4, 6) + " "
		+ String(__TIME__);

	this->_state = "";
	this->_firmware = "";
	this->_summary = "";

	this->Profile(profile);
	this->Device(device);

	_instance = this;
}

void Protonix::_interruptProcess () {
	for (IProtonixHardware* hardware : this->_hardware)
		hardware->HardwarePipeInterrupt(this);
}

void Protonix::_interrupt () {
	_instance->_interruptProcess();
}

#if defined(ESP32)
Protonix* Protonix::_dedicateTask (TaskHandle_t* handle, unsigned short core, unsigned short priority) {
	xTaskCreatePinnedToCore(Protonix::_dedicatedTask, "DedicatedTask", 4096, this, priority, handle, core);

	return this;
}

void Protonix::_dedicatedTask (void* param) {
	Protonix* device = (Protonix*)param;
	bool run = true;
	unsigned short core = xPortGetCoreID();
	IProtonixHardwareContext* context = nullptr;

	while (run) {
		for (IProtonixHardware* hardware : device->_hardware) {
			if (hardware->HardwareDedicatedCore() != core) continue;

			context = hardware->HardwareContext();
			if (context != nullptr)
				context->HardwareContextApply(hardware);

			hardware->HardwarePipe(device, core);
		}

		device->_device->DeviceOnDedicatedTask(device, core);

		vTaskDelay(10 / portTICK_PERIOD_MS);
	}

	vTaskDelete(NULL);
}
#endif



Protonix* Protonix::Init (IProtonixDevice* device, IProtonixProfile* profile) {
	return new Protonix(device, profile);
}

Protonix* Protonix::Instance () {
	return _instance;
}

Protonix* Protonix::Device (IProtonixDevice* device) {
	this->_device = device;
	this->_timerTick->Interval(this->_device->DeviceTick());

	return this;
}

IProtonixDevice* Protonix::Device () {
	return this->_device;
}

Protonix* Protonix::Profile (IProtonixProfile* profile) {
	this->_profile = profile;

	return this;
}

IProtonixProfile* Protonix::Profile () {
	return this->_profile;
}

Protonix* Protonix::Debug (bool debug) {
	this->_debug = debug;

	return this;
}

bool Protonix::Debug () {
	return this->_debug;
}

Protonix* Protonix::Active (bool active) {
	this->_active = active;

	return this;
}

bool Protonix::Active () {
	return this->_active;
}

String Protonix::Platform () {
	return this->_platform;
}

String Protonix::Build () {
	return this->_build;
}

Protonix* Protonix::State (String state) {
	this->_state = state;

	return this;
}

String Protonix::State () {
	return this->_state;
}

Protonix* Protonix::Firmware (String firmware) {
	this->_firmware = firmware;

	return this;
}

String Protonix::Firmware () {
	return this->_firmware;
}

Protonix* Protonix::InterruptAttach (unsigned short pin, int mode) {
	attachInterrupt(digitalPinToInterrupt(pin), _interrupt, mode);

	return this;
}

Protonix* Protonix::InterruptDetach (unsigned short pin) {
	detachInterrupt(digitalPinToInterrupt(pin));

	return this;
}



Protonix* Protonix::Pipe () {
	this->_timerUptime->Pipe();

	if (!this->_ready && this->_timerInitDelay->Pipe()) {
		//Serial.print("[ready.1]"); Serial.println(ESP.getFreeHeap());
		this->_ready = true;
		this->_timerInitDelay->Enabled(false);

		this->_registry->Load();

		bool i2c = false;
		bool spi = false;
		#if defined(ESP32)
		short core = -1;
		#endif

		for (IProtonixHardware* hardware : this->_hardware) {
			hardware->HardwareInitPre(this);

			#if defined(ESP32)
			core = hardware->HardwareDedicatedCore();
			if (core == 0) this->DedicateTaskCore0();
			if (core == 1) this->DedicateTaskCore1();
			#endif

			i2c |= hardware->HardwareI2C();
			spi |= hardware->HardwareSPI();
		}

		if (i2c) {
			for (IProtonixHardware* hardware : this->_hardware)
				hardware->HardwareI2CPre(this);

			Wire.begin();

			for (IProtonixHardware* hardware : this->_hardware)
				hardware->HardwareI2CPost(this);
		}

		if (spi) {
			for (IProtonixHardware* hardware : this->_hardware)
				hardware->HardwareSPIPre(this);

			SPI.begin();

			for (IProtonixHardware* hardware : this->_hardware)
				hardware->HardwareSPIPost(this);
		}

		for (IProtonixHardware* hardware : this->_hardware)
			hardware->HardwareInitPost(this);

		//Serial.print("[ready.2]"); Serial.println(ESP.getFreeHeap());
		this->_device->DeviceOnReady(this);
	}

	for (IProtonixNetworkDriver* network : this->_networks)
		network->NetworkDriverPipe(this);

	//Serial.print("[hw.1]"); Serial.println(ESP.getFreeHeap());
	IProtonixHardwareContext* context = nullptr;
	for (IProtonixHardware* hardware : this->_hardware) {
		#if defined(ESP32)
		if (hardware->HardwareDedicatedCore() != -1) continue;
		#endif

		context = hardware->HardwareContext();
		if (context != nullptr)
			context->HardwareContextApply(hardware);

		hardware->HardwarePipe(this, -1);
	}
	//Serial.print("[hw.2]"); Serial.println(ESP.getFreeHeap());

	//Serial.print("[profile.1]"); Serial.println(ESP.getFreeHeap());
	if (this->_profile != nullptr)
		this->_profile->ProfilePipe(this);
	//Serial.print("[profile.2]"); Serial.println(ESP.getFreeHeap());

	//Serial.print("[device.loop.1]"); Serial.println(ESP.getFreeHeap());
	this->_device->DeviceOnLoop(this);
	//Serial.print("[device.loop.2]"); Serial.println(ESP.getFreeHeap());

	if (this->_signals.Count() != 0) {
		//Serial.print("[signals.1]"); Serial.println(ESP.getFreeHeap());
		for (ProtonixSignal* signal : this->_signals) {
			//Serial.println("[debug] " + signal->From() + ":" + signal->ID());

			this->_device->DeviceOnSignal(this, signal);

			delete signal;
			signal = nullptr;
		}

		this->_signals.Clear();
		//Serial.print("[signals.1]"); Serial.println(ESP.getFreeHeap());
	}

	if (this->_timerTick->Pipe()) {
		//Serial.print("[device.tick.1]"); Serial.println(ESP.getFreeHeap());
		this->_device->DeviceOnTick(this);
		//Serial.print("[device.tick.2]"); Serial.println(ESP.getFreeHeap());
	}

	if (this->_actionQueue.Count() != 0) {
		//Serial.print("[actiob.queue.1]"); Serial.println(ESP.getFreeHeap());
		ProtonixAction* action = this->Action(this->_actionQueue.First());

		if (action != nullptr) {
			if (!action->Queued()) {
				action->Queued(true);
				action->Start();
			}

			this->ActionPipe(action);

			if (action->Completed()) {
				action->Reset();

				if (this->_actionQueue.Count() != 0)
					this->_actionQueue.PopFirst();
			}
		}
		//Serial.print("[action.queue.2]"); Serial.println(ESP.getFreeHeap());
	}

	for (ProtonixAction* action : this->_actions) {
		if (action->Queued()) continue;

		this->ActionPipe(action);
	}

	return this;
}

bool Protonix::Reboot () {
	//Serial.println("[debug] reboot");
	#if defined(ESP32) || defined(ESP8266)
		ESP.restart();

		return true;
	#elif defined(AVR)
		// https://forum.arduino.cc/t/soft-reset-and-arduino/367284/6
		wdt_disable();
		wdt_enable(WDTO_15MS);
		while (1) {}

		return true;
	#else
		return false;
	#endif
}

Protonix* Protonix::Reset (bool actions) {
	for (IProtonixHardware* hardware : this->_hardware)
		hardware->HardwareOnReset(this);

	if (actions)
		this->ActionReset();

	return this->State("")->SensorReset();
}

bool Protonix::FirmwareUpdateOTA (String version, String network) {
	IProtonixNetworkDriver* driver = this->Network(network);
	if (driver == nullptr) return false;

	IProtonixNetworkClient* client = driver->NetworkDriverAllocateClient();
	if (client == nullptr) return false;

	bool out = false;

	#if defined(ESP32)
		t_httpUpdate_return result = ESPhttpUpdate.update(this->_profile->ProfileFirmwareURI(this, "esp32", version));

		out = result == HTTP_UPDATE_OK;
	#elif defined(ESP8266)
		String uri = this->_profile->ProfileFirmwareURI(this, "esp8266", version);
		WiFiClient clientWiFi;
		t_httpUpdate_return result = ESPhttpUpdate.update(clientWiFi, uri);

		out = result == HTTP_UPDATE_OK;
	#else
		(void)version;

		out = false;
	#endif

	delete client;
	client = nullptr;

	return out;
}



Protonix* Protonix::Summary (String additional) {
	this->_summary = String("")
		 + "[uptime:" + this->_timerUptime->RunTime() + "] "
		 + "[memory:" + String(this->_memory->RAMFree()) + "] "
		 + "[active:" + String(this->_active ? "yes" : "no") + "] "
		 + "[state:" + this->_state + "]";

	for (ProtonixSensor* sensor : this->_sensors)
		this->_summary = this->_summary + " " + sensor->Summary();

	this->_summary = this->_summary + additional;

	return this;
}

String Protonix::Summary () {
	return this->_summary;
}

unsigned int Protonix::Tick () {
	return this->_device->DeviceTick();
}

bool Protonix::Ready () {
	return this->_ready;
}

ProtonixTimer* Protonix::TimerUptime () {
	return this->_timerUptime;
}

ProtonixTimer* Protonix::TimerTick () {
	return this->_timerTick;
}

ProtonixTimer* Protonix::TimerInitDelay () {
	return this->_timerInitDelay;
}

unsigned long Protonix::CPUFrequency () {
	#if defined(ESP32)
	return getCpuFrequencyMhz();
	#else
	return 0;
	#endif
}

ProtonixMemory* Protonix::Memory () {
	return this->_memory;
}

ProtonixRegistry* Protonix::Registry () {
	return this->_registry;
}

Qybercom::Value Protonix::Registry (const String &key) {
	return this->_registry->Data().Get(key);
}

Protonix* Protonix::Registry (const String &key, const Qybercom::Value &value) {
	this->_registry->Data().Set(key, value);

	return this;
}



Qybercom::List<IProtonixHardware*> &Protonix::Hardware () {
	return this->_hardware;
}

IProtonixHardware* Protonix::Hardware (String id) {
	for (IProtonixHardware* hardware : this->_hardware)
		if (hardware->HardwareID() == id) return hardware;

	return nullptr;
}

Qybercom::Value &Protonix::Hardware (String id, IProtonixHardware* hardware, bool allowSignal) {
	return this->HardwareOnBridge("", id, hardware, allowSignal);
}

Qybercom::Value &Protonix::HardwareOnBridge (String bridge, String id, IProtonixHardware* hardware, bool allowSignal) {
	// TODO: add check for existent id

	hardware->HardwareID(id);
	hardware->HardwareBridge((IProtonixBridge*)this->Hardware(bridge));
	hardware->HardwareContext(nullptr);

	this->_hardware.Add(hardware);

	return hardware->HardwareConfig();
}

bool Protonix::HardwareContext (String hardwareID, String contextID, const Qybercom::Value &data) {
	IProtonixHardware* hardware = this->Hardware(hardwareID);
	if (hardware == nullptr) return false;

	IProtonixHardwareContext* context = (IProtonixHardwareContext*)this->Hardware(contextID);
	if (context == nullptr) return false;

	context->HardwareContextData(hardwareID, data);
	hardware->HardwareContext(context);

	return true;
}



bool Protonix::BusSend (String bus, String data) {
	IProtonixBus* hardware = (IProtonixBus*)this->Hardware(bus);

	return hardware == nullptr ? false : hardware->BusSend(this, data);
}

bool Protonix::BusCommand (String bus, String command) {
	IProtonixBus* hardware = (IProtonixBus*)this->Hardware(bus);

	return hardware == nullptr ? false : hardware->BusCommand(this, command);
}

bool Protonix::BusCommandCustom (String bus, String command) {
	return this->BusCommand(bus, "custom:" + command);
}

bool Protonix::BusCommandSensor (String bus, String id, String value, bool active, bool failure, String state) {
	return this->BusCommand(bus, "std:sensor:" + id
		+ ";" + value
		+ ";" + String(active ? 1 : 0)
		+ ";" + String(failure ? 1 : 0)
		+ String(state == "" ? "" : ";" + state)
	);
}



List<IProtonixNetworkDriver*> &Protonix::Networks () {
	return this->_networks;
}

Protonix* Protonix::Network (IProtonixNetworkDriver* network) {
	if (this->_networks.Count() == 0)
		this->_networkDefault = network;

	this->_networks.Add(network);

	return this;
}

IProtonixNetworkDriver* Protonix::Network (String name) {
	if (name == "")
		return this->_networkDefault;

	for (IProtonixNetworkDriver* network : this->_networks)
		if (network->NetworkDriverName() == name) return network;

	return nullptr;
}

IProtonixNetworkDriver* Protonix::NetworkDefault () {
	return this->_networkDefault;
}

IProtonixNetworkDriver* Protonix::NetworkDefault (String name) {
	IProtonixNetworkDriver* network = this->Network(name);

	if (network != nullptr)
		this->_networkDefault = network;

	return network;
}



List<Qybercom::IValueFormat*> &Protonix::Formats () {
	return this->_formats;
}

Qybercom::IValueFormat* Protonix::Format (String mime) {
	for (Qybercom::IValueFormat* format : this->_formats)
		if (format->ValueFormatMIME() == mime) return format;

	return nullptr;
}



List<ProtonixSignal*> &Protonix::Signals () {
	return this->_signals;
}

Protonix* Protonix::Signal (ProtonixSignal* signal) {
	this->_signals.Add(signal);

	return this;
}

ProtonixSignal* Protonix::Signal (String id) {
	ProtonixSignal* signal = new ProtonixSignal(id);

	this->Signal(signal);

	return signal;
}

ProtonixSignal* Protonix::Signal (String from, String id) {
	ProtonixSignal* signal = new ProtonixSignal(from, id);

	this->Signal(signal);

	return signal;
}

bool Protonix::SignalSpawned (String id) {
	if (this->_signals.Count() == 0) return false;

	for (ProtonixSignal* signal : this->_signals)
		if (signal->Is(id)) return true;

	return false;
}

bool Protonix::SignalSpawned (String from, String id) {
	if (this->_signals.Count() == 0) return false;

	for (ProtonixSignal* signal : this->_signals)
		if (signal->Is(from, id)) return true;

	return false;
}



Qybercom::List<ProtonixAction*> &Protonix::Actions () {
	return this->_actions;
}

ProtonixAction* Protonix::Action (String name) {
	for (ProtonixAction* action : this->_actions)
		if (action->Name() == name) return action;

	return nullptr;
}

ProtonixAction* Protonix::ActionRegister (ProtonixAction* action) {
	this->_actions.Add(action);

	return action;
}

ProtonixAction* Protonix::ActionRegister (String name) {
	return this->ActionRegister(new ProtonixAction(name));
}

ProtonixAction* Protonix::ActionRegister (String name, unsigned int interval) {
	return this->ActionRegister(new ProtonixAction(name, interval));
}

ProtonixAction* Protonix::ActionRegister (String name, unsigned int interval, int stepEnd) {
	return this->ActionRegister(new ProtonixAction(name, interval, stepEnd));
}

ProtonixAction* Protonix::ActionRegister (String name, unsigned int interval, int stepBegin, int stepEnd) {
	return this->ActionRegister(new ProtonixAction(name, interval, stepBegin, stepEnd));
}

ProtonixAction* Protonix::ActionRegister (String name, unsigned int interval, int stepBegin, int stepEnd, int step) {
	return this->ActionRegister(new ProtonixAction(name, interval, stepBegin, stepEnd, step));
}

bool Protonix::ActionTrigger (String name) {
	ProtonixAction* action = this->Action(name);
	if (action == nullptr) return false;

	if (this->_debug)
		Serial.println("[action] Triggered '" + name + "' (" + String(action->Summary()) + ")");

	this->_actionQueue.Add(name);

	return true;
}

bool Protonix::ActionStart (String name) {
	ProtonixAction* action = this->Action(name);
	if (action == nullptr) return false;

	if (this->_debug)
		Serial.println("[action] Started '" + name + "' (" + String(action->Summary()) + ")");

	action->Start();

	return true;
}

bool Protonix::ActionPlay (String name) {
	ProtonixAction* action = this->Action(name);
	if (action == nullptr) return false;

	if (this->_debug)
		Serial.println("[action] Played '" + name + "' (" + String(action->Summary()) + ")");

	action->Play();

	return true;
}

bool Protonix::ActionPause (String name) {
	ProtonixAction* action = this->Action(name);
	if (action == nullptr) return false;

	if (this->_debug)
		Serial.println("[action] Paused '" + name + "' (" + String(action->Summary()) + ")");

	action->Pause();

	return true;
}

bool Protonix::ActionStop (String name) {
	ProtonixAction* action = this->Action(name);
	if (action == nullptr) return false;

	if (this->_debug)
		Serial.println("[action] Stopped '" + name + "' (" + String(action->Summary()) + ")");

	action->Stop();

	return true;
}

bool Protonix::ActionPipe (ProtonixAction* action) {
	if (action == nullptr) return false;
	if (!action->PipePre()) return false;

	this->_device->DeviceOnAction(this, action);

	action->PipePost();

	return true;
}

Protonix* Protonix::ActionReset () {
	ProtonixAction* action = nullptr;

	for (String act : this->_actionQueue) {
		action = this->Action(act);

		if (action != nullptr)
			action->Reset();
	}

	this->_actionQueue.Clear();

	return this;
}



Qybercom::List<ProtonixSensor*> &Protonix::Sensors () {
	return this->_sensors;
}

Protonix* Protonix::SensorAdd (String id) {
	this->_sensors.Add(new ProtonixSensor(id));

	return this;
}

Protonix* Protonix::SensorSet (String id, String value) {
	for (ProtonixSensor* sensor : this->_sensors) {
		if (id == "" || sensor->ID() == id) {
			sensor->Value(value);
		}
	}

	return this;
}

Protonix* Protonix::SensorSet (String id, String value, bool active) {
	for (ProtonixSensor* sensor : this->_sensors) {
		if (id == "" || sensor->ID() == id) {
			sensor->Value(value);
			sensor->Active(active);
		}
	}

	return this;
}

Protonix* Protonix::SensorSet (String id, String value, bool active, bool failure) {
	for (ProtonixSensor* sensor : this->_sensors) {
		if (id == "" || sensor->ID() == id) {
			sensor->Value(value);
			sensor->Active(active);
			sensor->Failure(failure);
		}
	}

	return this;
}

Protonix* Protonix::SensorSet (String id, String value, bool active, bool failure, String state) {
	for (ProtonixSensor* sensor : this->_sensors) {
		if (id == "" || sensor->ID() == id) {
			sensor->Value(value);
			sensor->Active(active);
			sensor->Failure(failure);
			sensor->State(state);
		}
	}

	return this;
}

Protonix* Protonix::SensorSet (String id, bool active) {
	for (ProtonixSensor* sensor : this->_sensors) {
		if (id == "" || sensor->ID() == id) {
			sensor->Active(active);
		}
	}

	return this;
}

Protonix* Protonix::SensorSet (String id, bool active, bool failure) {
	for (ProtonixSensor* sensor : this->_sensors) {
		if (id == "" || sensor->ID() == id) {
			sensor->Active(active);
			sensor->Failure(failure);
		}
	}

	return this;
}

Protonix* Protonix::SensorSet (String id, bool active, bool failure, String state) {
	for (ProtonixSensor* sensor : this->_sensors) {
		if (id == "" || sensor->ID() == id) {
			sensor->Active(active);
			sensor->Failure(failure);
			sensor->State(state);
		}
	}

	return this;
}

Protonix* Protonix::SensorSet (String id, String value, String state) {
	for (ProtonixSensor* sensor : this->_sensors) {
		if (id == "" || sensor->ID() == id) {
			sensor->Value(value);
			sensor->State(state);
		}
	}

	return this;
}

Protonix* Protonix::SensorSet (ProtonixSensor* sensor) {
	return this->SensorSet(
		sensor->ID(),
		sensor->Value(),
		sensor->Active(),
		sensor->Failure(),
		sensor->State()
	);
}

Protonix* Protonix::SensorReset (String id) {
	for (ProtonixSensor* sensor : this->_sensors) {
		if (id == "" || sensor->ID() == id) {
			sensor->Reset();
		}
	}

	return this;
}

Protonix* Protonix::SensorReset () {
	return this->SensorReset("");
}

ProtonixSensor* Protonix::Sensor (String id) {
	for (ProtonixSensor* sensor : this->_sensors)
		if (sensor->ID() == id) return sensor;

	return nullptr;
}



bool Protonix::Command (String command, IProtonixHardware* hardware) {
	ProtonixCommand cmd;

	cmd.ValueTypeDeserialize(command);
	cmd.Hardware(hardware);

	this->_device->DeviceHandleCommand(this, cmd);

	return true;
}



#if defined(ESP32)
Protonix* Protonix::DedicateTaskCore0 () {
	if (_dedicatedHandleCore0 == nullptr)
		this->_dedicateTask(&_dedicatedHandleCore0, 0);

	return this;
}

Protonix* Protonix::DedicateTaskCore1 () {
	if (_dedicatedHandleCore1 == nullptr)
		this->_dedicateTask(&_dedicatedHandleCore1, 1);

	return this;
}
#endif