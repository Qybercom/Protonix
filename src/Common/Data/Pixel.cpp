#include <Arduino.h>

#include "Position.h"
#include "Color.h"

#include "Pixel.h"

using namespace Qybercom;

Pixel::Pixel () {
	Position = Qybercom::Position();
	Color = Qybercom::Color();
}

Pixel::Pixel (Qybercom::Position position) {
	Position = position;
	Color = Qybercom::Color();
}

Pixel::Pixel (Qybercom::Color color) {
	Position = Qybercom::Position();
	Color = color;
}

Pixel::Pixel (Qybercom::Position position, Qybercom::Color color) {
	Position = position;
	Color = color;
}