#pragma once

#include "Protonix.h"

#include <Arduino.h>

#include "ProtonixDevice.h"
#include "ProtonixGenericDevice.h"

#include "Hardware/HReaderMFRC.h"

#include "Command/CStdOn.h"
#include "Command/CStdOff.h"
#include "Command/CStdReboot.h"
#include "Command/CStdRegistry.h"
#include "Command/CStdSensor.h"
#include "Command/CCustom.h"

#if defined(ESP32) || defined(ESP8266)
#include "ProtonixHTTPClient.h"
#include "Network/NWiFi.h"
#include "Protocols/PWiFiTCP.h"
#include "Protocols/PWebSocket.h"
#endif