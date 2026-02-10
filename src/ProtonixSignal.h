#pragma once

#include <Arduino.h>

#include "Common/index.h"

namespace Qybercom {
	namespace Protonix {
		class ProtonixSignal {
			private:
				String _id;
				String _from;
				Value _data;

			public:
				ProtonixSignal (String id);
				ProtonixSignal (String from, String id);

				String ID ();
				ProtonixSignal* ID (String id);

				String From ();
				ProtonixSignal* From (String from);

				bool Is (String id);
				bool Is (String from, String id);

				Value &Data ();
				ProtonixSignal* Data (Value data);
				template<typename T>
				ProtonixSignal* DataRaw (const T &obj) {
					return Data(Value::Raw(obj));
				}

				~ProtonixSignal ();
		};
	}
}