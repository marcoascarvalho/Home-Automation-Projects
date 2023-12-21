/*
 * X10Lib.c
 *
 *  Created on: 09/12/2008
 *      Author: Marco
 */

#include "x10lib.h"
//#include <math.h>



/****************************************************************************************************
;ISRRoutines
;****************************************************************************************************
/*;----------------------------------------------------------------------
; ISR-
;   Interrupt Service Routine.  Determine cause of interrupt and perform appropriate action.
;----------------------------------------------------------------------
;   Interrupt Sources:
;   TMR0:      Times duration of 1ms 120kHz burst and onset of 2nd & 3rd phase bursts.
;   RBO/INT:   INTF is set upon each zero-crossing,
;         used to synchronize transmission and
;         reception of X-10 signals (120kHz bursts). */

#INT_TIMER0
void tmr0_isr()
{
   EndX10Envelope();
}

#INT_EXT
void ext_isr()
{
   ZeroCrossing();
}

/*;**********************************************************************
;   Variables                                                         *
;***********************************************************************/

BYTE X10Flags;
unsigned BYTE NumTxBits;
unsigned BYTE TxFunction; //transmit function code
unsigned BYTE TxHouse;    //transmit house code (0-15 == A-P)
unsigned BYTE TxUnit;     //transmit unit code (0-15 == 1-16)

unsigned BYTE RxFunction; //received function code
unsigned BYTE RxHouse;    //received house code (0-15 == A-P)
unsigned BYTE RxUnit;     //received unit code (0-15 == 1-16)

unsigned BYTE RxHouse1;   //temp for house code before decode
unsigned BYTE RxHouse2;   //temp for house code before decode
unsigned BYTE RxKey1;     //temp for function or unit code before suffix is received.
unsigned BYTE RxKey2;

unsigned BYTE RxData0;    //receive shift registers
unsigned BYTE RxData1;
unsigned BYTE RxData2;
unsigned BYTE RxData3;

unsigned BYTE TxData0;    //transmit shift registers
unsigned BYTE TxData1;
unsigned BYTE TxData2;
unsigned BYTE TxData3;

#ifdef 3_PHASE
BYTE PhaseTime;
BYTE Phase;
#endif

//BYTE Count;      //Counter for shifting
//BYTE Dimmer;

/*;****************************************************************************************************
;X10 routines
;  some internal routines have been implemented as macros, to minimize use of the stack
;****************************************************************************************************
;----------------------------------------------------------------------
;InitX10Variables
;----------------------------------------------------------------------*/
void InitX10Variables()
{

   RxData0 = 0;
   RxData1 = 0;
   RxData2 = 0;
   RxData3 = 0;
   RxHouse1 = 0;
   RxHouse2 = 0;
   RxKey1 = 0;
   RxKey2 = 0;
   RxUnit = 0;
   RxFunction = 0;
   TxData0 = 0;
   TxData1 = 0;
   TxData2 = 0;
   TxData3 = 0;
   TxUnit = 0;
   TxHouse = 0;
   TxFunction = 0;
   X10Flags = 0;

#ifdef 3_PHASE
   PhaseTime = 0;
   Phase = 1;
#endif

   return;
}

/*;----------------------------------------------------------------------
; TransmitX10Bit
;   Called at every zero crossing.
;      If TxBit=1, enable 120KHz output for 1ms
;      otherwise, do nothing.
;----------------------------------------------------------------------*/
void TransmitX10Bit()
{

   if(bit_test(TxBit)) //Is TxBit a one?
   {

#ifdef 3_PHASE
   PhaseTime = 0;
   Phase = 1;
#endif

#if USING_TW523
      output_high(XOUT);
#else
      setup_ccp1(CCP_PWM);
#endif

      set_timer0(0xB2); //Set TMR0 up for 1ms delay to time duration of 120KHz burst.
      bit_clear(INTCON, T0IF); //Clear the Timer0 interrupt flag.
      enable_interrupts(INT_TIMER0); //Enable Timer0 interrupt.
   }
   else
   {
      //bit_clear(RxBit); //Clear RxBit.
      //if(bit_test(TxBit)) //Is the TxBit set?
      //{
      //   bit_set(RxBit); //Yes, then set RxBit. (This assures reception of own transmission)
      //}
      //else
      //{
         bit_clear(TxBit); //Clear the TxBit
      //}
   }

   return;
}

