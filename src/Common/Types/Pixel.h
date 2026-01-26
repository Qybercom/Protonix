#pragma once

#include <Arduino.h>

#include "Position.h"
#include "Color.h"

namespace Qybercom {
	namespace Types {
		struct Pixel {
			Qybercom::Types::Position Position;
			Qybercom::Types::Color Color;

			Pixel ();
			Pixel (Qybercom::Types::Position position);
			Pixel (Qybercom::Types::Color color);
			Pixel (Qybercom::Types::Position position, Qybercom::Types::Color color);
		};
	}
}