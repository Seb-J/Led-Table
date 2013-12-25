/*
================================================================================
File name: due_usart.h
   System: Due TLC5940 Library
 Platform: Arduino Due (Amtel SAM3X8E ARM Cortex-M3)
   Author: Madeline Usher
  Created: July 6, 2013
  Purpose: Declarations useful for working with the SAM3X8E USARTs,
           particularly in translating between Arduino Due pins and Amtel
           library constants.
================================================================================
  $LastChangedDate: 2013-07-07 00:28:29 -0500 (Sun, 07 Jul 2013) $
  $LastChangedBy: maddy314@gmail.com $
================================================================================
  Copyright (c) 2013 Madeline Usher <maddy314 ~@~ gmail.com>
 
  The Arduino Due TLC5940 Library is free software: you can redistribute it
  and/or modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation, either version 3 of the License, 
  or (at your option) any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT 
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with 
  this library.  If not, see <http://www.gnu.org/licenses/>.
================================================================================
*/

#ifndef DUE_TLC5940_USART_h
#define DUE_TLC5940_USART_h

#include "Arduino.h"
#include <inttypes.h>

////////////////////////////////////////////////////////////////////////////////

/*
USART0
  SCLK = SCK0 = PA17, periph B = due pin 70 (SDA1)
  MOSI = TXD0 = PA11, periph A = due pin 18 (TX1)
  MISO = RXD0 = PA10, periph A = due pin 19 (RX1)
  
USART1
  SCLK = SCK1 = PA16, periph A = due pin 54 (analog 0)
  MOSI = TXD1 = PA13, periph A = due pin 16 (TX2)
  MISO = RXD1 = PA12, periph A = due pin 17 (RX2)

USART2
  SCLK = SCK2 = PB24, periph A = no pin
  MOSI = TXD2 = PB20, periph A = due pin 65 (analog 11)
  MISO = RXD2 = PB21, periph A = due pin 52 (digital 52)

USART3
  SCLK = SCK3 = PE16, periph B = no pin
  MOSI = TXD3 = PD4, periph B = due pin 14 (TX3)
  MISO = RXD3 = PD5, periph B = due pin 15 (RX3)
*/

// Only the first two USARTs are defined since those are the only ones that are
// connected to Arduino Due pins.
#define TLC5940_USART0  0
#define TLC5940_USART1  1


typedef struct _USARTDesc
 {
  Usart* usart;
  uint32_t PMC_ID;
  
  Pio* SCLK_PIOport;
  uint32_t SCLK_PIOpin;
  EPioType SCLK_PIOperiph;
  
  Pio* MOSI_PIOport;
  uint32_t MOSI_PIOpin;
  EPioType MOSI_PIOperiph;
  
  Pio* MISO_PIOport;
  uint32_t MISO_PIOpin;
  EPioType MISO_PIOperiph;
 } USARTDesc;

extern const USARTDesc USARTDescs[];


#endif  // DUE_TLC5940_USART_h

////////////////////////////////////////////////////////////////////////////////
/// End of Code
