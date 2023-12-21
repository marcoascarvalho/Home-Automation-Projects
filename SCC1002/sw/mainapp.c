/*************************************************************************
 *
 *    Home Automation Project - main App function
 *
 *    (c) Copyright HomeAuto
 *
 *    File name   : mainapp.c
 *    Description : Define a main function to control HA Project 
 *                  
 *    History :
 *    1.1 Date        : Feb 27, 2012
 *        Author      : Marco Carvalho
 *        Description : Create
 *
 *    1.2 Date        : May 13, 2015
 *        Author      : Marco Carvalho
 *        Description : Include Relay control instead TRIAC. Include turn
 *                      ON/OFF Display backlight
 * 
 *    1.3 Date        : March 27, 2016
 *        Author      : Marco Carvalho
 *        Description : Some modifications in HA Prot. Tested with other commands
 *
 *    $Revision: 1.3 $
**************************************************************************/

#define USE_BOOTLOADER FALSE

#define USE_LCD TRUE

#define USE_KEYBOARD TRUE

#define USE_HAPROT TRUE

/*Habilita o sensor de temperatura */
#define USE_TEMPERATURE TRUE

/*Habilita o sensor de luz */
#define USE_LIGHT_SENSOR TRUE

/*Habilita o uso de triac na unidade controladora */
#define USE_TRIAC TRUE

/*Habilita o teste de dimmer, substituir pela funcao de dimmer apos testes */
#define DIMMER_TEST TRUE

#include "pic/16F877.h"

#device ICD=TRUE

#include "pic/pins.h"
#include "app/mainapp.h"


/***********************************************************************
 *   Global Variables                                                  *
 ***********************************************************************/

BYTE ControlFlags = 0x00; 

BYTE DisplayFlags;
BYTE MenuState, MenuSubState;
BYTE UpCount, DownCount;
BYTE L1, L2, Prompt;
BYTE Last_PORTB;

BYTE House, Unit, CurrentUnit;

BYTE UnitToConfig, HouseToConfig;

BYTE HoursToConfig, MinsToConfig;

//unsigned long On_Flags;
//unsigned long Off_Flags;

unsigned BYTE On_Flags;
unsigned BYTE Off_Flags;

#include "drv_app/X10Lib.c"

//BYTE UnitLastState[X10MaxUnits];

#include "drv/eeprom.c"

#if USE_TEMPERATURE
   #include "drv/temperature.c"
#endif

#include "drv/clock.c"

#if DIMMER_TEST
   #include "drv_app/dimmerTestapp.c"
#endif

#if USE_HAPROT
   #include "drv_app/haprot.c"
#endif

#if USE_LCD
   #include "drv_app/lcdapp.c"
   //#include "drv_app/menuapp.c"
#endif

#if USE_KEYBOARD
   #include "drv/keyboard.c"
#endif

#if USE_TRIAC
   #include "drv_app/triacapp.c"
#endif

#if USE_LCD
   //#include "drv_app/lcdapp.c"
   #include "drv_app/menuapp.c"
#endif

#if USE_LIGHT_SENSOR
   #include "drv/light_sensor.c"
#endif

#include "app/initapp.c"

//BYTE DimCommand;

//BYTE TxPackageSendOK;

//BYTE NextDuskHours, NextDuskMins, NextDawnHours, NextDawnMins;

/*;**********************************************************************
;   External declarations                                             *
;***********************************************************************/
//extern   BYTE ZeroCrossing, InitX10Tx, DecodeHouseOrUnit, EndX10Envelope, InitX10Variables;
//extern   BYTE X10Flags, TxHouse, TxUnit, TxFunction, RxHouse, RxUnit, RxFunction, RxHouse2, RxKey2;


//#if USE_BOOTLOADER
//   #ORG 0x1F00,0x1FFF //for the 8k 16F876/7
//   void loader()
//   {

      //Bootload code goes here

//   }
//#endif
         
/*;*****************
Uni   tControlCode   code   0x0e00
;******************/

