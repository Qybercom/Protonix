#pragma once



#if defined(ARDUINO_ARCH_AVR)
#define ANALOG_MAX 1023
#elif defined(ARDUINO_ARCH_SAM)
#define ANALOG_MAX 4095
#elif defined(ARDUINO_ARCH_SAMD)
#define ANALOG_MAX 1023
#elif defined(ESP8266)
#define ANALOG_MAX 1023
#elif defined(ESP32)
#define ANALOG_MAX 4095
#elif defined(ARDUINO_ARCH_STM32)
#define ANALOG_MAX 4095
#else
#warning "Unknown platform, fallback to 1023"
#define ANALOG_MAX 1023
#endif