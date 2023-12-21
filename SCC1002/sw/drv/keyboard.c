

#define InitButtons() set_tris_b(ALL_INPUTS) //Set TRISB to all inputs

BYTE Debounced, Toggled, LastDebounced, Held;

BYTE ButtonPressLock;


/*;----------------------------------------------------------------------
;ButtonToggle
;   Macro called from ScanKeys routine.
;   Checks to see if each button has been pressed and then released, if so
;   then corresponding bit of Toggled is clear, otherwise it's set.
;
;
;----------------------------------------------------------------------*/
void ButtonToggle()
{
   // If button was pressed and is now not pressed
   if(!bit_test(LastDebounced, BTN_MENU) && bit_test(Debounced, BTN_MENU))
   {
      bit_clear(Toggled, BTN_MENU);
   }
   else
   {
      bit_set(Toggled, BTN_MENU);
   }

   // If button was pressed and is now not pressed
   if(!bit_test(LastDebounced, BTN_UP) && bit_test(Debounced, BTN_UP))
   {
      bit_clear(Toggled, BTN_UP);
   }
   else
   {
      bit_set(Toggled, BTN_UP);
   }

   // If button was pressed and is now not pressed
   if(!bit_test(LastDebounced, BTN_DOWN) && bit_test(Debounced, BTN_DOWN))
   {
      bit_clear(Toggled, BTN_DOWN);
   }
   else
   {
      bit_set(Toggled, BTN_DOWN);
   }

   // If button was pressed and is now not pressed
   if(!bit_test(LastDebounced, BTN_ENTER) && bit_test(Debounced, BTN_ENTER))
   {
      bit_clear(Toggled, BTN_ENTER);
   }
   else
   {
      bit_set(Toggled, BTN_ENTER);
   }

  // If button was pressed and is now not pressed
   if(!bit_test(LastDebounced, BTN_EXIT) && bit_test(Debounced, BTN_EXIT))
   {
      bit_clear(Toggled, BTN_EXIT);
   }
   else
   {
      bit_set(Toggled, BTN_EXIT);
   }

   //If any button has toggled, if toggled xor FF >0 then set flag
   if((Toggled^0xFF) > 0)
   {
      bit_set(ControlFlags, ButtonPress);
   }

   return;

}

/*;----------------------------------------------------------------------
;ButtonHeld
;   Macro called from ScanKeys routine.
;   Checks to see if a button has been held for N consecutive KeyScan calls.
;   If it has, then the ButtonHeld flag for that button is cleared.  The flag
;   is set on the subsequent call of ButtonHeld.
;
;   This routine only checks Up and Down buttons.
;
;----------------------------------------------------------------------*/
void ButtonHeld()
{


   bit_set(Held, BTN_UP);
   bit_set(Held, BTN_DOWN);

   if(!bit_test(Debounced, BTN_UP)) //Is Up button pressed?
   {
      UpCount++;           //if so, increment held counter for Up button
      if(UpCount == 10)    //and check to see if it equals 10
      {
         bit_clear(Toggled, BTN_UP); //if so, clear the Held bit (active low)
         bit_clear(Held, BTN_UP);
         bit_set(Debounced, UP);     //and set the Debounced bit (so Toggle is not detected)
         bit_set(Last_PORTB, UP);
         UpCount=0;
      }
   }
   else
   {
      UpCount=0;
   }

   if(!bit_test(Debounced, BTN_DOWN)) //Is Up button pressed?
   {
      DownCount++;           //if so, increment held counter for Up button
      if(DownCount == 10)    //and check to see if it equals 10
      {
         bit_clear(Toggled, BTN_DOWN); //if so, clear the Held bit (active low)
         bit_clear(Held, BTN_DOWN);
         bit_set(Debounced, DOWN);     //and set the Debounced bit (so Toggle is not detected)
         bit_set(Last_PORTB, DOWN);
         DownCount=0;
      }
   }
   else
   {
      DownCount=0;
   }

   //If any button held, held xor 0 >0 then set flag
   if((Held^0xFF) > 0)
   {
      bit_set(ControlFlags, ButtonPress);
   }

   return;

}

