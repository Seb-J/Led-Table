/*
================================================================================
File name: tlc5940.h
   System: Arduino Due TLC5940 Library
 Platform: Arduino Due (Amtel SAM3X8E ARM Cortex-M3)
   Author: Madeline Usher
  Created: June 27, 2013
  Purpose: Declarations for code specific to handling the TLC5940.
================================================================================
  $LastChangedDate: 2013-07-14 12:12:19 -0500 (Sun, 14 Jul 2013) $
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

#ifndef DUE_TLC5940_h
#define DUE_TLC5940_h

#include "Arduino.h"
#include <inttypes.h>
#include "due_timer_counters.h"
#include "due_usart.h"
#include "due_tlc5940_config.h"


/*
================================================================================
Setup:

(1) To use this library, copy all the files in this folder into your 
    sketch's folder.
(2) Edit the parameters in due_tlc5940_config.h.
(3) #include "due_tlc5940.h" in your sketch.

The files have to be in your sketch's directory to allow individual 
customization via the #defines in the config file, which is unfortunately
necessary -- the name of the interrupt handler function must be known at
compile-time, which basically means we're dependent upon #defines to 
configure the library.

 
Usage:
 
Call initTLC5940() somewhere early in your sketch (in your setup function,
for example).


You can manipulate grayscale values for TLC5940 output channels with the 
following functions.  The grayscale values range from 0 (completely off) 
through 4095 (completely on).
- setGSData(<outputChannel>, <value>)
- setAllGSData(<value>)
- getGSData(<outputChannel>)

Grayscale data is not sent to the TLC5940 until you call sendGSData().


The following functions are used for dot-correction values.  The dot-
correction values range from 0 (no output is allowed on that channel)
through 63 (maxmimum output is allowed).
- setDCData(<outputChannel>, <value>)
- getDCData(<outputChannel>)

Send the dot-correction data to the TLC5940 with sendDCData ().
================================================================================
*/


#ifndef GSCLK_TC
  #error GSCLK_TC must be defined.
#endif

#if GSCLK_TC < TC0_CH0
  #error GSCLK_TC must be one of the TC*_CH* values.
#elif GSCLK_TC > TC2_CH2
  #error GSCLK_TC must be one of the TC*_CH* values.
#endif


#ifndef BLANK_TC
  #error BLANK_TC must be defined.
#endif

#if BLANK_TC < TC0_CH0
  #error BLANK_TC must be one of the TC*_CH* values.
#elif BLANK_TC > TC2_CH2
  #error BLANK_TC must be one of the TC*_CH* values.
#endif


#if BLANK_TC == TC0_CH0
  #define BLANK_TC_IRQ_HANDLER TC0_Handler
#elif BLANK_TC == TC0_CH1
  #define BLANK_TC_IRQ_HANDLER TC1_Handler
#elif BLANK_TC == TC0_CH2
  #define BLANK_TC_IRQ_HANDLER TC2_Handler
#elif BLANK_TC == TC1_CH0
  #define BLANK_TC_IRQ_HANDLER TC3_Handler
#elif BLANK_TC == TC1_CH1
  #define BLANK_TC_IRQ_HANDLER TC4_Handler
#elif BLANK_TC == TC1_CH2
  #define BLANK_TC_IRQ_HANDLER TC5_Handler
#elif BLANK_TC == TC2_CH0
  #define BLANK_TC_IRQ_HANDLER TC6_Handler
#elif BLANK_TC == TC2_CH1
  #define BLANK_TC_IRQ_HANDLER TC7_Handler
#elif BLANK_TC == TC2_CH2
  #define BLANK_TC_IRQ_HANDLER TC8_Handler
#else
  #error Invalid value for BLANK_TC.
#endif


////////////////////////////////////////////////////////////////////////////////
/// External functions

extern void initTLC5940 ();


extern void setGSData(uint8_t tlcOutput, uint16_t value);

extern uint16_t getGSData(uint8_t tlcOutput);

extern void setAllGSData(uint16_t value);

extern void sendGSData ();


extern void setDCData(uint8_t tlcOutput, byte value);

extern byte getDCData(uint8_t tlcOutput);

extern void sendDCData ();


#endif  // DUE_TLC5940_h

////////////////////////////////////////////////////////////////////////////////
/// End of Code
