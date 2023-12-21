
#include "drv_app/lcdapp.h"
#include "drv/lcd216.c"

BYTE BCDH, BCDL;


/*;****************************************************************************************************
MessageDisplayCode   code   0x1800 ;Program Memory Page 3
;****************************************************************************************************
   goto   $      ;This is a trap to prevent runaway code execution
*/
//#ORG 0x1800


/*;----------------------------------------------------------------------
;DisplayAMPM
;   If PM or TempPM flag is 0 display 'AM', if 1 display 'PM'
;----------------------------------------------------------------------*/
void DisplayAMPM(BOOLEAN PM_or_TempPM)
{

   if(!bit_test(DisplayFlags, PM_or_TempPM))//If PM_or_TempPM flag is 0
   {
      lcd_putc("AM"); //display 'AM'
   }
   else
   {
      lcd_putc("PM"); //Otherwise, display 'PM'
   }

   return;
}

/*;----------------------------------------------------------------------
;DisplayOkay
;   If Okay flag is 0 display 'N', if 1 display 'Y'
;----------------------------------------------------------------------*/
void DisplayOkay()
{

   if(bit_test(DisplayFlags, Okay))//If Okay flag is 1
   {
      lcd_putc("Y    ");// display 'Y' for yes
   }
   else
   {
      lcd_putc("N    ");//Otherwise, display 'N' for no
   }

   return;
}


/***********************************************************************
;MessageDisplayCode2   code   0x1900
;**********************************************************************
   goto   $      ;This is a trap to prevent runaway code execution

/*;----------------------------------------------------------------------
;Bin2BCD - converts binary variable in W to BCD variable, stored in BCDH
;      and BCDL
;----------------------------------------------------------------------*/
void Bin2BCD(BYTE TempVar)
{
   signed BYTE Var;

   BCDL = TempVar;
   BCDH = 0;

   do
   {
      Var = BCDL-10; //Loop to subtract 10 from low byte and increment high byte
      if(Var >= 0)
      {
         BCDL = Var;
         BCDH++;
      }
   }while(Var >= 0); //while low byte is >= 0

   BCDL = BCDL + 0x30;      //add 0x30 ('0') to convert variables from 0-9 to ASCII
   BCDH = BCDH + 0x30;       //equivalent of '0' to '9'

    return;
}

/*;**********************************************************************
DisplayTables   code   0x1900
;**********************************************************************
   goto   $      ;This is a trap to prevent runaway code execution
;----------------------------------------------------------------------
; DisplayMessage
;   W->Index1 contains the number of the string to be displayed
;      0  "Welcome to X-10 "
;      1  "Select Function:"
;      2  "Set System Time "
;      3  "Set System Addr "
;      4  "Program Unit    "
;      5  "Set hrs "
;      6  "Set min "
;      7  "Set House "
;      8  "Set Unit "
;      9  "Okay? "
;      10 "Program On-Time "
;      11 "Program Off-Time"
;---below message tables are in DisplayTables2 area---
;      12 "                "
;      13 "On at Dusk?"
;      14 "Off at Dawn?"
;      15 "Set Light Sensor"
;
;   Index2 is used to point to each character within the table.
;----------------------------------------------------------------------*/
//#ORG 0x1900
/*;**********************************************************************
DisplayTables2   code   0x1a00      ;necessary to prevent tables from crossing page boundaries (ie: 19 to 1a)
;***********************************************************************/
/*;**********************************************************************
DisplayTables3   code   0x1b00      ;necessary to prevent tables from crossing page boundaries (ie: 19 to 1a)
;***********************************************************************/

/*;----------------------------------------------------------------------
;DisplayDigits - Takes value in W and displays as two BCD digits
;   ******************
;   *XX*
;   ******************
;   where X is 0-9
;----------------------------------------------------------------------*/
void DisplayDigits(BYTE Var)
{
   Bin2BCD(Var);

   lcd_putc(BCDH);
   lcd_putc(BCDL);

   return;
}


/*;----------------------------------------------------------------------
;DisplayAddr - Moves cursor to home of line 2,
;calls Bin2BCD to get ASCII equivalents of
;House & Unit, and then writes them to the LCD.
;   ******************
;   **
;   *A-XX *
;   ******************
;----------------------------------------------------------------------*/
void DisplayAddr()
{

   L2homeLCD();

   lcd_putc(HouseToConfig);
   lcd_putc("-");

   DisplayDigits(UnitToConfig);

   lcd_putc("   ");

   return;

}

