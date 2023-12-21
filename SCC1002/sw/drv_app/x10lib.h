/*;**********************************************************************
;                                                                     *
;    Filename:       x10_lib.h                  *
;    Date:          10/15/02                                          *
;    File Version:   1.00                                             *
;                                                                     *
;    Author:        Jon Burroughs                                     *
;    Company:       Microchip Technology                              *
;                                                                     *
;                                                                     *
;**********************************************************************
;                    Software License Agreement                       *
;                                                                     *
; The software supplied herewith by Microchip Technology              *
; Incorporated (the "Company") for its PICmicro® Microcontroller is   *
; intended and supplied to you, the Company’s customer, for use       *
; solely and exclusively on Microchip PICmicro Microcontroller        *
; products. The software is owned by the Company and/or its           *
; supplier, and is protected under applicable copyright laws. All     *
; rights are reserved. Any use in violation of the foregoing          *
; restrictions may subject the user to criminal sanctions under       *
; applicable laws, as well as to civil liability for the breach of    *
; the terms and conditions of this license.                           *
;                                             *
; THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,   *
; WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED   *
; TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A          *
; PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,   *
; IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR          *
; CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.                 *
;                                                                     *
;**********************************************************************
;                                                                     *
;    Files required:                                                  *
;           p16F877.inc  or   p16F877a.inc                            *
;                                                                     *
;**********************************************************************
;                                                                     *
;    Notes:   This file provides constant definitions for X10 calls.  *
;**********************************************************************

   list      p=16F877         ; list directive to define processor
        #include <p16F877.inc>        ; processor specific variable definitions
        errorlevel -302               ; suppress "register not in bank0, check page bits" message.

;**********************************************************************
;   X-10 board signal/pin definitions                                 *
;**********************************************************************/

//#define   ZERO_X PORTB, 0
//#define   XOUT   TRISC, 2   //clear to enable 120kHz output, set disable 120kHz output
//#define   XIN    PORTC, 3

//#define   LED1   PORTA, 1
//#define   LED2   PORTA, 2


/*;**********************************************************************
; X10 function definitions                                          *
; For each 2 bits at least one change its state. The possible combinations
; are varying 2 two bits between 01 or 10 combination
;***********************************************************************/ 
#define   AllUnitsOff     0b01010101   // 0x55 - 'All units off'      b'0000'
#define   AllLightsOn     0b01010110   // 0x56 - 'All lights on'               b'0001'
#define   OnFnc           0b01011001   // 0x59 - 'On'                          b'0010'
#define   OffFnc          0b01011010   // 0x5A - 'Off'                         b'0011'
#define   Dim             0b01100101   // 0x45 - 'Dim'                         b'0100'
#define   Bright          0b01100110   // 0x46 - 'Bright'                      b'0101'
#define   AllLightsOff    0b01101001   // 0x49 - 'All lights off'              b'0110'
#define   ExtendedCode    0b01101010   // 0x4A - 'Extended Code'               b'0111'
#define   HailRequest     0b10010101   // 0x95 - 'Hail Request'                b'1000'
#define   HailAcknowledge 0b10010110   // 0x96 - 'Hail Acknowledge'            b'1001'
#define   PresetDim       0b10011001   // 0x99 - 'Pre-set Dim'(actually, 101x) b'1010'
#define   ExtendedData    0b10100101   // 0xA5 - 'Extended Data (Analog)'      b'1100'
#define   StatusOn        0b10100110   // 0xA6 - 'Status = ON'                 b'1101'
#define   StatusOff       0b10101001   // 0xA9 - 'Status = OFF'                b'1110'
#define   StatusRequest   0b10101010   // 0xAA - 'Status request'              b'1111'

#define   HouseA      0x00
#define   HouseB      0x01
#define   HouseC      0x02
#define   HouseD      0x03
#define   HouseE      0x04
#define   HouseF      0x05
#define   HouseG      0x06
#define   HouseH      0x07
#define   HouseI      0x08
#define   HouseJ      0x09
#define   HouseK      0x0A
#define   HouseL      0x0B
#define   HouseM      0x0C
#define   HouseN      0x0D
#define   HouseO      0x0E
#define   HouseP      0x0F

#define   Unit1      0x00
#define   Unit2      0x01
#define   Unit3      0x02
#define   Unit4      0x03
#define   Unit5      0x04
#define   Unit6      0x05
#define   Unit7      0x06
#define   Unit8      0x07
#define   Unit9      0x08
#define   Unit10      0x09
#define   Unit11      0x0A
#define   Unit12      0x0B
#define   Unit13      0x0C
#define   Unit14      0x0D
#define   Unit15      0x0E
#define   Unit16      0x0F

/*;**********************************************************************
;   X10 flag definitions                                              *
;**********************************************************************/
#define      RxBit               X10Flags, 0
#define      TxBit               X10Flags, 1
#define      RxCommandFlag      X10Flags, 2   //Set when command received, clear when address received
#define      TxCommandFlag      X10Flags, 3   //Set when sending command, clear when sending address
#define      X10RxFlag         X10Flags, 4   //Set when receive buffer ready, cleared when receive done
#define      X10TxFlag         X10Flags, 5   //Set to transmit, gets cleared when transmit complete
#define      SecondTxFlag      X10Flags, 6   //Set to indicate 2nd transmission
#define      Wait3CyclesFlag   X10Flags, 7   //Set to wait for 3 cycles, clear when done used for dimm command

/*;**********************************************************************
;   X10 General defines                                                 *
;**********************************************************************/
#define X10MaxUnits   8

/*;****************************************************************************************************
;Define X10 Library Macros
;*****************************************************************************************************/
/*;----------------------------------------------------------------------
; SkipIfTxDone
;----------------------------------------------------------------------*/
#define SkipIfTxDone bit_test(X10TxFlag) //If X10TxFlag is clear, then last transmit is done

/*;----------------------------------------------------------------------
; SkipIfRxDone
;----------------------------------------------------------------------*/
#define SkipIfRxDone bit_test(X10RxFlag) //If X10RxFlag is set, then data has been received



/*;----------------------------------------------------------------------
; SkipIfReceiveReady
;----------------------------------------------------------------------*/
#define SkipIfReceiveReady bit_test(X10RxFlag)//If X10RxFlag is set, then receive buffer is ready


/*----------------------------------------------------------------------
; SkipIfCommandRcvd
;----------------------------------------------------------------------*/
#define SkipIfCommandRcvd bit_test(TxCommandFlag)//If TxCommandFlag is set, then command has been received

/*;----------------------------------------------------------------------
; SkipIfAddressRcvd
;----------------------------------------------------------------------*/
#define SkipIfAddressRcvd !bit_test(TxCommandFlag)//If TxCommandFlag is clear, then address has been received


/* Function Prototypes */
void EndX10Envelope();
void ZeroCrossing();
