/*
================================================================================
File name: example.ino
   System: Due TLC5940 Library
 Platform: Arduino Due (Amtel SAM3X8E ARM Cortex-M3)
   Author: Madeline Usher
  Created: July 6, 2013
  Purpose: Example showing usage of the TLC5940 Library.
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

/// Pin Assignments:
/// - SPI CLK  = SDA1, A.17, peripheral B -> TLC5940 SCLK (pin 25)
/// - SPI MOSI = TX1 (pin 18), A.11. peripheral A -> TLC5940 SIN (pin 26)
/// - SPI MISO = RX1 (pin 17), A.10, peripheral A -> TLC5940 SOUT (pin 17) [opt]
/// - pin 6 -> TLC5940 VPRG (pin 27)
/// - pin 5 -> TLC5940 XLAT (pin 24)
/// - pin 4 -> TLC5940 BLANK (pin 23)
/// - pin 3 -> TLC5940 GSCLK (pin 18)
/// - pin 2 -> TLC5940 XERR (pin 16)  [optional]

////////////////////////////////////////////////////////////////////////////////

#include "Arduino.h"
#include "due_tlc5940.h"


////////////////////////////////////////////////////////////////////////////////
/// The setup() method runs once, when the sketch starts

void setup ()
 {
  initTLC5940();
  return;
 }


////////////////////////////////////////////////////////////////////////////////
/// The loop() method runs over and over again, as long as the Arduino has power

void loop()
 {
  // Assumes there is an RGB LED connected to the following TLC5940 output
  // channels:
  // - output 3 = red
  // - output 4 = green
  // - output 5 = blue
  
  delay (500);
  setRGB(1, 2048, 0, 0);
  
  delay (500);
  setRGB(1, 0, 2048, 0);
  
  delay (500);
  setRGB(1, 0, 0, 2048);
 
  delay (500);
  setRGB(1, 2048, 2048, 2048);
  
  delay (500);
  setRGB(1, 0, 0, 0);
  
  delay (500);
  setRGB(1, 0,  128, 0);
  delay (500);
  setRGB(1, 0,  256, 0);
  delay (500);
  setRGB(1, 0,  512, 0);
  delay (500);
  setRGB(1, 0, 1024, 0);
  delay (500);
  setRGB(1, 0, 2048, 0);
  delay (500);
  setRGB(1, 0, 4095, 0);
  delay (500);
  setRGB(1, 0, 0, 0);
 }


 
void setRGB (byte led, uint16_t r, uint16_t g, uint16_t b)
 {
  byte base = led * 3;
  setGSData(base++, r);  // red
  setGSData(base++, g);  // green
  setGSData(base,   b);  // blue
  sendGSData();
  return;
 }


////////////////////////////////////////////////////////////////////////////////
/// End of Code
