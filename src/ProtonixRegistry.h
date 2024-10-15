#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include "ProtonixMemory.h"

//#include "Command/CStdRegistry.h"

//#define PROTONIX_REGISTRY_START 0x0128
#define PROTONIX_REGISTRY_START 0

namespace Qybercom {
	namespace Protonix {
		class ProtonixRegistry {
        	private:
                ProtonixMemory* _memory;
				JsonDocument _buffer;
				String _bufferRaw;
				JsonObject _bufferObj;
                bool _bufferLoaded;
                bool _bufferLoad();
                String _tuple(String key, bool min);
                bool _debug;

            public:
        		enum class ValueType {
            		BOOLEAN,
            		INTEGER,
                	FLOAT,
                	STRING,
                	INTEGER_INTERVAL,
                	FLOAT_INTERVAL
        		};

            	ProtonixRegistry(ProtonixMemory* memory);

                ProtonixRegistry* Debug(bool debug);
                bool Debug();

				/*template<typename T>
				T Get(String key, T defaultValue);*/

				/*template<typename T>
				bool Set(String key, T value);

				template<typename T>
				bool Set(String key, T value, bool commit);*/

				String GetRaw(String key, String defaultValue);
				bool SetRaw(String key, String value, bool commit);

                //bool Set(Command::CStdRegistry* cmd, bool commit);

                int GetIntervalMin(String key, int defaultValue);
                int GetIntervalMax(String key, int defaultValue);
                float GetIntervalMin(String key, float defaultValue);
                float GetIntervalMax(String key, float defaultValue);

                bool Commit();
        };
	}
}