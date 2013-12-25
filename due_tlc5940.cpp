/*
================================================================================
File name: tlc5940.c
   System: Due TLC5940 Library
 Platform: Arduino Due (Amtel SAM3X8E ARM Cortex-M3)
   Author: Madeline Usher
  Created: June 27, 2013
  Purpose: Code specific to handling the TLC5940 is broken out into this file.
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

#include "due_tlc5940.h"


////////////////////////////////////////////////////////////////////////////////
/// GSCLK and BLANK Timers
/// 
/// The grayscale clock is used to clock the PWM outputs of the TLC5940.  These
/// are 12-bit, so one PWM cycle takes 4096 GSCLK ticks.  Blanking the output to
/// latch in new data should only be done at the boundaries between PWM cycles
/// for the smoothest looking output, so we use two counters:
/// 
/// 1) GSCLK running at 2.1 MHz, will output a 50% duty cycle.  The PWM cycle 
///    will then be ~513 Hz.
/// 
/// 2) The BLANK timer will generate an interrupt every 4096 cycles of GSCLK.  
/// 

Tc* gsblTC;
uint32_t blankTCChannel;

void setupTLC5940_GSCLKTimers ()
 {
  TCDescription gsclkTimer = TCDescs[GSCLK_TC];
  TCDescription blankTimer = TCDescs[BLANK_TC];
  blankTCChannel = blankTimer.channel;
  
  // GSCLK and BLANK must be on the same timer counter so we can chain
  // the TIOA output of GSCLK into BLANK.
  gsblTC = gsclkTimer.timerCounter;
     
  // Turn on power to the timer counters being used:
  pmc_enable_periph_clk(gsclkTimer.PMC_ID);
  pmc_enable_periph_clk(blankTimer.PMC_ID);

  // Configure PIO to output GSCLK's TIOA to a Due pin:
  PIO_SetPeripheral(gsclkTimer.TIOA_PIOport, gsclkTimer.TIOA_PIOperipheral, 
                    gsclkTimer.TIOA_PIOpin);
  
  // Disable write protect of TC registers:
  gsblTC->TC_WPMR = TC_WPMR_WPKEY(0x54494D);  
  
  TC_Configure(gsblTC, gsclkTimer.channel, 
               TC_CMR_WAVE |
               TC_CMR_WAVSEL_UP_RC |
               TC_CMR_TCCLKS_TIMER_CLOCK1 |  // MCK/2 = 42,000,000 Hz
               TC_CMR_ASWTRG_CLEAR |
               TC_CMR_ACPA_SET |
               TC_CMR_ACPC_CLEAR);
  TC_SetRA (gsblTC, gsclkTimer.channel, 10);  // 50% duty cycle
  TC_SetRC (gsblTC, gsclkTimer.channel, 20);  // 2.1 MHz
  
  // TIOA of the GSCLK timer counter channel is used as the input
  // clock for the BLANK timer counter channel:
  gsblTC->TC_BMR = gsclk_blank_BMR_val(gsclkTimer.channel, blankTimer.channel);
  TC_Configure(gsblTC, blankTimer.channel, 
               TC_CMR_WAVE |
               TC_CMR_WAVSEL_UP_RC |
               blankClockSrc(blankTimer.channel) |
               TC_CMR_CPCSTOP);
  TC_SetRC (gsblTC, blankTimer.channel, 4096);  // 4096 counts of GSCLK
    
  gsblTC->TC_CHANNEL[blankTimer.channel].TC_IER = TC_IER_CPCS;
  gsblTC->TC_CHANNEL[blankTimer.channel].TC_IDR = ~TC_IDR_CPCS;
  NVIC_EnableIRQ(blankTimer.IRQ_ID);
  
  TC_Start (gsblTC, blankTimer.channel);
  TC_Start (gsblTC, gsclkTimer.channel);
  return;
 }


////////////////////////////////////////////////////////////////////////////////
/// TLC5940 SPI Communications

Usart* tlcUSART;
Pio* SCLK_PIOport;
uint32_t SCLK_PIOpin;
EPioType SCLK_PIOperiph;


void setupTLC5940_SPI ()
 {
  USARTDesc tlcUSARTd = USARTDescs[TLC_USART];
  tlcUSART = tlcUSARTd.usart;
  SCLK_PIOport = tlcUSARTd.SCLK_PIOport;
  SCLK_PIOpin = tlcUSARTd.SCLK_PIOpin;
  SCLK_PIOperiph = tlcUSARTd.SCLK_PIOperiph;
   
  // Enable the USART peripheral clock:
  pmc_enable_periph_clk(tlcUSARTd.PMC_ID);
  
  // Connect correct peripherals to pins:
  PIO_SetPeripheral (SCLK_PIOport, SCLK_PIOperiph, SCLK_PIOpin);
  PIO_SetPeripheral (tlcUSARTd.MOSI_PIOport, 
                     tlcUSARTd.MOSI_PIOperiph, 
                     tlcUSARTd.MOSI_PIOpin);
  PIO_SetPeripheral (tlcUSARTd.MISO_PIOport, 
                     tlcUSARTd.MISO_PIOperiph, 
                     tlcUSARTd.MISO_PIOpin);
  
  // Disable write protect of USART registers:
  tlcUSART->US_WPMR = US_WPMR_WPKEY(0x555341);  
  
  USART_Configure(tlcUSART, 
                  US_MR_USART_MODE_SPI_MASTER |
                  US_MR_USCLKS_MCK |
                  US_MR_CHRL_8_BIT |
                  US_MR_SYNC |
                  US_SPI_BPMODE_0 | 
                  US_MR_PAR_NO |
                  US_MR_CHMODE_NORMAL |
                  US_MR_MSBF |
                  US_MR_CLKO,
                  10500000,     // SPI CLK ~= 10 MHz
                  VARIANT_MCK);  
  USART_SetTransmitterEnabled(tlcUSART, true);
  return;
 }


void pulseSPI_SCK ()
 // After latching in GS data, the TLC5940 requires another SCLK pulse before
 // more GS data can be sent.  (It triggers the sending off status data on MISO.)
 // So after an XLAT, we need to manually pulse the SCLK once.
 {
  PIO_SetOutput (SCLK_PIOport, SCLK_PIOpin, 1, 0, 0);
  // Don't need a delay here -- PIO_Clear takes longer than the minimum
  // hold time needed for SCLK.
  PIO_Clear(SCLK_PIOport, SCLK_PIOpin);
  PIO_SetPeripheral (SCLK_PIOport, SCLK_PIOperiph, SCLK_PIOpin);
  return;
 }


void waitForSPITransmitFinish ()
 {
  while ((tlcUSART->US_CSR & US_CSR_TXEMPTY) == 0);
 }


////////////////////////////////////////////////////////////////////////////////
/// Other I/O Pins

Pio* tlc_BLANK_port;
uint32_t tlc_BLANK_pin;

Pio* tlc_XLAT_port;
uint32_t tlc_XLAT_pin;


void initTLC5940_IOPins ()
 {
  pinMode(BLANK_PIN, OUTPUT);
  digitalWrite(BLANK_PIN, HIGH);  // start with BLANK active
  tlc_BLANK_port = digitalPinToPort(BLANK_PIN);
  tlc_BLANK_pin = digitalPinToBitMask(BLANK_PIN);
  
  pinMode(XLAT_PIN, OUTPUT);
  digitalWrite(XLAT_PIN, LOW);
  tlc_XLAT_port = digitalPinToPort(XLAT_PIN);
  tlc_XLAT_pin = digitalPinToBitMask(XLAT_PIN);
  
  pinMode(VPRG_PIN, OUTPUT);
  digitalWrite(VPRG_PIN, LOW);
  
  pinMode(XERR_PIN, INPUT);
  
  return;
 }


void pulseXLAT ()
 {
  PIO_Set(tlc_XLAT_port, tlc_XLAT_pin);
    
  // Don't need a delay here -- PIO_Clear takes longer than the minimum
  // hold time needed for XLAT.
  PIO_Clear(tlc_XLAT_port, tlc_XLAT_pin);
  return;
 }


////////////////////////////////////////////////////////////////////////////////
/// Grayscale Data
///
/// Packed grayscale data, 24 bytes (16 * 12 bits) per TLC.
/// 
/// Taken from Alex Leone's Arduino TLC5940 Library.
/// 
/// Format: Lets assume we have 2 TLCs, A and B, daisy-chained with the SOUT of
///   A going into the SIN of B.
///   - byte 0: upper 8 bits of B.15
///   - byte 1: lower 4 bits of B.15 and upper 4 bits of B.14
///   - byte 2: lower 8 bits of B.0
///   - ...
///   - byte 24: upper 8 bits of A.15
///   - byte 25: lower 4 bits of A.15 and upper 4 bits of A.14
///   - ...
///   - byte 47: lower 8 bits of A.0
/// 
/// Note: Normally packing data like this is bad practice.  But in this
/// situation, shifting the data out is really fast because the format of
/// the array is the same as the format of the TLC's serial interface.

byte tlc_GSData[NUM_TLCS * 24];
volatile boolean tlc_needXLAT = false;
boolean tlc_GSDataChangedSinceLastSend = true;
volatile boolean tlc_needExtraSCKPulseAfterGSData = false;


void setGSData(uint8_t tlcOutput, uint16_t value)
  // Sets a value in the local tlc_GSData array which will be used as the
  // grayscale value for the specified TLC5940 output the next time sendGSData
  // is called.  OUT0 of the first TLC is tlcOutput = 0, OUT0 of the next TLC 
  // is tlcOutput = 16, and so on.  4095 is the maximum allowed grayscale value.
  // Taken from Alex Leone's library.
 {
  uint8_t index8 = (NUM_TLCS * 16 - 1) - tlcOutput;
  uint8_t *index12p = tlc_GSData + ((((uint16_t)index8) * 3) >> 1);
  if (index8 & 1) 
   {// Starts in the middle.
    // First 4 bits intact, 4 top bits of value:
    *index12p = (*index12p & 0xF0) | (value >> 8);
    // 8 lower bits of value:
    *(++index12p) = value & 0xFF;
   }
  else
   {// Starts clean.
    // 8 upper bits of value:
    *(index12p++) = value >> 4;
    // 4 lower bits of value, last 4 bits intact:
    *index12p = ((uint8_t)(value << 4)) | (*index12p & 0xF);
   }
  tlc_GSDataChangedSinceLastSend = true;
  return;
 }


uint16_t getGSData(uint8_t tlcOutput)
  // Gets a value from the local tlc_GSData array.
  // Taken from Alex Leone's library.
 {
  uint8_t index8 = (NUM_TLCS * 16 - 1) - tlcOutput;
  uint8_t *index12p = tlc_GSData + ((((uint16_t)index8) * 3) >> 1);
  return (index8 & 1)? // starts in the middle
          (((uint16_t)(*index12p & 15)) << 8) | // upper 4 bits
          *(index12p + 1)                       // lower 8 bits
      : // starts clean
          (((uint16_t)(*index12p)) << 4) | // upper 8 bits
          ((*(index12p + 1) & 0xF0) >> 4); // lower 4 bits
 }


void setAllGSData(uint16_t value)
 // Sets all entries in the local tlc_GSData array to the specified value.
 // Taken from Alex Leone's library.
 {
  uint8_t firstByte = value >> 4;
  uint8_t secondByte = (value << 4) | (value >> 8);
  uint8_t *p = tlc_GSData;
  while (p < tlc_GSData + NUM_TLCS * 24) 
   {
    *p++ = firstByte;
    *p++ = secondByte;
    *p++ = (uint8_t)value;
   }
  tlc_GSDataChangedSinceLastSend = true;
  return;
 }
 
 
void sendGSData ()
 {
  if (tlc_GSDataChangedSinceLastSend)
   {
    // If we get here and the last XLAT request hasn't been handled yet, set
    // tlc_needXLAT back to false so we can send the latest data before latching 
    // it in.
    tlc_needXLAT = false;

    digitalWrite(VPRG_PIN, LOW);
    for (byte j = 0; j < NUM_TLCS * 24; j++) 
     {
      USART_Write(tlcUSART, tlc_GSData[j], 0);
     }
    tlc_GSDataChangedSinceLastSend = false;
	waitForSPITransmitFinish();
    tlc_needXLAT = true;
   }
  return;
 }


////////////////////////////////////////////////////////////////////////////////
/// Dot Correction
///
/// I'm going to assume that dot correction data is not changed very often, so
/// I'm not going to bother with the complexity of a packed data array for the
/// local copy of the DC data and instead will just use an array of bytes.
/// Sending the data to the TLC5940 will be a bit slower, but I don't think that
/// really matters here.
/// 
/// tlc_DCData corresponds to OUT0 of the first TLC5940, OUT16 to OUT0 of the 
/// second TLC5940, and so on.

byte tlc_DCData[NUM_TLCS * 16] = INITIAL_DCDATA;


void setDCData(uint8_t tlcOutput, byte value)
  // Sets a value in the local tlc_DCData array which will be used as the
  // dot-correction value for the specified TLC5940 output the next time sendDCData
  // is called.  OUT0 of the first TLC is tlcOutput = 0, OUT0 of the next TLC 
  // is tlcOutput = 16, and so on.  63 is the maximum allowed value.
 {
  tlc_DCData[tlcOutput] = value;
  return;
 }


byte getDCData(uint8_t tlcOutput)
  // Gets a value from the local tlc_DCData array.
 {
  return(tlc_DCData[tlcOutput]);
 }


void sendDCData ()
 // There are four 6-bit DC data values packed into every three bytes.
 {
  while (tlc_needXLAT);  // wait for pending GSData to be latched in
  
  digitalWrite(VPRG_PIN, HIGH);  // VPRG high says we're setting the DC register
  
  byte byte0, byte1, byte2;
  byte dca, dcb, dcc, dcd;
  for (short j = NUM_TLCS * 16 - 1; j > 0; j -=4)
   {
    dcd = (tlc_DCData[j] & 0x3F);
    dcc = (tlc_DCData[j - 1] & 0x3F);
    dcb = (tlc_DCData[j - 2] & 0x3F);
    dca = (tlc_DCData[j - 3] & 0x3F);
    byte2 = (dcd << 2) | (dcc >> 4);
    byte1 = (dcc << 4) | (dcb >> 2);
    byte0 = (dcb << 6) | dca;
    USART_Write(tlcUSART, byte2, 0);
    USART_Write(tlcUSART, byte1, 0);
    USART_Write(tlcUSART, byte0, 0);
   }
  waitForSPITransmitFinish();
  
  // Sending DC data can be done without blanking the output.  Just latch it
  // in and magic happens.
  pulseXLAT();

  tlc_needExtraSCKPulseAfterGSData = true;
  return;
 }


////////////////////////////////////////////////////////////////////////////////
/// Overall Initialization

void initTLC5940 ()
 {
  setAllGSData(0);
  initTLC5940_IOPins();
  setupTLC5940_SPI();
  sendDCData();
  setupTLC5940_GSCLKTimers();
  sendGSData();
  return;
 }


////////////////////////////////////////////////////////////////////////////////
/// Interrupt handler for GSCLK and BLANK Timers

void BLANK_TC_IRQ_HANDLER ()
// This fires every 4096 counts of GSCLK.
 {
  // The following clears the RC Compare Status bit so the interrupt can be
  // triggered again in the future.
  TC_GetStatus(gsblTC, blankTCChannel); 
  
  PIO_Set(tlc_BLANK_port, tlc_BLANK_pin);
  if (tlc_needXLAT)
   {
    tlc_needXLAT = false;
    pulseXLAT();
	if (tlc_needExtraSCKPulseAfterGSData)
	 {
	  pulseSPI_SCK();
	  tlc_needExtraSCKPulseAfterGSData = false;
	 }
   }
  gsblTC->TC_BCR = TC_BCR_SYNC; // reset both GSCLK timer counters
  PIO_Clear(tlc_BLANK_port, tlc_BLANK_pin);
  return;
 }


////////////////////////////////////////////////////////////////////////////////
/// End of Code
