#pragma once

#include <Arduino.h>
//#include <ArduinoJson.h>

#include "Common/index.h"

#include "ProtonixMemory.h"

namespace Qybercom {
	namespace Protonix {
		/*struct ProtonixRegistryColor {
			int r;
			int g;
			int b;
		};

		struct ProtonixRegistryList {
			String items[20];
		};*/

		class ProtonixRegistry {
			private:
				ProtonixMemory* _memory;
				Qybercom::IBucketFormat* _format;
				Qybercom::Bucket _data;
				String _raw;
				bool _loaded;
				bool _debug;

				bool _load ();
				/*JsonDocument _buffer;
				String _bufferRaw;
				JsonObject _bufferObj;*/
				//String _buffer;
				//String _tuple (String key, bool min);

			public:
				/*enum class ValueType {
					BOOLEAN,
					INTEGER,
					FLOAT,
					STRING,
					COLOR,
					INTEGER_INTERVAL,
					FLOAT_INTERVAL
				};*/

				ProtonixRegistry (ProtonixMemory* memory);

				Qybercom::Bucket &Data ();

				ProtonixRegistry* Debug (bool debug);
				bool Debug ();

				String &Raw ();

				/*String GetRaw (String key, String defaultValue = "");
				bool SetRaw (String key, String value, bool commit);

				int GetInt (String key, int defaultValue = 0);
				bool SetInt (String key, int value, bool commit);

				float GetFloat (String key, float defaultValue = 0.0);
				bool SetFloat (String key, float value, bool commit);

				ProtonixRegistryColor GetColor (String key, String defaultValue = "0,0,0");
				bool SetColor (String key, String value, bool commit);
				bool SetColor (String key, ProtonixRegistryColor &color, bool commit);

				// TODO: refactor for `List<T>`
				ProtonixRegistryList GetList (String key, String defaultValue = "");
				bool GetListItemExists (String key, String value);
				short GetListItemIndex (String key, String value);
				bool SetList (String key, String value, bool commit);
				bool SetList (String key, ProtonixRegistryList &list, bool commit);

				int GetIntervalMin (String key, int defaultValue = 0);
				int GetIntervalMax (String key, int defaultValue = 0);
				float GetIntervalMin_f (String key, float defaultValue = 0.0);
				float GetIntervalMax_f (String key, float defaultValue = 0.0);*/

				bool Commit ();
		};
	}
}