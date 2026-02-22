#pragma once

#include <Arduino.h>

#include "Common/Value.h"

//#include "IProtonixHardware.hpp"

namespace Qybercom {
	namespace Protonix {
		class IProtonixHardware;

		class ProtonixCommand : public IValueType {
			private:
				String _name;
				Value _arguments;
				IProtonixHardware* _hardware;

			public:
				ProtonixCommand ();
				ProtonixCommand (const String &name);
				ProtonixCommand (const String &name, const Value &arguments);

				ProtonixCommand &Hardware (IProtonixHardware* hardware);
				IProtonixHardware* Hardware () const;

				String Name () const;
				Value &Arguments ();
				const Value &Arguments() const;
				Value Argument (unsigned int i = 0);
				String Group () const;
				String Command () const;

				bool IsStd () const;
				bool IsCustom () const;

				void Dump () const;

				void ValueTypeDeserialize (const String &raw);
				String ValueTypeSerialize ();
		};
	}
}