//#ORG 0x0e00

/*;----------------------------------------------------------------------
;SetOnOffFlag-
;----------------------------------------------------------------------*/
void SetOnOffFlag(BOOLEAN aOn_or_Off, BYTE aUnitToSet)
{

   unsigned BYTE AuxUnit;

   AuxUnit = aUnitToSet-1;

   if (aOn_or_Off == ON) 
   {
      bit_set(On_Flags, AuxUnit);
   }
   else 
   {
      bit_set(Off_Flags, AuxUnit);
   }

   return;

}

/*;----------------------------------------------------------------------
;ClearOnOffFlag-
;----------------------------------------------------------------------*/
void ClearOnOffFlag(BOOLEAN aOn_or_Off, BYTE aUnitToClear)
{

   if(aOn_or_Off == ON) 
   {
      bit_clear(On_Flags, aUnitToClear);
   }
   else       
   {
      bit_clear(Off_Flags, aUnitToClear);
   }

   /* Clear UnitSetFlag bit */
   bit_clear(DisplayFlags, UnitSetFlag);

   return;

}

/*;----------------------------------------------------------------------
;SetupTxFunction
;----------------------------------------------------------------------*/
void SetupTxFunction(BOOLEAN aOn_or_Off, BYTE aX10Function, BYTE aUnitToSet)
{
 
   TxFunction = aX10Function;
   SetOnOffFlag(aOn_or_Off, aUnitToSet);
   bit_set(DisplayFlags, UnitSetFlag);
   
   /* Store the last state send to the unit */
   //UnitLastState[aUnitToSet] = (BYTE) aOn_or_Off;

   return;
   
}

/*;----------------------------------------------------------------------
;TurnUnitOnOff
;----------------------------------------------------------------------*/
void TurnUnitOnOff(BOOLEAN aOn_or_Off, unsigned long aOnOffFlags)
{
   unsigned long OnOffFlag;

   OnOffFlag = aOnOffFlags & 0x0001; 

   if(OnOffFlag == ON) //is unit on or off flag set?
   {
      output_low(YELLOW_LED);//Yes, Turn on LED2
         
      /* if TxUnit is the Home Controller Unit then */
      if(TxUnit == Unit)
      {
         #if USE_TRIAC      
            if(aOn_or_Off == ON)
            {
               TurnTriacOff;  //negative logic
            }
            else
            {
               TurnTriacOn;
            }
         #endif

         ClearOnOffFlag(aON_or_OFF, TxUnit);

			output_high(YELLOW_LED);//Yes, Turn off LED2

      }
      else // if it´s another unit then
      {
         if(bit_test(DisplayFlags, X10AddressSent))//has address been sent?
         {
            SendX10CommandVar();      //yes, then send command
            bit_clear(DisplayFlags,X10AddressSent);

            #if DIMMER_TEST
               DimmerCount--;
               if(DimmerCount == 0)
               {
                  DimmerCount = 1;
               }
            #endif
            
            bit_set(Wait3CyclesFlag);
            ClearOnOffFlag(aON_or_OFF, TxUnit);
         }
         else
         {
            SendX10AddressVar(); // no, then send address
            bit_set(DisplayFlags, X10AddressSent);
         }
      }
   }

   return;

}

