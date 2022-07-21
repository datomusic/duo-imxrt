/*
  Arduino API main include
  Copyright (c) 2016 Arduino LLC. All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef ARDUINO_H
#define ARDUINO_H

// version 1.0.0
#define ARDUINO_API_VERSION 10000

/* Standard C library includes */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "Binary.h"
// Misc Arduino core functions
#include "Common.h"
#include "WVariant.h"
#include "wiring_analog.h"
#include "pins_arduino.h"

#ifdef __cplusplus
#include "Client.h"
#include "HardwareI2C.h"
#include "HardwareSerial.h"
#include "Uart.h"
#include "USBCDC.h"
#include "Interrupts.h"
#include "IPAddress.h"
#include "Print.h"
#include "Printable.h"
#include "PluggableUSB.h"
#include "Server.h"
#include "WString.h"
#include "Stream.h"
#include "Udp.h"
#include "USBAPI.h"
#include "WCharacter.h"
#endif

#endif
