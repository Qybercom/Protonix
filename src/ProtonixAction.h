#pragma once

#include <Arduino.h>

#include "ProtonixTimer.h"

namespace Qybercom {
	namespace Protonix {
		class ProtonixAction {
			protected:
				ProtonixTimer* _timer;
				String _name;
				int _step;
				int _stepBegin;
				int _stepEnd;
				int _stepDirection;
				int _cursor;
				bool _completed;
				bool _infinite;

				void _init (String name, unsigned int interval, int stepBegin, int stepEnd, int step);

			public:
				ProtonixAction (String name);
				ProtonixAction (String name, unsigned int interval);
				ProtonixAction (String name, unsigned int interval, int stepEnd);
				ProtonixAction (String name, unsigned int interval, int stepBegin, int stepEnd);
				ProtonixAction (String name, unsigned int interval, int stepBegin, int stepEnd, int step);

				ProtonixTimer* Timer ();

				void Name (String name);
				String Name ();

				void Step (int step);
				int Step ();

				void StepBegin (int step);
				int StepBegin ();

				void StepEnd (int step);
				int StepEnd ();

				int StepDirection ();

				int Cursor ();

				bool Completed ();

				bool Infinite ();
				ProtonixAction* Infinite (bool infinite);

				bool PipeStart ();
				bool Pipe ();
				void PipeEnd ();

				void Reset ();
		};
	}
}