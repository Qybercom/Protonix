#pragma once

#include <Arduino.h>

#include "ProtonixDevice.h"
#include "ProtonixGenericDevice.h"

#include "Command/CStdOn.h"
#include "Command/CStdOff.h"
#include "Command/CStdSensor.h"
#include "Command/CCustom.h"

#if defined(ESP32) || defined(ESP8266)
#include "Network/NWiFi.h"
#include "Protocols/PWiFiTCP.h"
#include "Protocols/PWebSocket.h"
#endif