/*;----------------------------------------------------------------------
; ControlX10Units
;----------------------------------------------------------------------*/
void ControlX10Units()
{
   
   //unsigned long OnOffFlags;
   unsigned BYTE OnOffFlags;
   
   if(!SkipIfTxDone) //is transmission not in progress?
   {

      TxHouse = House; //Load TxHouse with current House address

      /*First, Turn ON the Units according to On_Flags */
      TxUnit = 0; //start with TxUnit=0 (Unit1)

      OnOffFlags = On_Flags; //Copy On_Flags into OnOffFlags
      do //no, continue checking On_Flags1
      {
          TurnUnitOnOff(ON, OnOffFlags);
          OnOffFlags = OnOffFlags>>1; // rotate to verify TXUnit set
          TxUnit++;
      }
      while(TxUnit < X10MaxUnits); //Have we reached Unit 16 yet?

      /*Second, Turn OFF the Units according to Off_Flags */
      TxUnit = 0; //start with TxUnit=0 (Unit1)

      OnOffFlags = Off_Flags; //Copy Off_Flags into OnOffFlags
      do //no, continue checking Off_Flags
      {
         TurnUnitOnOff(OFF, OnOffFlags);
         OnOffFlags = OnOffFlags>>1; // rotate to verify TXUnit set
         TxUnit++;
      }
      while(TxUnit < X10MaxUnits); //Have we reached Unit 16 yet?
      //yes, then all flags have been checked
   }

   return;
}

/****************************************************************************************************
;ISRRoutines
;****************************************************************************************************
/*;----------------------------------------------------------------------
; ISR-
;   Interrupt Service Routine.  Determine cause of interrupt and perform appropriate action.
;----------------------------------------------------------------------
;   Interrupt Sources:
;   TMR2:      Used with CCP1 to generate 120kHz PWM output,
;         Also used with postscaler to generate dimmer timing increments
;         of .125us for 32 interrupts/half-cycle.
;
;----------------------------------------------------------------------*/

/*
#INT_TIMER2
tmr2_isr()
{

   UpdateDisplay();
   ControlX10Units();

}
*/





/*;----------------------------------------------------------------------
; EnableInterrupts
;   - First, clear peripheral interrupt flags
;      - Enable Peripheral Interrupts
;      - Enable Global Interrupts
;----------------------------------------------------------------------*/
void EnableInterrupts()
{
   PIR1 = 0;
   PIR2 = 0;

   enable_interrupts(GLOBAL);   //Enable Global Interrupts
   return;
}

/*;----------------------------------------------------------------------
;ReadUnitOnOffMins- Retrieves Unit On or off Mins from DATA EEPROM and copies into.
;      Unit to retrieve is in UnitToRead
;----------------------------------------------------------------------*/
BYTE ReadUnitOnOffMins(BOOLEAN On_or_Off, BYTE aUnitToRead)
{

   BYTE AddrToRead, MinsRead;

   if(On_or_Off == ON) // if ON
   {
      AddrToRead = (aUnitToRead-1) + EEOnMins; //calculate address for unit's on minutes
   }
   else // if OFF
   {
      AddrToRead = (aUnitToRead-1) + EEOffMins; //calculate address for unit's off minutes
   }

   MinsRead = read_eeprom (AddrToRead);
   
   if((MinsRead&0x3F) < 60)
   {
      return(MinsRead);
   }
   else
   {
      return(MinsRead&0xC0);
   }

}

/*;----------------------------------------------------------------------
;ReadUnitOnOffHours- Retrieves Unit On or off Hours from DATA EEPROM and copies into.
;      Unit to retrieve is in UnitToRead
*                4 Bits     4 Bits 
* EEOnOffHOURS | On Hour | Off Hour |
*
;----------------------------------------------------------------------*/
BYTE ReadUnitOnOffHours(BOOLEAN On_or_Off, BYTE aUnitToRead)
{

   BYTE AddrToRead, HoursRead;

   AddrToRead = (aUnitToRead-1) + EEOnOffHours; //calculate address for unit's on or off hours

   HoursRead = read_eeprom (AddrToRead);

   if(On_or_Off == OFF) 
   {
      HoursRead &= 0x0F; //mask off the OnHours nibble
   }
   else 
   {
      HoursRead &= 0xF0; //mask off the OffHours nibble
      HoursRead >>=4; //shift OnHours to lower nibble of HoursToConfig
   }
   
   if(HoursRead < 13)
   {   
      return(HoursRead);
   }
   else
   {
      return(0);
   }
   
}


