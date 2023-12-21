/*;****************************************************************************************************
 Light Sensor Routines
****************************************************************************************************/

BYTE DawnCount, DuskCount;

unsigned BYTE AmbientLevel;

/*----------------------------------------------------------------------
 CheckForDusk --> Check Low Light
 treshold for low limit is 0x20
----------------------------------------------------------------------*/
void CheckForDusk()
{

   if(AmbientLevel < LowLightTreshold) //if ambient is darker than threshold
   {
      DuskCount++;
      if(DuskCount == MinutesToConfirmLightLevel) //if 5 consecutive minutes of light lower than threshold
      {
         bit_set(ControlFlags, DuskFlag); //then set LowLightFlag
         DuskCount = 0;
      }
   }
   else
   {
      DuskCount = 0; //clear the LowLightCounter
      bit_clear(ControlFlags, DuskFlag);
   }

   return;
}

/*;----------------------------------------------------------------------
; CheckForDawn --> Check High Light
; treshold for high limit is 0x50
;----------------------------------------------------------------------*/
void CheckForDawn()
{

   if(AmbientLevel > HighLightTreshold) //if Ambient is brighter than threshold
   {
      DawnCount++;
      if(DawnCount == MinutesToConfirmLightLevel) //if 5 consecutive minutes of light lower than threshold
      {
         bit_set(ControlFlags, DawnFlag); //then set HighLightFlag
         DawnCount = 0;
      }
   }
   else
   {
      DawnCount = 0; //clear the DawnLightCounter
      bit_clear(ControlFlags, DawnFlag);
   }

   return;
}

/*;----------------------------------------------------------------------
; ReadADC - Light
;----------------------------------------------------------------------*/
void ReadLightSensor()
{

   set_adc_channel (0);
   delay_us(10);
   AmbientLevel = read_adc();

   return;

}

/*;----------------------------------------------------------------------
;CheckLightLevel - Read LDR sensor (A0) and check for Dusk or Dawn
;
;----------------------------------------------------------------------*/
void CheckLightLevel()
{

   ReadLightSensor();

   CheckForDusk();
   CheckForDawn();

   //bit_clear(ControlFlags, CheckLightFlag);

   return;

}

/*;----------------------------------------------------------------------
; ProcessLightLevel() - If Dawn or Dusk Flag was set, Read LightActivate
; bit from Unit Home Controller and check if it´s set and in this case,
; perform TRIAC on or off actions
;----------------------------------------------------------------------*/
//void ProcessLightLevel()
//{
//
//   //BYTE _TempUnit;
//
//   //_TempUnit = TempUnit;
//
//   if(bit_test(ControlFlags, DuskFlag)) // if light is low than treshold
//   {
//      //TempUnit = Unit + 1;
//      //ReadUnitOnOffTime(TRUE); // ON - read LightActivate bit from the Unit
//      if(bit_test(ControlFlags, LightActivate)) // if LightActivate bit is ON
//      {
//         /* If Unit is the same as Controller */
//         //if(_TempUnit == Unit)
//         //{
//            // Turn on the lights
//            #if USE_TRIAC
//               TurnTriacOff;
//            #endif
//         //}
//      }
//   }
//   else
//   {
//      if(bit_test(ControlFlags, DawnFlag)) // if light is high than treshold
//      {
//         //TempUnit = Unit + 1;
//         //ReadUnitOnOffTime(FALSE); // OFF - read LightActivate bit from the Unit
//         if(bit_test(ControlFlags, LightActivate)) // if LightActivate bit is ON
//         {
//            // Turn off the lights
//            //if(_TempUnit == Unit)
//            //{
//               #if USE_TRIAC
//                  TurnTriacOn;
//               #endif
//            //}
//         }
//      }
//   }
//
//   //TempUnit = _TempUnit;
//
//   return;
//
//}

/*;----------------------------------------------------------------------
; ToggleLightActivate:
;   Toggles LightActivate bit from on to off, off to on
;----------------------------------------------------------------------*/
//void ToggleLightActivate()
//{

//   if(bit_test(ControlFlags, LightActivate))
//   {
//      bit_clear(ControlFlags, LightActivate);
//   }
//   else
//   {
//      bit_set(ControlFlags, LightActivate);
//   }

//   return;

//}
