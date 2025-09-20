#pragma once

#include <Arduino.h>

#include "_platforms.h"

#include "Common/List.hpp"

#include "IProtonixHardware.h"
#include "IProtonixNetworkDriver.h"
#include "IProtonixCommand.h"
#include "IProtonixDevice.h"
#include "IProtonixProfile.h"
#include "ProtonixTimer.h"
#include "ProtonixAction.h"
#include "ProtonixSignal.h"
#include "ProtonixMemory.h"
#include "ProtonixRegistry.h"
#include "ProtonixSensor.h"

namespace Qybercom {
	namespace Protonix {
		using Qybercom::List;

		class Protonix {
			private:
				static Protonix* _instance;
				Protonix (IProtonixDevice* device, IProtonixProfile* profile = nullptr);

				IProtonixDevice* _device;
				IProtonixProfile* _profile;

				ProtonixTimer* _timerUptime;
				ProtonixTimer* _timerTick;

				bool _ready;
				bool _debug;
				bool _active;
				String _state;
				String _firmware;
				String _summary;

				ProtonixMemory* _memory;
				ProtonixRegistry* _registry;

				List<IProtonixHardware*> _hardware;
				List<IProtonixNetworkDriver*> _networks;
				IProtonixNetworkDriver* _networkDefault;

				List<ProtonixSignal*> _signals;

				List<ProtonixAction*> _actions;
				List<String> _actionQueue;

				List<ProtonixSensor*> _sensors;
				List<IProtonixCommand*> _commands;

				void _interruptProcess ();
				static void QYBERCOM_PROTONIX_INTERRUPT _interrupt ();

				#if defined(ESP32)
				TaskHandle_t _dedicatedHandleCore0 = nullptr;
				TaskHandle_t _dedicatedHandleCore1 = nullptr;

				Protonix* _dedicateTask (TaskHandle_t* handle, unsigned short core, unsigned short priority = 1);
				static void _dedicatedTask (void* param);
				#endif

			public:
				static Protonix* Init (IProtonixDevice* device, IProtonixProfile* profile = nullptr);
				static Protonix* Instance ();

				Protonix* Device (IProtonixDevice* device);
				IProtonixDevice* Device ();

				Protonix* Profile (IProtonixProfile* profile);
				IProtonixProfile* Profile ();

				Protonix* Debug (bool debug);
				bool Debug ();

				Protonix* Active (bool active);
				bool Active ();

				Protonix* State (String state);
				String State ();

				Protonix* Firmware (String firmware);
				String Firmware ();

				Protonix* InterruptAttach (unsigned short pin, int mode);
				Protonix* InterruptDetach (unsigned short pin);

				Protonix* Pipe ();

				bool Reboot ();

				bool FirmwareUpdateOTA (String version = "");

				Protonix* Summary (String additional);
				String Summary ();

				unsigned int Tick ();

				bool Ready ();

				ProtonixTimer* TimerUptime ();
				ProtonixTimer* TimerTick ();

				ProtonixMemory* Memory ();
				ProtonixRegistry* Registry ();

				List<IProtonixHardware*> &Hardware ();
				IProtonixHardware* Hardware (String id);
				Protonix* Hardware (String id, IProtonixHardware* hardware, bool allowSignal = true);
				template<typename T>
				T* Hardware (String id) {
					IProtonixHardware* base = this->Hardware(id);

					return static_cast<T*>(base);
				}

				List<IProtonixNetworkDriver*> &Networks ();
				Protonix* Network (IProtonixNetworkDriver* network);
				IProtonixNetworkDriver* Network (String name);
				IProtonixNetworkDriver* NetworkDefault ();
				IProtonixNetworkDriver* NetworkDefault (String name);

				List<ProtonixSignal*> &Signals ();
				Protonix* Signal (ProtonixSignal* signal);
				ProtonixSignal* Signal (String id);
				ProtonixSignal* Signal (String from, String id);

				List<ProtonixAction*> &Actions ();
				ProtonixAction* Action (String name);
				ProtonixAction* ActionRegister (ProtonixAction* action);
				ProtonixAction* ActionRegister (String name);
				ProtonixAction* ActionRegister (String name, unsigned int interval);
				ProtonixAction* ActionRegister (String name, unsigned int interval, int stepEnd);
				ProtonixAction* ActionRegister (String name, unsigned int interval, int stepBegin, int stepEnd);
				ProtonixAction* ActionRegister (String name, unsigned int interval, int stepBegin, int stepEnd, int step);
				bool ActionTrigger (String name);
				bool ActionStart (String name);
				bool ActionPlay (String name);
				bool ActionPause (String name);
				bool ActionStop (String name);
				bool ActionPipe (ProtonixAction* action);
				Protonix* ActionReset ();

				List<ProtonixSensor*> &Sensors ();
				Protonix* SensorAdd (String id);
				Protonix* SensorSet (String id, String value);
				Protonix* SensorSet (String id, String value, bool active);
				Protonix* SensorSet (String id, String value, bool active, bool failure);
				Protonix* SensorSet (String id, String value, bool active, bool failure, String state);
				Protonix* SensorSet (String id, bool active);
				Protonix* SensorSet (String id, bool active, bool failure);
				Protonix* SensorSet (String id, bool active, bool failure, String state);
				Protonix* SensorSet (String id, String value, String state);
				Protonix* SensorSet (ProtonixSensor* sensor);
				Protonix* SensorReset ();
				Protonix* SensorReset (String id);
				ProtonixSensor* Sensor (String id);

				List<IProtonixCommand*> &Commands ();
				Protonix* CommandAdd (IProtonixCommand* command);
				IProtonixCommand* CommandRecognize (String cmd, IProtonixHardware* hardware = nullptr);
				bool CommandProcess (IProtonixCommand* command, IProtonixHardware* hardware = nullptr);
				bool CommandRecognizeAndProcess (String cmd, IProtonixHardware* hardware = nullptr);

				#if defined(ESP32)
				Protonix* DedicateTaskCore0 ();
				Protonix* DedicateTaskCore1 ();
				#endif
		};
	}
}