/*;----------------------------------------------------------------------
; ReceiveX10Bit
;   Called at every zero crossing.
;      If 120KHz carrier is present (XIN=1), set the RxBit
;      otherwise, clear the the RxBit.
;----------------------------------------------------------------------*/
void ReceiveX10Bit()
{
//   if(!bit_test(X10TxFlag)) //If X10 transmit flag is not set,
//   {
//      bit_clear(RxBit); //Clear RxBit.
//   }
//   else
//   {
#if USING_TW523
      if(input(XIN)) //Is 120kHz signal present?
      {
         bit_clear(RxBit); //Yes, then clear RxBit.
      }
      else
      {
         bit_set(RxBit); //Set RxBit.
      }
#else
      if(input(XIN)) //Is 120kHz signal present?
      {
         bit_set(RxBit); //Yes, then set RxBit.
      }
      else
      {
         bit_clear(RxBit); //Clear RxBit.
      }
#endif
//   }

   return;
}

/*;----------------------------------------------------------------------
;ToggleInterruptEdge
;   Toggles INTEDG bit so that interrupts are generated
;    on both rising and falling zero-crossings on RB0/INT pin.
;----------------------------------------------------------------------*/
void ToggleInterruptEdge()
{
   if(bit_test(OPTION_REG, INTEDG))//If interrupt was on rising edge
   {
      ext_int_edge( H_TO_L ); //set next interrupt for falling edge
   }
   else
   {
      ext_int_edge( L_TO_H ); //otherwise, set next interrupt for rising edge
   }

   return;

}

/*;----------------------------------------------------------------------
; RotateTxData
;     Rotates data in TxData register to left by number in Wreg
;----------------------------------------------------------------------*/
void RotateTxData(BYTE Times_to_rotate)
{

   BYTE i;

   bit_clear(STATUS, 0); //Clear the carry bit
   if(bit_test(TxData3, 7)) //Is MSb of TxData3 set?
   {
      bit_set(STATUS, 0); //Yes, set the carry bit
   }

   for(i=0;i<Times_to_rotate;i++)
   {
#asm
      rlf   TxData0, f
      rlf   TxData1, f
      rlf   TxData2, f
      rlf   TxData3, f
#endasm
   }


   return;
}

/*;----------------------------------------------------------------------
; X10Tx
;
;   Transmit 22 bits of data out of TXData registers
;   Rotate by 10 to reposition TXData
;   Original: Transmit 28 bits of data out of TXData registers:
;            if considering Transmiting Zeros during 6 Zerox.
;  Changed: Transmit only 22 bits and transmit nothing during
;           zero crossings. --> aborted
;
;----------------------------------------------------------------------*/
void X10Tx()
{

   //BYTE Remainder;

   if(bit_test(TxData3, 7)) //Is MSb of TxData3 set?
   {
      bit_set(TxBit); //Yes, set TxBit
      /* if TxBit set then */
      bit_set(STATUS, 0); //set the carry bit
   }
   else
   {
      bit_clear(TxBit);
      bit_clear(STATUS, 0); //Clear the carry bit
   }

   //            -       ------------
   //       <-- |C| <-- | REGISTER F | <--
   //       |    -       ------------     | RLF F, 0 instruction
   //       |-----------------------------| C = Carry Bit (Status Register)

#asm
      rlf   TxData0, f
      rlf   TxData1, f
      rlf   TxData2, f
      rlf   TxData3, f
#endasm

   NumTxBits--;

   if(NumTxBits == 0)//Have all the bits been sent?
   {
      if(bit_test(SecondTxFlag))//was this the second transmission?
      {
         output_high(RED_LED);//Yes, Turn off LED1
         bit_clear(X10TxFlag); //Yes, transmit is done
         //if(bit_test(TxCommandFlag))
         //{
            //DimmerCount--;
           // if(DimmerCount == 0)
           // {
           //    TxPackageSendOK = TRUE;
               output_high(YELLOW_LED);//Yes, Turn off LED2
            //}
         //}
      }
      else
      {/* set up for second transmission */
         bit_set(SecondTxFlag); //Set the flag to indicate second transmission
         RotateTxData(10); //Reposition data at left end of buffer

         /* if wait for 3 cycles then */
         if(bit_test(Wait3CyclesFlag))
         {
            NumTxBits = 28;   //Send 22 bits again plus 6 zeros
         }
         else //if Dim/Bright Command, not wait for 3 cycles
         {
            NumTxBits = 22; //parece nao precisar de ser soh depois do endereco
         }
      }
   }

   return;
}

