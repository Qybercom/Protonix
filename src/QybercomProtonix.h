#pragma once

#include <Arduino.h>

#include "Common/List.hpp"
#include "Common/Utils.hpp"

#include "Protonix.h"

#include "ProtonixDevice.h"
#include "ProtonixGenericDevice.h"

#include "Hardware/HPlayerMP3.h"
#include "Hardware/HReaderMFRC.h"
#include "Hardware/HRelay.h"
#include "Hardware/HSensorHall.h"
#include "Hardware/HSensorMicrophone.h"
#include "Hardware/HSensorPhotoResistor.h"
#include "Hardware/HSensorTouch.h"
#include "Hardware/HSensorTrigger.h"
#include "Hardware/HVisualiserLEDMatrix.h"
#include "Hardware/HVisualiserLEDRibbon.h"

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