/*;----------------------------------------------------------------------
;DisplayDimmerPercent - Moves cursor to home of line 2,
;calls Bin2BCD to get ASCII equivalents of
;House & Unit, and then writes them to the LCD.
;   ******************
;   **
;   *A-XX *
;   ******************
;----------------------------------------------------------------------*/
#if DIMMER_TEST
void DisplayDimmerPercent()
{

   L2homeLCD();

   lcd_putc(M_PercentOn);

   DisplayDigits(DimmerPercentage);

   lcd_putc("%  ");

   return;

}
#endif

/*;----------------------------------------------------------------------
;UpdatePrompt
;   Based on the value of Prompt, display the appropriate message
;   0 = " Set hrs"
;   1 = " Set min"
;   2 = " Okay? " and call DisplayOkay to display "Y" or "N"
;   3 = "Set Unit "
;   4 = "Set House"
;   5 = DisplayYN   (displays Y or N only, without "Okay?" prompt)
;----------------------------------------------------------------------*/
void UpdatePrompt()
{

   switch(Prompt)
   {
      case P_SetHrs:
         lcd_putc(M_SetHours);
         break;
      case P_SetMin:
         lcd_putc(M_SetMin);
         break;
      case P_Okay   :
         lcd_putc(M_Okay);
         DisplayOkay(); //If prompt is Okay?, also display Y or N
         break;
      case P_SetUnit   :
         lcd_putc(M_SetUnit);
         break;
      case P_SetHouse:
         lcd_putc(M_SetHouse);
         break;
      case P_DisplayYN:
         DisplayOkay();
         break;
   }

   return;
}


/*;----------------------------------------------------------------------
;DisplayTime - Moves cursor to home of line 2,
;calls Bin2BCD and displays hours, minutes, seconds as shown below.
;   ******************
;   **
;   *HH:MM:SS        *
;   ******************
;----------------------------------------------------------------------*/
void DisplayTime()
{

   L2homeLCD();

   DisplayDigits(Hours); //Display Hours
   lcd_putc(":"); //Display colon
   DisplayDigits(Mins); //Display Mins
   lcd_putc(":"); //Display colon
   DisplayDigits(Secs); //Display Secs
   lcd_putc(" ");
   DisplayAMPM(PM);
   lcd_putc("  ");
   #if USE_TEMPERATURE
      DisplayDigits(TemperatureValue); //Display Temperature
      lcd_putc("C");
   #endif
   
   return;

}


/*----------------------------------------------------------------------
;DisplaySetTime - Moves cursor to home of line 2,
;calls Bin2BCD and displays hours, minutes, seconds as shown below.
;   ******************
;   **
;   *HH:MM        *
;   ******************
;----------------------------------------------------------------------*/
void DisplaySetTime()
{

   L2homeLCD();

   DisplayDigits(HoursToConfig); //Display HoursToConfig
   lcd_putc(":"); //Display colon
   DisplayDigits(MinsToConfig); //Display MinsToConfig
   lcd_putc(" ");
   DisplayAMPM(TempPM);
   lcd_putc("  ");

   return;

}

/*
;----------------------------------------------------------------------
;UpdateLine1
;   Based on the value of L1, display the appropriate message
;   0 = "Welcome to X-10 "
;   1 = "Select Function:"
;   2 = "Set System Time "
;   3 = "Set System Addr "
;   4 = "Program Unit    "
;   5 = "Program On-Time "
;   6 = "Program Off-Time"
;   7 = "Set Light Sensor"
;----------------------------------------------------------------------*/
/*
#define L1Welcome             0
#define L1SelectFunction    1
#define L1SetSystemTime       2
#define L1SetSystemAddr       3
#define L1ProgramUnit       4
#define L1ProgramOnTime       5
#define L1ProgramOffTime    6
#define L1SetLightSensor    7
#define L1SetDimmerLight    8
*/

