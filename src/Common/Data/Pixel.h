#pragma once

#include <Arduino.h>

#include "Position.h"
#include "Color.h"

namespace Qybercom {
	struct Pixel {
		Qybercom::Position Position;
		Qybercom::Color Color;

		Pixel ();
		Pixel (Qybercom::Position position);
		Pixel (Qybercom::Color color);
		Pixel (Qybercom::Position position, Qybercom::Color color);
	};
}