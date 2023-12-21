
#include "drv_app/triacapp.h"


/*;----------------------------------------------------------------------
;ProcessRXCommand()
;----------------------------------------------------------------------*/
void ProcessRXCommand()
{
   switch(RxFunction)
   {
      case(AllUnitsOff):
         TurnTriacOn;
      break;
      case(AllLightsOn):
         TurnTriacOff;
      break;
      case(AllLightsOff):
         TurnTriacOn;
      break;
      case(OnFnc):
         TurnTriacOff;
      break;
      case(OffFnc):
         TurnTriacOn;
      break;
      //case(Dim):
      //case(Bright):
   }

   return;
}

/*;----------------------------------------------------------------------
;ControlTriac- Checks received X10 messages and performs appropriate TRIAC action
;----------------------------------------------------------------------*/
void ControlTriac()
{

   ReceiveX10Message();

   //Was last house address received the correct house address?
   if(RXHouse == House) //Check House address
   {
      if(bit_test(RxCommandFlag)) //Was a command received?
      {
         //ProcessRXCommand(); ?? Is it really necessary here too?
         if(RxUnit == Unit) //Check Unit address
         {
            ProcessRXCommand();
         }
      }
   }

   return;
}

/*;----------------------------------------------------------------------
; InitTRIAC
;----------------------------------------------------------------------*/
void InitTRIAC()
{

   //bsf   TRIAC      ;make TRIAC pin an input, this is off-state for triac
   //bsf   PORTA, 5   ;set PORTA TRIAC pin high
   //set_tris_a(0b0010000);
   bit_set(TRISA, TRIAC_pin);

   output_bit(TRIAC_pin, 1); //bit_set(PORTA, TRIAC_pin);

   return;
}
