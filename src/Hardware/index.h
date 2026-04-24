#pragma once

#include "HBridgeADS1115.h"
#include "HBridgeDefault.h"
#include "HBridgePCF8574.h"
#include "HBuiltInLED.h"
#include "HBusCAN.h"
#include "HBusOneWire.h"
#include "HBusSerial.h"
#include "HButton.h"
#include "HBuzzer.h"
#include "HContextCA954.h"
#include "HDisplayHD44780.h"
#include "HDisplayTM1637.h"
#include "HEncoder.h"
#include "HGenericAnalog.h"
#include "HGenericPWM.h"
#include "HISMBandRFModule.h"
#include "HJoystick.h"
#include "HKeypad.h"
#include "HPhotonix.h"
#include "HPhotoResistor.h"
#include "HPlayerMP3.h"
#include "HPotentiometer.h"
#include "HReaderNFC.h"
#include "HRelay.h"
#include "HSelector3P.h"
#include "HSensorFlowYFB5.h"
#include "HSensorHall.h"
#include "HSensorHall2D.h"
#include "HSensorMicrophone.h"
#include "HSensorOrientationMPU6050.h"
#include "HSensorTemperatureDS18.h"
#include "HSensorWeightHX711.h"
#include "HTouchButton.h"
#include "HTrigger.h"
/*
 * awaiting implementation
 *
 * MCP23017 (https://github.com/blemasle/arduino-mcp23017)
 * servo motor (Stepper.h) (check HGenericPWM)
 * gas/particle sensor
 * audio output (i2s output)
 * SD card reader
 * camera
 * audio sensor
 * infrared sensor/trigger/bus (PIR)
 * bluetooth (maybe bus)
 * spi bus
 * i2c bus
 * usb bus
 * gps
 * gsm
 * phone dialer (rotary/keypad)
 * ultrasonic sensor
 * ...
 *
 * recognizing UX gestures (double-click etc. for buttons, touch etc.)
 */