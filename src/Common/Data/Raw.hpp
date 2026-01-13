#pragma once

#include <Arduino.h>

namespace Qybercom {
	class Raw {
		private:
			void* _ptr;

		public:
			Raw () : _ptr(0) { }
			explicit Raw (void* ptr) : _ptr(ptr) { }

			void* Get () const { return _ptr; }
			bool Valid () const { return _ptr != 0; }
	};
}