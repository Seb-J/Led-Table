/*
================================================================================
File name: due_timer_counters.h
   System: Due TLC5940 Library
 Platform: Arduino Due (Amtel SAM3X8E ARM Cortex-M3)
   Author: Madeline Usher
  Created: July 5, 2013
  Purpose: Declarations useful for working with the SAM3X8E timer counters,
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

#ifndef DUE_TLC5940_TC_h
#define DUE_TLC5940_TC_h

#include "Arduino.h"
#include <inttypes.h>

////////////////////////////////////////////////////////////////////////////////

/*
TC0, ch0, TIOA = TIOA0 = PB25, periph B = due digital pin 2 (2)
TC0, ch0, TIOB = TIOB0 = PB27, periph B = due digital pin 13 (13)

TC0, ch1, TIOA = TIOA1 = PA2, A = analog in 7 (61)
TC0, ch1, TIOB = TIOB1 = PA3, A = analog in 6 (60)

TC0, ch2, TIOA = TIOA2 = PA5, A = no pin
TC0, ch2, TIOB = TIOB2 = PA6, A = analog in 4 (58)

TC1, ch0, TIOA = TIOA3 = PE9, A = no pin
TC1, ch0, TIOB = TIOB3 = PE10, A = no pin

TC1, ch1, TIOA = TIOA4 = PE11, A = no pin
TC1, ch1, TIOB = TIOB4 = PE12, A = no pin

TC1, ch2, TIOA = TIOA5 = PE13, A = no pin
TC1, ch2, TIOB = TIOB5 = PE14, A = no pin

TC2, ch0, TIOA = TIOA6 = PC25, B = digital 5 (5)
TC2, ch0, TIOB = TIOB6 = PC26, B = digital 4 (4)

TC2, ch1, TIOA = TIOA7 = PC28, B = digital 3 (3)
TC2, ch1, TIOB = TIOB7 = PC29, B = digital 10 (10)

TC2, ch2, TIOA = TIOA8 = PD7, B = digital 11 (11)
TC2, ch2, TIOB = TIOB8 = PD8, B = digital 12 (12)

*/

#define TC0_CH0   0
#define TC0_CH1   1
#define TC0_CH2   2
#define TC1_CH0   3
#define TC1_CH1   4
#define TC1_CH2   5
#define TC2_CH0   6
#define TC2_CH1   7
#define TC2_CH2   8


// The following does not include info about TIOB since we can't use that as the
// input to another channel when chaining TCs (and we need to do that in this
// library).
typedef struct _TCDescription
 {
  Tc* timerCounter;
  uint32_t channel;
  uint32_t PMC_ID;
  IRQn_Type IRQ_ID;
  Pio* TIOA_PIOport;
  uint32_t TIOA_PIOpin;
  EPioType TIOA_PIOperipheral;
 } TCDescription;

extern const TCDescription TCDescs[];


////////////////////////////////////////////////////////////////////////////////
/// Some helper functions to make configuration of the of the GSCLK and BLANK
/// counters more flexible.

extern uint32_t gsclk_blank_BMR_val (uint32_t gsclkChannel, 
                                     uint32_t blankChannel);

extern uint32_t blankClockSrc (uint32_t blankChannel);


////////////////////////////////////////////////////////////////////////////////
/// Fixes typo in sam3xa component_tc.h:

#define TC_BMR_TC2XC2S_TIOA0 (0x2u << 4)
#define TC_BMR_TC2XC2S_TIOA1 (0x3u << 4)


#endif  // DUE_TLC5940_TC_h

////////////////////////////////////////////////////////////////////////////////
/// End of Code
