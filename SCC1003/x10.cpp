/*
  x10.cpp - X10 transmission library for Arduino version 0.4
  
 Copyright (c) 2007 by Tom Igoe (tom.igoe@gmail.com)
This file is free software; you can redistribute it and/or modify
it under the terms of either the GNU General Public License version 2
or the GNU Lesser General Public License version 2.1, both as
published by the Free Software Foundation.
  
  Original library								(0.1) 
  Timing bug fixes								(0.2)
  #include bug fixes for 0012					(0.3)
  Refactored version following Wire lib API		(0.4)
    
  Zero crossing algorithms borrowed from David Mellis' shiftOut command
  for Arduino.
  
  The circuits can be found at 
 
http://www.arduino.cc/en/Tutorial/x10
 
 
 */

#include "application.h"
#include "x10.h"
#include "x10constants.h"
//#include "coremacros.h"

uint8_t x10Class::zeroCrossingPin = 2;	// AC zero crossing pin
uint8_t x10Class::rxPin = 3;			// data in pin
uint8_t x10Class::txPin = 4;			// data out pin

uint8_t x10Class::houseCode = 0;		// house code
uint8_t x10Class::transmitting = 0;		// whether you're transmitting or not


/*
 Constructor.
 
 Sets the pins and sets their I/O modes.
 
 */
 
//x10Class::x10Class() {}


void x10Class::begin(int _rxPin, int _txPin, int _zcPin)
{
  // initialize pin numbers:
  txPin = _txPin;        		
  rxPin = _rxPin;        	
  zeroCrossingPin = _zcPin;			
  houseCode = 0;				  
  transmitting = 0;			

  // Set I/O modes:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  pinMode (zeroCrossingPin, INPUT);
  
}

/*
void x10Class::beginTransmission(uint8_t data)
{
	houseCode = data;
	transmitting = 1;
}

void x10Class::beginTransmission(int data)
{
	 beginTransmission((uint8_t)data);

}

void x10Class::endTransmission(void)
{
   // indicate that we are done transmitting
  transmitting = 0;
}

    
size_t x10Class::write(uint8_t data)
{
	if (transmitting) {
		sendCommand(houseCode, data);
	}
}
size_t x10Class::write(const char * data)
{
 write((uint8_t*)data, strlen(data));
}

size_t x10Class::write(const uint8_t *data, size_t quantity)
{
    for(size_t i = 0; i < quantity; ++i){
      write(data[i]);
    }
}
*/

/*
	methods inherited from Stream but not implemented yet.
*/
/*
int x10Class::available(void)
{
}

int x10Class::read(void)
{
}

int x10Class::peek(void)
{
}

void x10Class::flush(void)
{
}
*/
/*
	Writes an X10 command out to the X10 modem
*/
void x10Class::write(byte houseCode, byte numberCode, byte numRepeats) {
 
   byte startCode = 0b1110; 		// every X10 command starts with this
   uint8_t i;
   
	// repeat as many times as requested:
    for (i = 0; i < numRepeats; i++) {
	    // send the three parts of the command:
	    sendBits(startCode, 4, true);	
	    sendBits(houseCode, 4, false);
	    sendBits(numberCode, 5, false);
    }

  	// if this isn't a bright or dim command, it should be followed by
  	// a delay of 3 power cycles (or 6 zero crossings):
  	if ((numberCode != BRIGHT) && (numberCode != DIM)) {
  		waitForZeroCross(zeroCrossingPin, 6);
   	}
}


/*
	Writes a sequence of bits out.  If the sequence is not a start code,
	it repeats the bits, inverting them.
*/

void x10Class::sendBits(byte cmd, byte numBits, byte isStartCode) {
  	byte thisBit;	// byte for shifting bits
  
	// iterate the number of bits to be shifted:
	for(int i=1; i<=numBits; i++) {
		// wait for a zero crossing change
		waitForZeroCross(zeroCrossingPin, 1);
		// shift off the last bit of the command:
		thisBit = cmd & (1 << (numBits - i));
		
		// repeat once for each phase:
		for (int phase = 0; phase < 3; phase++) {
			// set the data Pin:
			digitalWrite(txPin, thisBit);
			delayMicroseconds(BIT_LENGTH);
			// clear the data pin:
			digitalWrite(txPin, LOW);
			delayMicroseconds(BIT_DELAY);
		}
		
		// if this command is a start code, don't
		// send its complement.  Otherwise do:
		if(!isStartCode) {
			// wait for zero crossing:
			waitForZeroCross(zeroCrossingPin, 1);
			for (int phase = 0; phase < 3; phase++) {
				// set the data pin:
				digitalWrite(txPin, !thisBit);
				delayMicroseconds(BIT_LENGTH);
				// clear the data pin:
				digitalWrite(txPin, LOW);
				delayMicroseconds(BIT_DELAY);
			}
		}
	}
}