/*;----------------------------------------------------------------------
; X10Rx
;----------------------------------------------------------------------*/
void X10Rx()
{
   unsigned BYTE Temp_data;

   bit_clear(STATUS, 0); //Clear the carry bit

   if(bit_test(RxBit)) //Was the 120kHz signal present when ReceiveX10Bit was called?
   {
      bit_set(STATUS, 0); //Yes, set the carry bit
   }

   /* Rotate the received data into the RxData registers */
#asm
   rlf   RxData0, f
   rlf   RxData1, f
   rlf   RxData2, f
   rlf   RxData3, f
#endasm

   Temp_data = RxData3;
   Temp_data &= 0b00001111;   //Mask off the 4 MSB's
   Temp_data ^= 0b00001110;    //Check for the Start Code '1110'

#ifdef PRINT_IN_OUT
   if(bit_test(WaitFlag))
   {
      printf("R0x%x 0x%x 0x%x 0x%x\n ", RxData3, RxData2, RxData1, RxData0);
      bit_clear(WaitFlag);
   }
#endif


   if(Temp_data == 0)//Is the start code present?
   {

//---When Start Code is detected in RxData3 [xxxx1110] perform the below operations:
      RxHouse2 = RxHouse1; //transfer contents of RxHouse1 to RxHouse2
      RxKey2 = RxKey1; //transfer contents of RxKey1 to RxKey2

      RxHouse1 = RxData2; //Store RxData2 in RxHouse1
      RxKey1 = RxData1; //Store RxData1 in RxKey1

      //printf("R0x%x 0x%x 0x%x 0x%x\n ", RxData3, RxData2, RxData1, RxData0);
      //printf("RxH2 K2 = 0x%x 0x%x\n", RxHouse2, RxKey2);
      //printf("RxKey1 = 0x%x\n", RxKey1);
      //printf("RxKey2 = 0x%x\n", RxKey2);

      if(RxHouse1 == RxHouse2)//Do last two received house codes match?
      {


/* receiving only the second half of the command */
//#if USING_TW523
         //if(bit_test(X10TxFlag)) //Is the TxBit set?
         //{
            //if((RxKey2 == TxUnit))// && (RxKey1 == TxFunction))
            //{
               //output_low(YELLOW_LED);
               //printf("RxKey2 = %x\n", RxKey2);
               //printf("RxKey1 = 0x%x\n", RxKey1);
            //}
         //}
//#endif

         if(RxKey1 == RxKey2)//Do last two received key codes match?
         {
            Temp_data = RxData0;
            Temp_data &= 0b11000000; //Mask off the 6 LSB's
            Temp_data ^= 0b01000000; //Check for the Unit address suffix '01'

            //output_low(YELLOW_LED);

            if(Temp_data == 0) //Is the Unit address suffix present?
            {
               //Yes, then address has been received.
               RxUnit = RxKey2; //Transfer value in RxKey2 to RxUnit
               bit_clear(RxCommandFlag); //Clear command received flag since address was received
               bit_set(X10RxFlag);
            }
            else //No, then check for Function suffix.
            {
               Temp_data = RxData0;
               Temp_data &= 0b11000000; //Mask off the 6 LSB's
               Temp_data ^= 0b10000000; //Check for the Function suffix '10'

               if(Temp_data == 0)
               {
                  RxFunction = RxKey2; //Transfer value in RxKey2 to RxFunction
                  bit_set(RxCommandFlag); //Set command received flag
                  bit_set(X10RxFlag);
                  //output_low(YELLOW_LED);
               }
               else
               {
                  //No, then neither command or address has been properly received
                  printf("neither X10 command or address has been properly received\n");
               }
            }
         }
      }
   }

   return;
}

