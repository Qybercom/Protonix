#pragma once

#include <Arduino.h>

#include "Common/Data.hpp"

namespace Qybercom {
	namespace Protonix {
		class ProtonixSignal {
			private:
				String _id;
				String _from;
				Any _value;

			public:
				ProtonixSignal (String id);
				ProtonixSignal (String from, String id);

				String ID ();
				ProtonixSignal* ID (String id);

				String From ();
				ProtonixSignal* From (String from);

				bool Is (String id);
				bool Is (String from, String id);

				Any Value ();
				template<typename T>
			 	T Value () {
			 		return this->_value.As<T>();
			 	}
				ProtonixSignal* Value (Any value);
				template<typename T>
				ProtonixSignal* Value (const T& value) {
					return this->Value(Any(value));
				}

				~ProtonixSignal ();
		};
	}
}