void UpdateLine1()
{

   L1homeLCD();

   switch(L1)
   {
      case L1Welcome:
         lcd_putc(M_Welcome);
         break;
      case L1SelectFunction:
         lcd_putc(M_SelectFunction);
         break;
      case L1SetSystemTime:
         lcd_putc(M_SetSystemTime);
         break;
      case L1SetSystemAddr:
         lcd_putc(M_SetSystemAddr);
         break;
      case L1ProgramUnit:
         lcd_putc(M_ProgramUnit);
         break;
      case L1ProgramOnTime:
         lcd_putc(M_ProgramOnTime);
         break;
      case L1ProgramOffTime:
         lcd_putc(M_ProgramOffTime);
         break;
      #if USE_LIGHT_SENSOR
         case L1SetLightSensor:
            lcd_putc(M_SetLightSensor);
            break;
      #endif
      #if DIMMER_TEST
         case L1SetDimmerLight:
            lcd_putc(M_SetDimmerLight);
            break;
      #endif
      case L1TurnUnitOn:
         lcd_putc(M_TurnUnitOn);
         break;
      case L1TurnUnitOff:
         lcd_putc(M_TurnUnitOff);
         break;
   }

   return;
}

/*;----------------------------------------------------------------------
;UpdateLine2
;   Based on the value of L2, display the appropriate message
;   0 = "Set System Time "
;   1 = "Set System Addr "
;   2 = "Set Light Sensor"
;   3 = "Program Unit    "
;   4 = DisplayTime (12:00:00 AM format)
;   5 = DisplaySetTime (12:00 AM format)
;   6 = DisplayAddr (A-00 format)
;   7 = "On at Dusk?"
;   8 = "Off at Dawn?"
;   If PromptUpdateFlag is set, UpdatePrompt is called
;----------------------------------------------------------------------*/
void UpdateLine2()
{
   L2homeLCD();

   switch(L2)
   {
      case L2SetSystemTime:
         lcd_putc(M_SetSystemTime);
         break;
      case L2SetSystemAddr:
         lcd_putc(M_SetSystemAddr);
         break;
      #if USE_LIGHT_SENSOR
         case L2SetLightSensor:
            lcd_putc(M_SetLightSensor);
            break;
      #endif
      case L2ProgramUnit:
         lcd_putc(M_ProgramUnit);
         break;
      case L2DisplayTime:
         DisplayTime();
         break;
      case L2DisplaySetTime:
         DisplaySetTime();
         break;
      case L2DisplayAddr:
         DisplayAddr();
         break;
      #if USE_LIGHT_SENSOR
         case L2OnAtDusk:
            lcd_putc(M_OnAtDusk);
            break;
         case L2OffAtDawn:
            lcd_putc(M_OffAtDawn);
            break;
      #endif
      #if DIMMER_TEST
         case L2SetDimmerLight:
            lcd_putc(M_SetDimmerLight);
            break;
         case L2DisplayDimmerPercent:
            DisplayDimmerPercent();
            break;
      case L2PercentOkay:
         lcd_putc(M_PercentOkay);
         break;
      #endif
      case L2TurnUnitOn:
         lcd_putc(M_TurnUnitOn);
         break;
      case L2TurnUnitOff:
         lcd_putc(M_TurnUnitOff);
         break;
   }

   return;

}

/*;----------------------------------------------------------------------
;UpdateDisplay
;   If L1UpdateFlag is set, then update line 1
;   Always update line 2
;----------------------------------------------------------------------*/
void UpdateDisplay()
{

   if(bit_test(DisplayFlags, L1UpdateFlag))
   {
      UpdateLine1();
   }

   UpdateLine2();

   if(bit_test(DisplayFlags, PromptUpdateFlag))
   {
      UpdatePrompt();
   }

   bit_clear(DisplayFlags, UpdateDisplayFlag);
   bit_clear(DisplayFlags, L1UpdateFlag);
   bit_clear(DisplayFlags, PromptUpdateFlag);
   
   return;
}

/*;****************************************************************************************************
MenuNav   code   0x0800
;*****************************************************************************************************/
//#ORG 0x0800


/*;----------------------------------------------------------------------
;InitMainMenuState
;----------------------------------------------------------------------*/
void InitMainMenuState()
{

   MenuState = 0;
   MenuSubState = 0;

   //Update display settings
   L1 = L1Welcome;

   bit_set(DisplayFlags, L1UpdateFlag);
   L2 = L2DisplayTime;

   return;

}