/*
	TO DO: receiveBits and receiveCommand to parallel the above
*/

/*
  waits for a the zero crossing pin to cross zero
*/
void x10Class::waitForZeroCross(int pin, int howManyTimes) {
	unsigned long cycleTime = 0;
	
    uint8_t bit = pinReadFast(pin);
    
  	for (int i = 0; i < howManyTimes; i++) {
		// wait for pin to change:
    	if(pinReadFast(pin) & bit)
    	 	while(pinReadFast(pin) & bit) 
        		cycleTime++;
    	else
      		while(!pinReadFast(pin) & !bit) 
        		cycleTime++;
  		}    
}

/*;****************************************************************************************************
;X-10 Table
;****************************************************************************************************
;----------------------------------------------------------------------
;EncodeTable
;   Takes EncodeValue and returns corresponding X-10 code
;   in true compliment format (0=01, 1=10)
;
;   Used to return value for H1 H2 H3 H4 based on housecode 0-15 ('A'-'P')
;   or value of D1 D2 D4 D16 based on unit code 0-15 ('1'-'16')
;----------------------------------------------------------------------*/
uint8_t x10Class::EncodeX10House(uint8_t EncodeValue)
{
   switch(EncodeValue)              
   {
      case(65): return A; 
         break;
      case(66): return B; 
         break;
      case(67): return C; 
         break;
      case(68): return D; 
         break;
      case(69): return E; 
         break;
      case(70): return F; 
         break;
      case(71): return G; 
         break;
      case(72): return H; 
         break;
      case(73): return I; 
         break;
      case(74): return J; 
         break;
      case(75): return K; 
         break;
      case(76): return L; 
         break;
      case(77): return M; 
         break;
      case(78): return N; 
         break;
      case(79): return O; 
         break;
      case(80): return P; 
         break;
   }

   return(0);
}

/*
;----------------------------------------------------------------------
;EncodeTable
;   Takes EncodeValue and returns corresponding X-10 code
;   in true compliment format (0=01, 1=10)
;
;   Used to return value of D1 D2 D4 D16 based on unit code 0-15 ('1'-'16')
;----------------------------------------------------------------------*/
uint8_t x10Class::EncodeX10Unit(uint8_t EncodeValue)
{
   switch(EncodeValue)              
   {
      case(49): return UNIT_1; 
         break;
      case(50): return UNIT_2; 
         break;
      case(51): return UNIT_3; 
         break;
      case(52): return UNIT_4; 
         break;
      case(53): return UNIT_5; 
         break;
      case(54): return UNIT_6; 
         break;
      case(55): return UNIT_7; 
         break;
      case(56): return UNIT_8; 
         break;
      case(57): return UNIT_9; 
         break;
      case(58): return UNIT_10; 
         break;
      case(59): return UNIT_11; 
         break;
      case(60): return UNIT_12; 
         break;
      case(61): return UNIT_13; 
         break;
      case(62): return UNIT_14; 
         break;
      case(63): return UNIT_15; 
         break;
      case(64): return UNIT_16; 
         break;
   }

   return(0);
}

/*
;----------------------------------------------------------------------
;EncodeTable
;   Takes EncodeValue and returns corresponding X-10 code
;   in true compliment format (0=01, 1=10)
;
;   Used to return value of D1 D2 D4 D16 based on unit code 0-15 ('1'-'16')
;----------------------------------------------------------------------*/
uint8_t x10Class::EncodeX10State(uint8_t EncodeValue)
{
   switch(EncodeValue)              
   {
      case(49): return ALL_UNITS_OFF; //1
         break;
      case(50): return ALL_LIGHTS_ON; //2
         break;
      case(51): return ON;            //3
         break;
      case(52): return OFF;           //4
         break;
      case(53): return DIM;           //5
         break;
      case(54): return BRIGHT;        //6
         break;
      case(55): return ALL_LIGHTS_OFF; //7
         break;
      case(56): return EXTENDED_CODE; //8
         break;
      case(57): return HAIL_REQUEST;  //9
         break;
      case(58): return HAIL_ACKNOWLEDGE; //:
         break;
      case(59): return PRE_SET_DIM;  //;
         break;
      case(60): return EXTENDED_DATA; //<
         break;
      case(61): return STATUS_ON; //=
         break;
      case(62): return STATUS_OFF; //>
         break;
      case(63): return STATUS_REQUEST; //?
         break;
   }

   return(0);
}