/*;----------------------------------------------------------------------
;ZeroCrossing
;----------------------------------------------------------------------*/
void ZeroCrossing()
{

   //disable_interrupts(INT_TIMER0);
   //disable_interrupts(INT_TIMER1);
   //disable_interrupts(INT_EXT);
   //disable_interrupts(GLOBAL);

   //TransmitX10Bit();      //Transmit value of TxBit

   //ReceiveX10Bit();      //Update value of RxBit

   if(bit_test(X10TxFlag))//If X10 transmit flag is set
   {
      X10Tx(); //execute X10Tx macro
      TransmitX10Bit();      //Transmit value of TxBit
   }

   ReceiveX10Bit();      //Update value of RxBit


   //bit_clear(PIE1, TMR1IE);//clear the Timer0 interrupt flag
   X10Rx(); //execute X10Rx macro


   ToggleInterruptEdge(); //set rising or falling edge for next zero-crossing
   bit_clear(INTCON, INTF); //clear the RB0/INT flag

   //enable_interrupts(INT_TIMER0);
   //enable_interrupts(INT_TIMER1);
   //enable_interrupts(INT_EXT);
   //enable_interrupts(GLOBAL);

   return;

}

/*;----------------------------------------------------------------------
; EndX10Envelope
;---------------------------------------------------------------------*/
void EndX10Envelope()
{

#if USING_TW523
   output_low(XOUT);
#else
   setup_ccp1(CCP_OFF);
#endif

#ifdef 3_PHASE
   if(!bit_test(TxBit)) //Is TxBit zero?
   {
     disable_interrupts(INT_RTCC);//disable Timer0 interrupt
     bit_clear(INTCON, T0IF);//clear the Timer0 interrupt flag
   }
   else // TxBit is one
   {
      if (Phase < 3)
      {
         if(PhaseTime == 0)
         {
            PhaseTime = 1;
            set_timer0(0x75); //Set TMR0 up for 1,778ms delay to time duration of 120KHz burst.
         }
         else
         {
            PhaseTime = 0;
#if USING_TW523
            output_high(XOUT);
#else
            setup_ccp1(CCP_PWM);
#endif
            Phase++;
            set_timer0(0xB2); //Set TMR0 up for 1ms delay to time duration of 120KHz burst.
         }
         bit_clear(INTCON, T0IF); //Clear the Timer0 interrupt flag.
      }
   }
#else
   disable_interrupts(INT_RTCC);//disable Timer0 interrupt
   bit_clear(INTCON, T0IF);//clear the Timer0 interrupt flag
#endif

   return;
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
BYTE EncodeTable( BYTE EncodeValue )
{
   switch(EncodeValue)              //HOUSE   UNIT
   {
      case(0): return 0b01101001; // 'A'    1   b'0110'
         break;
      case(1): return 0b10101001; // 'B'    2   b'1110'
         break;
      case(2): return 0b01011001; // 'C'    3   b'0010'
         break;
      case(3): return 0b10011001; // 'D'    4   b'1010'
         break;
      case(4): return 0b01010110; // 'E'    5   b'0001'
         break;
      case(5): return 0b10010110; // 'F'    6   b'1001'
         break;
      case(6): return 0b01100110; // 'G'    7   b'0101'
         break;
      case(7): return 0b10100110; // 'H'    8   b'1101'
         break;
      case(8): return 0b01101010; // 'I'    9   b'0111'
         break;
      case(9): return 0b10101010; // 'J'   10   b'1111'
         break;
      case(10): return 0b01011010; // 'K'   11   b'0011'
         break;
      case(11): return 0b10011010; // 'L'   12   b'1011'
         break;
      case(12): return 0b01010101; // 'M'   13   b'0000'
         break;
      case(13): return 0b10010101; // 'N'   14   b'1000'
         break;
      case(14): return 0b01100101; // 'O'   15   b'0100'
         break;
      case(15): return 0b10100101; // 'P'   16   b'1100'
         break;
   }

   return(0);
}

/*;----------------------------------------------------------------------
; InitX10Tx
;----------------------------------------------------------------------*/
void InitX10Tx()
{

   TxData3 = 0b00001110; //load TxData3 with start code

   TxData2 = EncodeTable( TxHouse );//get corresponding X-10 code for house address and
                     //load encoded house address into TxData2

   if(bit_test(TxCommandFlag))//Is the transmit command flag set?
   {
      //Yes, then prepare to transmit command
      //If transmitting a command
      TxData1 = TxFunction; //load function code into TxData1
      TxData0 = 0b10000000; //load suffix for function code into TxData0
   }
   else //No, then prepare to transmit an address
   {
      //If transmitting an address

      TxData1 = EncodeTable( TxUnit );//get corresponding X-10 code for unit address and
                                      //load encoded unit address into TxData1
      TxData0 = 0b01000000; //load suffix for unit address into TxData0

   }

#ifdef PRINT_IN_OUT
   printf("0x%x 0x%x 0x%x 0x%x\n ", TxData3, TxData2, TxData1, TxData0);
   bit_clear(WaitFlag);
#endif

   //output_high(YELLOW_LED);

   //Rotate the TxData registers 4 times to the left to prepare for transmission
   RotateTxData(4); // strip the first 4 bits from X10 Start Header

   NumTxBits = 22;//set number of bits to transmit
   bit_clear(SecondTxFlag); //clear the 2ndTxFlag
   output_low(RED_LED);//turn on LED1

   return;
}


/*;----------------------------------------------------------------------
;DecodeHouseOrUnit
;   Converts true compliment format X-10 House or Unit code in WREG
;       to value (0-15).
;
;   Works forwards through EncodeTable, comparing value to decode
;   against encoded values.  When there is a match, returns the index to
;   EncodeTable that produced the match (a value from 0-15).
;   If no values match, sets the decoded value to FF.
;
;----------------------------------------------------------------------*/
BYTE DecodeHouseOrUnit(BYTE DecodeValue)
{
   BYTE TempValue, EncodeValue;

   EncodeValue = 0;

   do
   {
      TempValue = EncodeTable( EncodeValue );
      TempValue ^= DecodeValue;

      if(TempValue == 0)//Is decode value same as value from EncodeTable?
      {
         break;
      }

      EncodeValue++; //No, increment EncodeValue

   }
   while(EncodeValue < X10MaxUnits);

   if(EncodeValue >= X10MaxUnits)//Is EncodeValue past max ?
   {
      EncodeValue = 0xFF;   //Yes, set EncodeValue to FF
   }

   return(EncodeValue);

}

/*;----------------------------------------------------------------------
; SendX10AddressVar
;----------------------------------------------------------------------*/
void SendX10AddressVar()
{

   bit_clear(TxCommandFlag);//Clear the TxCommandFlag for sending address
   InitX10Tx();
   bit_set(X10TxFlag); //Set the X10TxFlag to begin transmission

   return;

}

/*;----------------------------------------------------------------------
; SendX10CommandVar
;----------------------------------------------------------------------*/
void SendX10CommandVar()
{

   bit_set(TxCommandFlag); //Set the TxCommandFlag for sending command
   InitX10Tx();
   bit_set(X10TxFlag); //Set the X10TxFlag to begin transmission

   return;

}

/*;----------------------------------------------------------------------
; ReceiveX10Message
;   -may be expanded to receive extended data
;----------------------------------------------------------------------*/
void ReceiveX10Message()
{

   RxHouse = DecodeHouseOrUnit(RxHouse2);//Convert received house code to 0-15

   if(bit_test(RxCommandFlag))//Was a command received?
   {
      RxFunction = RxKey2; //Yes, then key code is function code
   }
   else //No, then convert key code to unit address
   {
      RxUnit = DecodeHouseOrUnit(RxKey2);//Convert received key code to 0-15
   }

   bit_clear(X10RxFlag);

   return;

}

/*;----------------------------------------------------------------------
; SendX10Address (HouseConstant, UnitConstant)
;----------------------------------------------------------------------*/
/*
void SendX10Address(BYTE House, BYTE Unit)
{
   TxHouse = House;
   TxUnit =  Unit;

   bit_clear(TxCommandFlag);//Clear the TxCommandFlag for sending address
   InitX10Tx();
   bit_set(X10TxFlag); //Set the X10TxFlag to begin transmission

   return;

}*/

/*;----------------------------------------------------------------------
; SendX10Command (House, Function)
;----------------------------------------------------------------------*/
/*
void SendX10Command(BYTE House, BYTE Function)
{
   TxHouse = House; //Copy house argument into TxHouse
   TxFunction = Function; //Copy function argument into TxFunc

   bit_set(TxCommandFlag); //Set the TxCommandFlag for sending command
   InitX10Tx();
   bit_set(X10TxFlag); //Set the X10TxFlag to begin transmission

   return;


}
*/
