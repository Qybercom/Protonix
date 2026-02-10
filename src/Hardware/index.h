#pragma once

#include "HBridgeADS1115.h"
#include "HBridgeDefault.h"
#include "HBridgePCF8574.h"
#include "HBuiltInLED.h"
#include "HBusCAN.h"
#include "HBusSerial.h"
#include "HButton.h"
#include "HBuzzer.h"
#include "HContextCA958.h"
#include "HEncoder.h"
#include "HGenericAnalog.h"
#include "HGenericPWM.h"
#include "HJoystick.h"
#include "HPhotonix.h"
#include "HPhotoResistor.h"
#include "HPlayerMP3.h"
#include "HPotentiometer.h"
#include "HReaderNFC.h"
#include "HRelay.h"
#include "HSelector3P.h"
#include "HSensorHall.h"
#include "HSensorHall2D.h"
#include "HSensorMicrophone.h"
#include "HTouchButton.h"
#include "HTrigger.h"
#include "HWeightHX711.h"
/*
 * awaiting implementation
 *
 * MCP23017 (https://github.com/blemasle/arduino-mcp23017)
 * keypad 3x4, 4x4 (IOXhop_I2CKeypad.h or Keypad.h)
 * 7seg
 * clock7seg (https://github.com/AKJ7/TM1637)
 * led text display (LiquidCrystal_I2C.h)
 * radio module (RCSwitch.h)
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