/*;----------------------------------------------------------------------
;WriteUnitOnOffTime- Writes Unit On or Off Time to DATA EEPROM and copies into.
;      Unit to write is in UnitToConfig
;----------------------------------------------------------------------*/
void WriteUnitOnOffTime(BOOLEAN aOn_or_Off)
{
   BYTE AddrToWrite, TempValue;

   if(aOn_or_Off == ON) // if ON
   {
      HoursToConfig = HoursToConfig<<4; //shift OnHours value to upper nibble of HoursToConfig
   }

   AddrToWrite = (UnitToConfig-1) + EEOnOffHours; //calculate address for unit's on or off hours

   TempValue = read_eeprom (AddrToWrite);

   if(aOn_or_Off == ON) // if ON
   {
      TempValue &= 0x0F; //mask off the OnHours nibble
   }
   else
   {
      TempValue &= 0xF0; //mask off the OffHours nibble
   }

   TempValue |= HoursToConfig; //combine upper nibble OnHours with lower nibble OffHours
   WriteData(AddrToWrite, TempValue); //WriteData(EEOnOffHours, TempValue);

   TempValue = MinsToConfig;

   if(bit_test(DisplayFlags, TempPM)) //if TempPM bit is set
   {
      bit_set(TempValue, EE_PM);
		//bit_clear(TempValue, EE_PM);
   }
   else
   {
      bit_clear(TempValue, EE_PM);
		//bit_set(TempValue, EE_PM);
   }

   if(CheckLightActivateBit(aOn_or_Off, UnitToConfig))
	{
      bit_set(TempValue, EE_LightActivate);
   }
   else
   {
      bit_clear(TempValue, EE_LightActivate);
   }

   if(aOn_or_Off == ON) // if ON
   {
      AddrToWrite = (UnitToConfig-1) + EEOnMins; //calculate address for unit's on minutes
   }
   else
   {
      AddrToWrite = (UnitToConfig-1) + EEOffMins; //calculate address for unit's off minutes
   }

   WriteData(AddrToWrite, TempValue);

   return;
}


/*;----------------------------------------------------------------------
;CheckLightActivate- does current light Activate Flag Unit flag is on?
;----------------------------------------------------------------------*/
BOOLEAN CheckLightActivateBit(BOOLEAN aOn_or_Off, BYTE aUnitToCheck)
{

   BYTE LightActivateToCheck = 0;
   
   LightActivateToCheck = ReadUnitOnOffMins(aOn_or_Off, aUnitToCheck);

   if(bit_test(LightActivateToCheck, EE_LightActivate)) //if EE_LightActivate bit is set
   {
      return TRUE; //if yes, return true
   }
   else
   {
      return FALSE;
   }

}

/*----------------------------------------------------------------------
;EnableOnAtDusk- Dusk = crepúsculo - anoitecer
;EnableOffAtDawn- Dawn = alvorecer - amanhecer
;----------------------------------------------------------------------*/
void EnableOnOffAtDuskDawn(BOOLEAN aON_or_OFF)
{

	BYTE TempValue, AddrToWrite;

	/* Recover OnOffMins value for the unit */
   	TempValue = ReadUnitOnOffMins(aON_or_OFF, UnitToConfig);

	if(bit_test(DisplayFlags, Okay))
   	{
    	bit_set(TempValue, EE_LightActivate);
   	}
   	else
   	{
    	bit_clear(TempValue, EE_LightActivate);
   	}

	if(aOn_or_Off == ON) // if ON
    {
    	AddrToWrite = (UnitToConfig-1) + EEOnMins; //calculate address for unit's on Minutes
   	}
   	else
   	{
    	AddrToWrite = (UnitToConfig-1) + EEOffMins; //calculate address for unit's off Minutes
   	}

   	WriteData(AddrToWrite, TempValue);	

	return;
}


