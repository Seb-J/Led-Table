/*
================================================================================
File name: due_timer_counters.cpp
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

#include "due_timer_counters.h"


////////////////////////////////////////////////////////////////////////////////
/// Timer Counter Descriptions

const TCDescription TCDescs[] = 
 {
  {TC0, 0, ID_TC0, TC0_IRQn, PIOB, PIO_PB25, PIO_PERIPH_B},  // due pin 2
  {TC0, 1, ID_TC1, TC1_IRQn, PIOA, PIO_PA2,  PIO_PERIPH_A},  // pin 61 
  {TC0, 2, ID_TC2, TC2_IRQn, PIOA, PIO_PA5,  PIO_PERIPH_A},  // no pin
  {TC1, 0, ID_TC3, TC3_IRQn, 0,          0,  PIO_NOT_A_PIN}, // no pin
  {TC1, 1, ID_TC4, TC4_IRQn, 0,          0,  PIO_NOT_A_PIN}, // no pin
  {TC1, 2, ID_TC5, TC5_IRQn, 0,          0,  PIO_NOT_A_PIN}, // no pin
  {TC2, 0, ID_TC6, TC6_IRQn, PIOC, PIO_PC25, PIO_PERIPH_B},  // pin 5
  {TC2, 1, ID_TC7, TC7_IRQn, PIOC, PIO_PC28, PIO_PERIPH_B},  // pin 3
  {TC2, 2, ID_TC8, TC8_IRQn, PIOD, PIO_PD7,  PIO_PERIPH_B}   // pin 11
 };


////////////////////////////////////////////////////////////////////////////////
/// Some helper functions to make configuration of the of the GSCLK and BLANK
/// counters more flexible.

uint32_t gsclk_blank_BMR_val (uint32_t gsclkChannel, uint32_t blankChannel)
 {
  switch (blankChannel)
   {
    case 0:
      if (gsclkChannel == 1)
        return (TC_BMR_TC0XC0S_TIOA1);
      else
        return (TC_BMR_TC0XC0S_TIOA2);
    case 1:
      if (gsclkChannel == 0)
        return (TC_BMR_TC1XC1S_TIOA0);
      else
        return (TC_BMR_TC1XC1S_TIOA2);
    case 2:
      if (gsclkChannel == 0)
        return (TC_BMR_TC2XC2S_TIOA0);
      else
        return (TC_BMR_TC2XC2S_TIOA1);
   }
 }

extern uint32_t blankClockSrc (uint32_t blankChannel)
 {
   switch (blankChannel)
   {
    case 0:  return (TC_CMR_TCCLKS_XC0);
    case 1:  return (TC_CMR_TCCLKS_XC1);
    case 2:  return (TC_CMR_TCCLKS_XC2);
   }
 }


////////////////////////////////////////////////////////////////////////////////
/// End of Code
