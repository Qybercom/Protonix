#pragma once

#include <Arduino.h>



#include "Common/List.hpp"
#include "Common/Utils.hpp"

#include "Protonix.h"

#include "ProtonixDevice.h"
#include "ProtonixGenericDevice.h"

#include "Hardware/HButton.h"
#include "Hardware/HBuzzer.h"
#include "Hardware/HEncoder.h"
#include "Hardware/HMesonix.h"
#include "Hardware/HPhotonix.h"
#include "Hardware/HPhotoResistor.h"
#include "Hardware/HPlayerMP3.h"
#include "Hardware/HPotentiometer.h"
#include "Hardware/HRelay.h"
#include "Hardware/HSelector3P.h"
#include "Hardware/HSensorHall.h"
#include "Hardware/HSensorHall2D.h"
#include "Hardware/HSensorMicrophone.h"
#include "Hardware/HTouchButton.h"
#include "Hardware/HTrigger.h"
/*
 * awaiting implementation
 *
 * keypad 3x4, 4x4 (IOXhop_I2CKeypad.h or Keypad.h)
 * 7seg
 * clock7seg (https://github.com/AKJ7/TM1637)
 * led text display (LiquidCrystal_I2C.h)
 * velostat
 * weight (HX711)
 * canBus (MCP2515)
 * i2cExpander (pcf8574)
 * radio module (RCSwitch.h)
 * servo motor (Stepper.h)
 * ...
 *
 * button switch handling - not working for the moment
 * recognizing UX gestures (double-click etc. for buttons, touch etc.)
 */

#include "Command/CCustom.h"
#include "Command/CStdFirmware.h"
#include "Command/CStdOff.h"
#include "Command/CStdOn.h"
#include "Command/CStdReboot.h"
#include "Command/CStdRegistry.h"
#include "Command/CStdSensor.h"

#if defined(ESP32) || defined(ESP8266)
#include "Network/NWiFi.h"
#include "Protocols/PWiFiTCP.h"
#include "Protocols/PWebSocket.h"
#include "ProtonixHTTPClient.h"
#endif