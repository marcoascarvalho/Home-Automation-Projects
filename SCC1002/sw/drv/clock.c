

#define TICKS_FOR_1_SECOND 40
#define SECONDS_PER_MINUTE 60
#define MINUTES_PER_HOUR   60
#define HOUR_AM_PM         12

unsigned BYTE Hours, Mins, Secs, Ticks, TimeLimit; //Tick is 25ms


/*;----------------------------------------------------------------------
;UpdateClock -
;   Macro executed within ScanKeys routine.
;   update TMR1H, Hours, Secs, Mins, and set the UpdateDisplayFlag.
;----------------------------------------------------------------------*/
void UpdateClock()
{
	//Fast timer = 0xFCCE
	// delay timer = 0xFCC6
   //set_timer1(0xFCCE);  //preload Timer 1 for 25ms interrupt (see InitTimer1 for calculation)
	set_timer1(0xFCCD);

   Ticks++;   //Each tick = 25ms.  40 ticks = 1 sec.

   if (Ticks == TICKS_FOR_1_SECOND)
   {
      Ticks=0;
      Secs++;
      
      //Check temperature every second
      #if USE_TEMPERATURE
         GetTemperatureValue();
      #endif

      if(Secs == SECONDS_PER_MINUTE)
      {
         Secs=0;
         Mins++;

         //Every time Mins get incremented,
         //set a flag to check On/Off Times and check Light Level
         bit_set(ControlFlags, CheckTimesFlag);

         if(Mins == MINUTES_PER_HOUR)
         {
            Mins=0;
            Hours++;
            if(Hours == HOUR_AM_PM)
            {
               if(bit_test(DisplayFlags, PM))
               {
                  bit_clear(DisplayFlags, PM);
               }
               else
               {
                  bit_set(DisplayFlags, PM);
               }
            }
            else
            {
               if(Hours == 13)
               {
                  Hours = 1;
               }
            }
         }
      }
   }

   //Set the update display flag
   bit_set(DisplayFlags, UpdateDisplayFlag);
   bit_set(DisplayFlags, PromptUpdateFlag);

   return;
}

/*;----------------------------------------------------------------------
; TogglePM:
;   Toggles TempPM from PM to AM, AM to PM
;----------------------------------------------------------------------*/
void TogglePM()
{
   if(bit_test(DisplayFlags, TempPM))
   {
      bit_clear(DisplayFlags, TempPM);
   }
   else
   {
      bit_set(DisplayFlags, TempPM);
   }

   return;
}


/*----------------------------------------------------------------------
; AdjustHoursUp:
;   increments hours when called
;----------------------------------------------------------------------*/
void AdjustHoursUp()
{

   HoursToConfig++;

   if (HoursToConfig == 12) //If hours increments to 12 (so 11PM>12AM, and 11AM>12PM)
   {
      TogglePM(); //Update PM
   }
   else
   {
      if (HoursToConfig >= 13) //If hours increments to 13
      {
        HoursToConfig = 1; //then make hours = 1
      }
   }

   return;

}

/*;----------------------------------------------------------------------
; AdjustHoursDown:
;   decrements hours when called
;----------------------------------------------------------------------*/
void AdjustHoursDown()
{
   if(HoursToConfig == 0) //check to see if Hours is at zero
   {
      HoursToConfig++;
   }

   if(HoursToConfig == 1) //check to see if Hours is at 1
   {
      HoursToConfig = 12; //if it is at 1, load Hours with 12
   }
   else
   {
      if(HoursToConfig >= 13)
      {
         HoursToConfig =0;
      }
      else
      {
         HoursToConfig--; //if not, simply decrement Hours
      }
   }

   if (HoursToConfig == 11) //check to see if Hours has decremented to 11
   {
      TogglePM(); //if it is, then toggle PM (so 12PM>11AM, and 12AM>11PM)
   }

   return;

}

/*;----------------------------------------------------------------------
; AdjustHoursUpThruZero:
;   increments hours when called, 0 hours will disable unit
;----------------------------------------------------------------------*/
void AdjustHoursUpThruZero()
{

   HoursToConfig++;

   if (HoursToConfig == 12)//If hours increments to 12 (so 11PM>12AM, and 11AM>12PM)
   {
      TogglePM(); //Update PM
   }
   else if (HoursToConfig >= 13) //If hours increments to 13
   {
      HoursToConfig = 0; //then make hours = 0  (0 is available so unit can be disabled)
   }

   return;

}

/*;----------------------------------------------------------------------
; AdjustHoursDownThruZero:
;   decrements hours when called, 0 hours will disable unit
;----------------------------------------------------------------------*/
void AdjustHoursDownThruZero()
{

   if(HoursToConfig == 0) //check to see if Hours is at zero
   {
      HoursToConfig = 12;   //If hours are zero, then roll hours
   }
   else
   {
      if(HoursToConfig >= 13)
      {
         HoursToConfig = 1;
      }
      else
      {
         HoursToConfig--; //if not, simply decrement Hours
      }
   }

   if (HoursToConfig == 11) //check to see if Hours has decremented to 11
   {
      TogglePM(); //if it is, then toggle PM (so 12PM>11AM, and 12AM>11PM)
   }

   return;

}


/*;----------------------------------------------------------------------
; AdjustMinutesUp:
;   increments minutes when called
;----------------------------------------------------------------------*/
void AdjustMinutesUp()
{
   MinsToConfig++;

   if(MinsToConfig >= 60)
   {
      MinsToConfig = 0;
   }

   return;

}

/*;----------------------------------------------------------------------
; AdjustMinutesDown:
;   decrements minutes when called
;----------------------------------------------------------------------*/
void AdjustMinutesDown()
{

   if(MinsToConfig == 0) //check to see if MinsToConfig is at zero already
   {
      MinsToConfig=59; //if it is at zero, load Hours with 59
   }
   else
   {
      if(MinsToConfig > 60)
      {
         MinsToConfig=0;
      }
      else
      {
         MinsToConfig--; //if not, simply decrement MinsToConfig
      }
   }

   return;
}
