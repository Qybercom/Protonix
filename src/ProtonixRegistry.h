#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

#include "ProtonixMemory.h"

#define PROTONIX_REGISTRY_START 0

namespace Qybercom {
	namespace Protonix {
		struct ProtonixRegistryColor {
			int r;
			int g;
			int b;
		};

		struct ProtonixRegistryList {
			String items[20];
		};

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
					COLOR,
					INTEGER_INTERVAL,
					FLOAT_INTERVAL
				};

				ProtonixRegistry(ProtonixMemory* memory);

				ProtonixRegistry* Debug(bool debug);
				bool Debug();

				String Raw();

				String GetRaw(String key, String defaultValue);
				bool SetRaw(String key, String value, bool commit);

				int GetInt(String key, int defaultValue);
				bool SetInt(String key, int value, bool commit);

				float GetFloat(String key, float defaultValue);
				bool SetFloat(String key, float value, bool commit);

				ProtonixRegistryColor GetColor(String key, String defaultValue);
				bool SetColor(String key, String value, bool commit);
				bool SetColor(String key, ProtonixRegistryColor &color, bool commit);

				ProtonixRegistryList GetList(String key, String defaultValue);
				bool GetListItemExists(String key, String value);
				bool SetList(String key, String value, bool commit);
				bool SetList(String key, ProtonixRegistryList &list, bool commit);

				int GetIntervalMin(String key, int defaultValue);
				int GetIntervalMax(String key, int defaultValue);
				float GetIntervalMin_f(String key, float defaultValue);
				float GetIntervalMax_f(String key, float defaultValue);

				bool Commit();
		};
	}
}