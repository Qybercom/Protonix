#pragma once

#include <Arduino.h>

#include "Common/KeyValuePair.hpp"

namespace Qybercom {
	namespace Protonix {
		class ProtonixSignal {
			private:
				String _id;
				String _from;
				bool _valueBool;
				int _valueInt;
				float _valueFloat;
				String _valueString;
				KeyValuePair* _valueKV;
				void* _valueAny;

			public:
				ProtonixSignal (String id);
				ProtonixSignal (String from, String id);

				String ID ();
				ProtonixSignal* ID (String id);

				String From ();
				ProtonixSignal* From (String from);

				bool Is (String id);
				bool Is (String from, String id);

				bool ValueBool ();
				ProtonixSignal* ValueBool (bool value);

				int ValueInt ();
				ProtonixSignal* ValueInt (int value);

				float ValueFloat ();
				ProtonixSignal* ValueFloat (float value);

				String ValueString ();
				ProtonixSignal* ValueString (String value);

				KeyValuePair* ValueKV ();
				ProtonixSignal* ValueKV (KeyValuePair* value);

				void* ValueAny ();
				ProtonixSignal* ValueAny (void* value);

				~ProtonixSignal ();
		};
	}
}