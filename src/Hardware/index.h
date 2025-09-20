#pragma once

#include "HBuiltInLED.h"
#include "HBusCAN.h"
#include "HBusSerial.h"
#include "HButton.h"
#include "HBuzzer.h"
#include "HEncoder.h"
#include "HMesonix.h"
#include "HPhotonix.h"
#include "HPhotoResistor.h"
#include "HPlayerMP3.h"
#include "HPotentiometer.h"
#include "HRelay.h"
#include "HSelector3P.h"
#include "HSensorHall.h"
#include "HSensorHall2D.h"
#include "HSensorMicrophone.h"
#include "HTouchButton.h"
#include "HTrigger.h"
/*
 * awaiting implementation
 *
 * keypad 3x4, 4x4 (IOXhop_I2CKeypad.h or Keypad.h)
 * 7seg
 * clock7seg (https://github.com/AKJ7/TM1637)
 * led text display (LiquidCrystal_I2C.h)
 * velostat
 * weight (HX711)
 * i2cExpander (pcf8574)
 * radio module (RCSwitch.h)
 * servo motor (Stepper.h)
 * gas/particle sensor
 * audio output
 * SD card reader
 * camera
 * joystick
 * audio sensor
 * infrared sensor/trigger/bus
 * bluetooth
 * genericAnalog
 * spi bus
 * gps
 * gsm
 * ...
 *
 * button switch handling - not working for the moment
 * recognizing UX gestures (double-click etc. for buttons, touch etc.)
 */