/*;****************************************************************************************************
;Unit Control Routines
;****************************************************************************************************

/*;----------------------------------------------------------------------
;CheckPM- does present PM match unit's on-time or off-time PM?
;----------------------------------------------------------------------*/
BOOLEAN CheckPM(BOOLEAN aOn_or_Off)
{

   BYTE PMToCheck;
   
   PMToCheck = ReadUnitOnOffMins(aOn_or_Off, CurrentUnit);

   if(bit_test(DisplayFlags, PM)) //if PM is set
   {
      if(bit_test(PMToCheck, EE_PM)) //is EE_PM also set?
      {
         return TRUE; //if yes, set Match
      }
   }
   else
   {
      if(!bit_test(PMToCheck, EE_PM)) //is TempPM also clear?
      {
         return TRUE; //if yes, set Match
      }
   }

   return FALSE;

}

/*;----------------------------------------------------------------------
;CheckHours-
;----------------------------------------------------------------------*/
BOOLEAN CheckHours(BOOLEAN On_or_Off)
{

   Byte HoursToCheck;

   HoursToCheck = ReadUnitOnOffHours(On_or_Off, CurrentUnit);

   if (HoursToCheck == Hours) //Does HoursToCheck match Hours?
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
   
}

/*;----------------------------------------------------------------------
;CheckMins-
;----------------------------------------------------------------------*/
BOOLEAN CheckMins(BOOLEAN On_or_Off)
{
   
   BYTE MinsToCheck;

   MinsToCheck = (ReadUnitOnOffMins(On_or_Off, CurrentUnit))&0x3F;

   //bit_clear(ControlFlags, Match);

   if (MinsToCheck == Mins) //Does TempMins match Mins
   {
      //bit_set(ControlFlags, Match); //if yes, set Match
      return TRUE;
   }
   else
   {
      return FALSE;
   }

}


/*----------------------------------------------------------------------
;CheckUnitOnOffTime-
;----------------------------------------------------------------------*/
void CheckUnitOnOffTime(BOOLEAN aOn_or_Off)
{

   /* check if PM flag match */
   if(CheckPM(aOn_or_Off))
   {
      /* Check if Unit Hours Match */
      if(CheckHours(aOn_or_Off))
      {
         /* Check if Unit Minutes Match */
         if(CheckMins(aOn_or_Off))
         {
            if (aOn_or_Off == ON)
            {
               SetupTxFunction(aOn_or_Off, OnFnc, CurrentUnit);
               //TxFunction = OnFnc;
            }
            else
            {
               SetupTxFunction(aOn_or_Off, OffFnc, CurrentUnit);
               //TxFunction = OffFnc;
            }
         
            //SetOnOffFlag(aOn_or_Off, CurrentUnit);
            
            //bit_set(DisplayFlags, UnitSetFlag);
         
         }
      }
   }

   #if USE_LIGHT_SENSOR
      /* Light Level Condition */
      if(bit_test(ControlFlags, DuskFlag)) // if light is low than treshold
      {
         if(CheckLightActivateBit(aOn_or_Off, CurrentUnit)) // if LightActivate bit is ON
         {
            if (aOn_or_Off == ON)
            {
               /* If Unit is the same as Controller */
               if(CurrentUnit == Unit)
               {
                  #if USE_TRIAC
                     // Turn on the lights
                     TurnTriacOff;
                  #endif
               }
               else
               {
                  SetupTxFunction(aOn_or_Off, OnFnc, CurrentUnit);                  
                  //TxFunction = OnFnc;
                  //SetOnOffFlag(aOn_or_Off, CurrentUnit);                  
                  //bit_set(DisplayFlags, UnitSetFlag);
               }
            }
         }
      }
      else
      {
         if(bit_test(ControlFlags, DawnFlag)) // if light is high than treshold
         {
            if(CheckLightActivateBit(aOn_or_Off, CurrentUnit)) // if LightActivate bit is ON
            {
               if (aOn_or_Off == OFF)
               {
                  /* If Unit is the same as Controller */
                  if(CurrentUnit == Unit)
                  {
                     #if USE_TRIAC
                        // Turn off the lights
                        TurnTriacOn;
                     #endif
                  }
                  else
                  {
                     SetupTxFunction(aOn_or_Off, OffFnc, CurrentUnit);
                     //TxFunction = OffFnc;
                     //SetOnOffFlag(aOn_or_Off, CurrentUnit);                     
                     //bit_set(DisplayFlags, UnitSetFlag);
                  }
               }
            }
         }
      }
   #endif


   return;
}

