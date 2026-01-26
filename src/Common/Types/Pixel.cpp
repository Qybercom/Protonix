#include <Arduino.h>

#include "Position.h"
#include "Color.h"

#include "Pixel.h"

using namespace Qybercom::Types;

Pixel::Pixel () {
	Position = Qybercom::Types::Position();
	Color = Qybercom::Types::Color();
}

Pixel::Pixel (Qybercom::Types::Position position) {
	Position = position;
	Color = Qybercom::Types::Color();
}

Pixel::Pixel (Qybercom::Types::Color color) {
	Position = Qybercom::Types::Position();
	Color = color;
}

Pixel::Pixel (Qybercom::Types::Position position, Qybercom::Types::Color color) {
	Position = position;
	Color = color;
}