/*
================================================================================
File name: due_usart.cpp
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

#include "due_usart.h"


////////////////////////////////////////////////////////////////////////////////
/// USART Descriptions
///
// Only the first two USARTs are defined since those are the only ones that are
// connected to Arduino Due pins.
///

const USARTDesc USARTDescs[] = 
 {
  {USART0, ID_USART0, PIOA, PIO_PA17, PIO_PERIPH_B, PIOA, PIO_PA11, PIO_PERIPH_A, PIOA, PIO_PA10, PIO_PERIPH_A},
  {USART1, ID_USART1, PIOA, PIO_PA16, PIO_PERIPH_A, PIOA, PIO_PA13, PIO_PERIPH_A, PIOA, PIO_PA12, PIO_PERIPH_A}
 };


////////////////////////////////////////////////////////////////////////////////
/// End of Code