/*;----------------------------------------------------------------------
;CheckOnOffTimesLightLevel- Checks the unit on and off times stored in EEPROM and
;                 compares against the present time.
;
;                 If there is a match, then a flag is set to turn ON or OFF
;                 the unit.
;
;        Flags are located in On_Flags1, On_Flags2, Off_Flags1, Off_Flags2
;----------------------------------------------------------------------*/
void CheckOnOffTimesLightLevel()
{

   #if USE_LIGHT_SENSOR
      /*Check Light Level */
      CheckLightLevel();
   #endif

   CurrentUnit = 1; //start at unit 0

   do
   {
      CheckUnitOnOffTime(ON); 
      CheckUnitOnOffTime(OFF);

      CurrentUnit++; //increment unit
   }
   while(CurrentUnit!=(X10MaxUnits+1)); //if unit not reached 16, repeat loop

   bit_clear(ControlFlags, CheckTimesFlag);

   return;
}

/*;----------------------------------------------------------------------
; Main Routine
;----------------------------------------------------------------------*/
void main()
{

   #if USE_KEYBOARD
      InitButtons();               //initialize button inputs
   #endif

   InitVariables();             //initialize variables
   InitX10Variables();
   InitTimer0();                //set up Timer0
   InitTimer1();                //selected when using external 32kHz osc as TMR1 clock source
   InitPWM();                   //set up PWM as 120KHz oscillator
   InitADC();                   //set up analog-to-digital converter for light sensor
   #if USE_TRIAC
      InitTRIAC();                 //initialize TRIAC to off condition
   #endif
   InitZeroXDetect();           //set up external interrupt as zero-crossing detector

   #if USE_LCD
      lcd_init();                   //initialize the LCD
      InitMainMenuState();         //initialize the state machine to MainMenu state
      UpdateDisplay();              //update display
   #endif

   #if USE_HAPROT
      /* enable RX IRQ */
      enable_interrupts(INT_RDA);
   #endif
   enable_interrupts(GLOBAL);          //enable interrupts

   printf("Welcome Home\n");
   output_high(YELLOW_LED);        //Turn off LED2

/****************************************************************************************************
;****************************************************************************************************
; Main Loop Code
;   This is the main loop which continually performs the following tasks:
;   -if a key press has occured, update the state
;   -if no key press has occured for over a minute, return to main menu
;   -if the a clock minute has elapsed, check the unit on and off times
;   -if an X10 message has been received, control the TRIAC
;   *****-if display update flag has been set, update display
;   *****-send X-10 control messages as necessary
;
;   The various control flags are set from within the Interrupt Service Routine
;****************************************************************************************************
;****************************************************************************************************/

   while (1)
   {

      #if USE_KEYBOARD      
         if (bit_test(ControlFlags, ButtonPress))
         {
            UpdateState();
         }
      
         if(bit_test(ControlFlags, MinElapsedFlag))
         {
            InitMainMenuState();
         }
      #endif

      /*Every minute check if some Unit had to be turned on or off */
      if(bit_test(ControlFlags, CheckTimesFlag))
      {
         CheckOnOffTimesLightLevel();
      }

      if(bit_test(DisplayFlags, UnitSetFlag))
      {
         ControlX10Units();
      }

      #if USE_TRIAC
         if(bit_test(X10RxFlag))
         {
            ControlTriac();
         }
      #endif

      #if USE_LCD
         if(bit_test(DisplayFlags, UpdateDisplayFlag))
         {
            UpdateDisplay();
         }
      #endif
      
      #if USE_HAPROT
         if(char_received == TRUE) //condition need to be tested
         {
            DecodeHACommand();
         }
      #endif

   }

}