/*
;----------------------------------------------------------------------
;MinuteElapsed
;   Macro called from ScanKeys routine.
;
;   If 1 minute elapses without a button press, set Minute Elapsed Flag (MinElapsedFlag)
;----------------------------------------------------------------------*/
void MinuteElapsed()
{

   if(bit_test(ControlFlags, ButtonPress)) //check to see if button has been pressed
   {
      if(Secs == 0) //if Seconds is 0, set new TimeLimit to 59
      {
         TimeLimit = 59;
      }
      else // otherwise, set new TimeLimit to Seconds - 1
      {
         TimeLimit = Secs - 1;
      }

      ButtonPressLock = TRUE; // lock the pressed button state
      
		/* Turn LCD On */
		TurnLCDOn;
		
		/* Turn LCD backlight On */
		TurnLCDBLOn;
      bit_clear(ControlFlags, InitFlag);

   }
   else
   {
      if(ButtonPressLock == TRUE)
      {
         if(Secs == TimeLimit)
         {
            bit_set(ControlFlags, MinElapsedFlag); //set the minute elapsed flag
            ButtonPressLock = FALSE; //minElapsed flag will be set just one time
            TurnLCDBLOff; //turn the lcd backlight off
				TurnLCDOff; //turn the lcd off
         }
      }
      else
      {
         bit_clear(ControlFlags, MinElapsedFlag); //clear the minute elapsed flag
      }
   }
   
   /* Turn off LCD backlight after one minute initialization 
      without press any button */
   if(bit_test(ControlFlags, InitFlag))
   {
      if(Secs == TimeLimit)
      {
         bit_clear(ControlFlags, InitFlag);
         TurnLCDBLOff; //turn the lcd backlight off
			TurnLCDOff; //turn the lcd off
      }
   }
      
   return;

}


/*;----------------------------------------------------------------------
;ScanKeys
;   UP     = Debounced_PORTB, 1
;   DOWN   = Debounced_PORTB, 2
;   MENU   = Debounced_PORTB, 3
;   ENTER  = Debounced_PORTB, 4
;   EXIT   = Debounced_PORTB, 5
;   ON_OFF = Debounced_PORTB, 5
;
;   Buttons are active low.  If a button is low for two scans in a row,
;   it is considered to be debounced.
;
;   When a button that was pressed is released, it is considered to be Toggled.
;   When a button is held for N iterations of ScanKeys, it is considered to be Held.
;
;----------------------------------------------------------------------*/
void ScanKeys()
{

   //UpdateClock();

   Debounced = input_b();  //read PORTB
   Debounced |= Last_PORTB; //Compare with PORTB from previous key scan
                            //If any bit is pulled low for 2 scans in a row,
                            //the button associated with that bit is considered
                            //to be pressed.
   //               EEMDU
   //               XNEOP
   //               ITNW
   //               TEUN
   //                R
   Debounced |= 0b11000001; //Mask the bits that aren't buttons (make them 1's)

   Last_PORTB = input_b(); //Save PORTB to compare in next scan.

   ButtonToggle();         //Check to see which pressed buttons have been released
   ButtonHeld();            //Check to see which buttons have been held up or down
   MinuteElapsed();         //Check to see if a minute has elapsed since last button pressed

   LastDebounced = Debounced; //save debounced to last debounced for next time

   return;

}

/****************************************************************************************************
;ISRRoutines
;****************************************************************************************************/
/*;----------------------------------------------------------------------
; ISR-
;   Interrupt Service Routine.  Determine cause of interrupt and perform appropriate action.
;----------------------------------------------------------------------
;   Interrupt Sources:
;   TMR1:       Time-keeping clock, also generates 1 interrupt every 25ms for keyscan.*/

#INT_TIMER1
void tmr1_isr()
{
   UpdateClock();
   ScanKeys(); // melhoria colocar interrupacao por sensibilidade na porta B
}



