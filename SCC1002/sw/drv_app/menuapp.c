/*************************************************************************
 *
 *    Home Automation Project - menu App function
 *
 *    (c) Copyright HomeAuto
 *
 *    File name   : menuapp.c
 *    Description : Define a menu app when the HAC use LCD and Keyboard 
 *                  
**************************************************************************/

/*;----------------------------------------------------------------------
;CheckButtonsRoutine() - check if exit button was pressed during updating
;                        state and check button press and update lcd flags
;----------------------------------------------------------------------*/
void CheckButtonsRoutine()
{

   if(!bit_test(Toggled, BTN_EXIT)) //check exit button
   {
      InitMainMenuState();
   }

   bit_clear(ControlFlags, ButtonPress);
   bit_set(DisplayFlags, UpdateDisplayFlag);

   return;

}


/*;----------------------------------------------------------------------
;InitSelectFunctionState
;----------------------------------------------------------------------*/
void InitSelectFunctionState()
{

   MenuState = L1SelectFunction; //MainMenuState=SelectFunction
   MenuSubState = 0;

   L1 = L1SelectFunction;        //Update display settings
   bit_set(DisplayFlags, L1UpdateFlag);
   L2 = L2SetSystemTime;

   return;

}


/*;----------------------------------------------------------------------
;MainMenuState
;
;----------------------------------------------------------------------*/
void MainMenuState()
{

   L2 = L2DisplayTime;      //Update second line

   if(!bit_test(Toggled, BTN_MENU))
   {
      InitSelectFunctionState();
   }
   else
   {
      if(!bit_test(Toggled, BTN_ENTER)) //turn Triac on if press Enter
      {
         #if USE_TRIAC
            TurnTriacOn;                      //in main menu
         #endif
      }
      else
      {
         if(!bit_test(Toggled, BTN_EXIT))
         {
            #if USE_TRIAC
               TurnTriacOff;
            #endif
         }
         else
         {
            if(!bit_test(Toggled, BTN_UP))
            {
               #if DIMMER_TEST
                  //TempUnit = 3;
                  //AdjustDimmerPercentUp(Unit);
                  //DimmerControl();
                  //WriteData(EEDimmerPercent, DimmerPercent); //Record Dimmer Percentage;
               #endif
            }
            else
            {
               if(!bit_test(Toggled, BTN_DOWN))
               {
                  #if DIMMER_TEST
                     //TempUnit = 3;
                     //AdjustDimmerPercentDown(Unit);
                     //DimmerControl();
                     //WriteData(EEDimmerPercent, DimmerPercent); //Record Dimmer Percentage;
                  #endif
               }
            }
         }
      }
   }

   return;

}

/*;----------------------------------------------------------------------
; InitFunctionData
; Init Some Function Data to display  
;----------------------------------------------------------------------*/
void InitFunctionData(BYTE aMenuStateData, BYTE aL2Data, BYTE aPromptData)
{

   MenuState = aMenuStateData;
   MenuSubState = 0;

   bit_set(DisplayFlags, Okay); //set default Okay to Yes

   L1 = aMenuStateData;   //Update display settings
   bit_set(DisplayFlags, L1UpdateFlag); //update L1 state
   L2 = aL2Data;

   Prompt = aPromptData;
   bit_set(DisplayFlags, PromptUpdateFlag);

   return;
   
}

/*;----------------------------------------------------------------------
; AdjustHouseUp:
;   increments housecode when called
;----------------------------------------------------------------------*/
void AdjustHouseUp()
{
   HouseToConfig++;

   if((HouseToConfig-0x41) == X10MaxUnits)
   {
      HouseToConfig=0x41;
   }

   return;
}

/*;----------------------------------------------------------------------
; AdjustUnitUp:
;   increments unit when called
;----------------------------------------------------------------------*/
void AdjustUnitUp()
{
   UnitToConfig++;

   if(UnitToConfig >= (X10MaxUnits+1))
   {
      UnitToConfig=1;
   }

   return;
}

/*;----------------------------------------------------------------------
; AdjustHouseDown:
;   increments housecode when called
;----------------------------------------------------------------------*/
void AdjustHouseDown()
{
   if((HouseToConfig-0x41)==0) //check to see if HouseToConfig is at zero ("A") already
   {
      HouseToConfig=0x41 + (X10MaxUnits-1); //if it is at zero, load HouseToConfig with 15 ("P")
   }
   else
   {
      HouseToConfig--; //if not, simply decrement HouseToConfig
   }

   return;

}

/*;----------------------------------------------------------------------
; AdjustUnitDown:
;   increments unit when called
;----------------------------------------------------------------------*/
void AdjustUnitDown()
{
   if(UnitToConfig == 1) //check to see if TempUnit is at zero already
   {
      UnitToConfig=X10MaxUnits; //if it is at zero, load TempUnit with 15
   }
   else
   {
      UnitToConfig--; //if not, simply decrement TempUnit
   }

   return;
}

/*;----------------------------------------------------------------------
; ToggleOkay:
;   Toggles Okay from Yes to No, No to Yes
;----------------------------------------------------------------------*/
void ToggleOkay()
{

   if(bit_test(DisplayFlags, Okay))
   {
      bit_clear(DisplayFlags, Okay);
   }
   else
   {
      bit_set(DisplayFlags, Okay);
   }

   return;
}

/*;----------------------------------------------------------------------
;SelectFunctionState
;   User interface to select function.
;      UP- scrolls forward through functions
;      DOWN- scrolls backward through functions
;      MENU- scrolls forward through functions
;      ENTER- selects function
;      EXIT- returns to Main Screen
;----------------------------------------------------------------------*/
void SelectFunctionState()
{

   //check buttons and perform appropriate action
   if(!bit_test(Toggled, BTN_UP)||!bit_test(Toggled, BTN_MENU)) //check UP button
   {
      if(L2 == L2_SCROLL_MIN) //if L2 = L2_SCROLL_MIN roll up to the end 
      {
         L2 = L2_SCROLL_MAX;
         MenuSubState = L2_SCROLL_MAX;
      }
      else
      {
         L2--; //Update second line
         MenuSubState--;
      }
   }
   else
   {
      if(!bit_test(Toggled, BTN_DOWN)) //check DOWN button
      {
         if(L2 == L2_SCROLL_MAX) ////if L2 = L2_SCROLL_MAX roll down to the beggining 
         {
            L2 = L2_SCROLL_MIN;
            MenuSubState = L2_SCROLL_MIN;
         }
         else
         {
            L2++; //Update second line
            MenuSubState++;
         }

      }
      else
      {
         if(!bit_test(Toggled, BTN_ENTER)) //check ENTER button
         {
            
            HouseToConfig = House + 0x41;//Add 0x41 to convert from 0-15 to 'A'-'P'
            UnitToConfig = Unit + 1; //Display TempUnit 0-15 as '1'-'16'
            
            switch(MenuSubState) //Based on MenuSubState, call appropriate subroutine
            {
               case SFS0:
                  //InitSetSystemTimeState();
                  InitFunctionData(L1SetSystemTime, L2DisplaySetTime, P_SetHrs);
                  HoursToConfig = Hours;
                  MinsToConfig = Mins;
                  if(bit_test(DisplayFlags, PM))
                  {
                     bit_set(DisplayFlags, TempPM);
                  }
                  else
                  {
                     bit_clear(DisplayFlags, TempPM);
                  }
                  break;
               case SFS1:
                  //InitSetSystemAddrState();
                  InitFunctionData(L1SetSystemAddr, L2DisplayAddr, P_SetHouse);
                  //HouseToConfig = House + 0x41;//Add 0x41 to convert from 0-15 to 'A'-'P'
                  //UnitToConfig = Unit + 1; //Display TempUnit 0-15 as '1'-'16'
                  break;
               case SFS2:
                  //InitSetLightSensorState();
                  InitFunctionData(L1SetLightSensor, L2DisplayAddr, P_SetUnit); 
                  break;
               case SFS3:
                  //InitProgramUnitState();
                  //HouseToConfig = House + 0x41;;
                  InitFunctionData(L1ProgramUnit, L2DisplayAddr, P_SetUnit);
                  break;
               case SFS4:
                  //InitSetDimmerLightState();
                  InitFunctionData(L1SetDimmerLight, L2DisplayAddr, P_SetUnit);
                  break;
               case SFS5:
                  //InitTurnOnOffUnitState(ON); // Turn unit on
                  InitFunctionData(L1TurnUnitOn, L2DisplayAddr, P_SetUnit);
                  break;
               case SFS6:
                  //InitTurnOnOffUnitState(OFF); // Turn unit off
                  InitFunctionData(L1TurnUnitOff, L2DisplayAddr, P_SetUnit);
                  break;
            }
         }
      }
   }


   CheckButtonsRoutine();


/*;*********
;   bsf   INTCON, INTE      ;re-enable X-10 service
;**********/
//   enable_interrupts(INT_EXT);
   return;

}

/*;----------------------------------------------------------------------
;SetSystemHours
;----------------------------------------------------------------------*/
void SetSystemHours()
{
   
   if(!bit_test(Toggled, BTN_UP)) //check UP button
   {
      AdjustHoursUp();
   }
   else
   {
      if(!bit_test(Toggled, BTN_DOWN))
      {
         AdjustHoursDown();
      }
      else
      {
         if(!bit_test(Toggled, BTN_ENTER))
         {
            MenuSubState = 1;
            Prompt = P_SetMin;
            bit_set(DisplayFlags, PromptUpdateFlag);
         }
      }
   }

   CheckButtonsRoutine();

   return;
}

/*;----------------------------------------------------------------------
;SetSystemMins
;----------------------------------------------------------------------*/
void SetSystemMins()
{

   
   if(!bit_test(Toggled, BTN_UP)) //check UP button
   {
      AdjustMinutesUp();
   }
   else
   {
      if(!bit_test(Toggled, BTN_DOWN))
      {
         AdjustMinutesDown();
      }
      else
      {
         if(!bit_test(Toggled, BTN_ENTER))
         {
            MenuSubState = 2;
            Prompt = P_Okay;
            bit_set(DisplayFlags, PromptUpdateFlag);
         }
      }
   }

   CheckButtonsRoutine();

   return;
}

/*;----------------------------------------------------------------------
;SetSystemTime
;----------------------------------------------------------------------*/
void SetSystemTime()
{

   if(!bit_test(Toggled, BTN_UP)||!bit_test(Toggled, BTN_DOWN)) //check UP button
   {
      ToggleOkay();
      bit_set(DisplayFlags, PromptUpdateFlag);
   }
   else
   {
      if(!bit_test(Toggled, BTN_ENTER))
      {
         if(bit_test(DisplayFlags, Okay))
         {
            Hours = HoursToConfig;
            Mins = MinsToConfig;
            Secs = 0;
            
            if(bit_test(DisplayFlags, TempPM))
            {
               bit_set(DisplayFlags, PM);
            }
            else
            {
               bit_clear(DisplayFlags, PM);
            }
            
            InitMainMenuState();
         }
         else //if Okay is not 1, then return again to set hour without saving the values
         {
            InitFunctionData(L1SetSystemTime, L2DisplaySetTime, P_SetHrs);
         }
      }
   }

   CheckButtonsRoutine();

   return;
}



/*;----------------------------------------------------------------------
; SetSystemTimeState:
;   user interface to set system time (hours and minutes)
;   SubMenuState=0, initialize variables
;   SubMenuState=1, adjust system hours:
;      UP- increments hours
;      DOWN- decrements hours
;      ENTER- goes to adjust minutes
;      EXIT- returns to Select Function screen, without changing time
;   SubMenuState=2, adjust system minutes:
;      UP- increments minutes
;      DOWN- decrements minutes
;      ENTER- goes to confirm new time
;      EXIT- returns to SetSystemHours screen, without changing time
;   SubMenuState=3, confirm time:
;      UP- select between Yes and No
;      DOWN- select between Yes and No
;      ENTER- if Yes is selected, new time is saved, exit to Main screen
;         if No is selected, new time is not saved, exit to Main
;      EXIT- returns to Select Function screen, without changing time
;
;----------------------------------------------------------------------*/
void SetSystemTimeState()
{

   switch(MenuSubState) //Based on MenuSubState, call appropriate subroutine
   {
      case 0: //SSTS0
         SetSystemHours();
         break;
      case 1: //SSTS1
         SetSystemMins();
         break;
      case 2: //SSTS2
         SetSystemTime();
         break;
   }

   CheckButtonsRoutine();

   return;
}



/*;----------------------------------------------------------------------
;AdjustHouseAddress
;----------------------------------------------------------------------*/
//BYTE SetSystemAddrState0()
void AdjustHouseAddress()
{

   if(!bit_test(Toggled, BTN_UP)) //check UP button
   {
      AdjustHouseUp();
   }
   else
   {
      if(!bit_test(Toggled, BTN_DOWN))
      {
         AdjustHouseDown();
      }
      else
      {
         if(!bit_test(Toggled, BTN_ENTER))
         {
            MenuSubState = 1;
            Prompt = P_SetUnit;
            bit_set(DisplayFlags, PromptUpdateFlag);
         }
      }
   }

   CheckButtonsRoutine();

   return;
}

/*;----------------------------------------------------------------------
;AdjustUnitAddress
;----------------------------------------------------------------------*/
void AdjustUnitAddress()
{
   
   if(!bit_test(Toggled, BTN_UP)) //check UP button
   {
      AdjustUnitUp();
   }
   else
   {
      if(!bit_test(Toggled, BTN_DOWN))
      {
         AdjustUnitDown();
      }
      else
      {
         if(!bit_test(Toggled, BTN_ENTER))
         {
            MenuSubState = 2;
            Prompt = P_Okay;
            bit_set(DisplayFlags, PromptUpdateFlag);
         }
      }
   }

   CheckButtonsRoutine();

   return;
}

/*;----------------------------------------------------------------------
;SetSystemAddress
;----------------------------------------------------------------------*/
void SetSystemAddress()
{

   if(!bit_test(Toggled, BTN_UP)||!bit_test(Toggled, BTN_DOWN)) //check UP button
   {
      ToggleOkay();
      bit_set(DisplayFlags, PromptUpdateFlag);
   }
   else
   {
      if(!bit_test(Toggled, BTN_ENTER))
      {
         if(bit_test(DisplayFlags, Okay))
         {
            House = HouseToConfig - 0x41;
            WriteData(EEHouse, House); //Save new house address to EEPROM
            Unit = UnitToConfig - 1;
            WriteData(EEUnit, Unit); //Save new unit address to EEPROM
            InitMainMenuState();
         }
         else //if Okay is not 1, then return to set house again without saving the values
         {
            InitFunctionData(L1SetSystemAddr, L2DisplayAddr, P_SetHouse); 
         }
      }
   }

   CheckButtonsRoutine();

   return;
}


/*----------------------------------------------------------------------
; SetSystemAddrState:
;   user interface to set system address (house and units)
;   SubMenuState=0, initialize variables
;   SubMenuState=1, adjust house address:
;      UP- increments house address
;      DOWN- decrements house address
;      ENTER- goes to adjust unit address
;      EXIT- returns to Select Function screen, without changing system address
;   SubMenuState=2, adjust system unit address:
;      UP- increments system unit address
;      DOWN- decrements system unit address
;      ENTER- goes to confirm new system address
;      EXIT- returns to SetSystemHouse address screen, without changing system address
;   SubMenuState=3, confirm system address:
;      UP- select between Yes and No
;      DOWN- select between Yes and No
;      ENTER- if Yes is selected, new system address is saved, exit to Main screen
;         if No is selected, new system address is not saved, exit to Main
;      EXIT- returns to Select Function screen, without changing system address
;
;----------------------------------------------------------------------*/
void SetSystemAddrState()
{

   switch(MenuSubState) //Based on MenuSubState, call appropriate subroutine
   {
      case 0: //SSAS0:
         AdjustHouseAddress();//SetSystemAddrState0();
         break;
      case 1: //SSAS1:
         AdjustUnitAddress();//SetSystemAddrState1();
         break;
      case 2: //SSAS2:
         SetSystemAddress();
         break;
   }

   CheckButtonsRoutine();

   return;
}

/*;----------------------------------------------------------------------
; ProgramUnitState:
;   user interface to select unit to control
;   First, select unit to program:
;      UP- increments unit address
;      DOWN- decrements unit address
;      ENTER- goes to set on-time
;      EXIT- returns to Select Function screen, without changing unit address
;----------------------------------------------------------------------*/
void ProgramUnitState()
{

   L2 = L2DisplayAddr;
   bit_set(DisplayFlags, UpdateDisplayFlag);
   bit_set(DisplayFlags, L1UpdateFlag);

   if(!bit_test(Toggled, BTN_UP)) //check UP button
   {
      AdjustUnitUp();
   }
   else
   {
      if(!bit_test(Toggled, BTN_DOWN))
      {
         AdjustUnitDown();
      }
      else
      {
         if(!bit_test(Toggled, BTN_ENTER))
         {
            //InitProgramOnTimeState(TempUnit);
            HoursToConfig = ReadUnitOnOffHours(ON, UnitToConfig);
            //MinsToConfig = ReadUnitOnOffMins(ON, UnitToConfig)&0x3F;
            MinsToConfig = ReadUnitOnOffMins(ON, UnitToConfig);
				if ((MinsToConfig>>7)&0x01)
				{
					bit_set(DisplayFlags, TempPM);
				}
				else
				{
					bit_clear(DisplayFlags, TempPM);
				}
				MinsToConfig = MinsToConfig&0x3F;
				InitFunctionData(L1ProgramOnTime, L2DisplaySetTime, P_SetHrs);
         }
         else
         {
            if(!bit_test(Toggled, BTN_EXIT))
            {
               InitMainMenuState();
            }
         }
      }
   }

   CheckButtonsRoutine();

   return;
}

/*;----------------------------------------------------------------------
;ProgramOnTimeState0
;----------------------------------------------------------------------*/
void ProgramUnitHours()
{
   
   //check buttons and perform appropriate action
   if(!bit_test(Toggled, BTN_UP)) //check UP button
   {
      AdjustHoursUpThruZero();
   }
   else
   {
      if(!bit_test(Toggled, BTN_DOWN))
      {
         AdjustHoursDownThruZero();
      }
      else
      {
         if(!bit_test(Toggled, BTN_ENTER))
         {
            MenuSubState = 1;
            Prompt = P_SetMin;
            bit_set(DisplayFlags, PromptUpdateFlag);
         }
      }
   }

   return;

}

/*;----------------------------------------------------------------------
;ProgramOnOffTimeState1
;----------------------------------------------------------------------*/
void ProgramUnitMins()
{
   
   if(HoursToConfig != 0) //have hours been set to zero?
   {
      //check buttons and perform appropriate action
      if(!bit_test(Toggled, BTN_UP)) //check UP button
      {
         AdjustMinutesUp();
      }
      else
      {
         if(!bit_test(Toggled, BTN_DOWN))
         {
            AdjustMinutesDown();
         }
         else
         {
            if(!bit_test(Toggled, BTN_ENTER))
            {
               MenuSubState = 2;
               Prompt = P_Okay;
               bit_set(DisplayFlags, PromptUpdateFlag);
            }
         }
      }
   }
   else //if yes then minutes are 00 by default
   {
      MinsToConfig = 0;
      MenuSubState = 2;
      Prompt = P_Okay;
      bit_set(DisplayFlags, PromptUpdateFlag);
   }

   return;

}

/*;----------------------------------------------------------------------
;POnTS2
;----------------------------------------------------------------------*/
void ProgramUnitOnOffTime(BOOLEAN aOn_or_Off)
{

   //check buttons and perform appropriate action
   if(!bit_test(Toggled, BTN_UP)||!bit_test(Toggled, BTN_DOWN)) //check UP button
   {
      ToggleOkay();
      bit_set(DisplayFlags, PromptUpdateFlag);
   }
   else
   {
      if(!bit_test(Toggled, BTN_ENTER))
      {
         if(bit_test(DisplayFlags, Okay))
         {
            if(aOn_or_Off == ON)
            {
               WriteUnitOnOffTime(ON);
               HoursToConfig = ReadUnitOnOffHours(OFF, UnitToConfig);
               //MinsToConfig = ReadUnitOnOffMins(OFF, UnitToConfig)&0x3F; 
               MinsToConfig = ReadUnitOnOffMins(OFF, UnitToConfig);
					if ((MinsToConfig>>7)&0x01)
					{
						bit_set(DisplayFlags, TempPM);
					}
					else
					{
						bit_clear(DisplayFlags, TempPM);
					}
					MinsToConfig = MinsToConfig&0x3F;
					InitFunctionData(L1ProgramOffTime, L2DisplaySetTime, P_SetHrs);
            }
            else
            {
               WriteUnitOnOffTime(OFF); 
               InitFunctionData(L1ProgramUnit, L2DisplayAddr, P_SetUnit);
            }
            
         }
         else //if Okay is not 1, then return without saving the values
         {
            if(aOn_or_Off == ON)
            {
               HoursToConfig = ReadUnitOnOffHours(ON, UnitToConfig);
               //MinsToConfig = ReadUnitOnOffMins(ON, UnitToConfig)&0x3F; 
            	MinsToConfig = ReadUnitOnOffMins(ON, UnitToConfig);
					if ((MinsToConfig>>7)&0x01)
					{
						bit_set(DisplayFlags, TempPM);
					}
					else
					{
						bit_clear(DisplayFlags, TempPM);
					}
					MinsToConfig = MinsToConfig&0x3F;               
					InitFunctionData(L1ProgramOnTime, L2DisplaySetTime, P_SetHrs);
            }
            else
            {
               HoursToConfig = ReadUnitOnOffHours(OFF, UnitToConfig);
               //MinsToConfig = ReadUnitOnOffMins(OFF, UnitToConfig)&0x3F;                
               MinsToConfig = ReadUnitOnOffMins(OFF, UnitToConfig);
					if ((MinsToConfig>>7)&0x01)
					{
						bit_set(DisplayFlags, TempPM);
					}
					else
					{
						bit_clear(DisplayFlags, TempPM);
					}
					MinsToConfig = MinsToConfig&0x3F;
               InitFunctionData(L1ProgramOffTime, L2DisplaySetTime, P_SetHrs);
            }            
         }
      }
   }

   return;

}


/*;----------------------------------------------------------------------
; ProgramOnTimeState:
;   SubMenuState=0, set on-time hours:
;      UP- increments hours
;      DOWN- decrements hours
;      ENTER- goes to set on-time minutes
;      EXIT- returns to select unit screen, without changing on-time hours
;   SubMenuState=1, set on-time minutes:
;      UP- increments minutes
;      DOWN- decrements minutes
;      ENTER- goes to confirm on-time.
;      EXIT- returns to select unit screen, without changing on-time minutes
;   SubMenuState=2, confirm on-time:
;      UP- select between Yes and No
;      DOWN- select between Yes and No
;      ENTER- if Yes is selected, new on-time is saved, go to set off-time minutes.
;         if No is selected, new on-time is not saved, exit to Main
;      EXIT- returns to Select Function screen, without changing on-time for selected unit.
;----------------------------------------------------------------------*/
void ProgramOnTimeState()
{

   switch(MenuSubState) //Based on MenuSubState, call appropriate subroutine
   {
      case 0: //POnTS0:
         ProgramUnitHours(); //ProgramOnOffTimeState0
         break;
      case 1: //POnTS1:
         ProgramUnitMins();//ProgramOnOffTimeState1();
         break;
      case 2: //POnTS2:
         ProgramUnitOnOffTime(ON);
         break;
   }

   if(!bit_test(Toggled, BTN_EXIT)) //check exit button
   {
      InitFunctionData(L1ProgramUnit, L2DisplayAddr, P_SetUnit);
   }

   bit_clear(ControlFlags, ButtonPress);
   bit_set(DisplayFlags, UpdateDisplayFlag);

   return;
}


/*;----------------------------------------------------------------------
;SetLightSensorState0 - SLSS0
;----------------------------------------------------------------------*/
#if USE_LIGHT_SENSOR
void SetLightSensorUnit()
{

   //check buttons and perform appropriate action
   if(!bit_test(Toggled, BTN_UP))
   {
      AdjustUnitUp();
      bit_set(DisplayFlags, PromptUpdateFlag);
   }
   else
   {
      if(!bit_test(Toggled, BTN_DOWN))
      {
         AdjustUnitDown();
         bit_set(DisplayFlags, PromptUpdateFlag);
      }
      else
      {
         if(!bit_test(Toggled, BTN_ENTER))
         {
            bit_clear(DisplayFlags, Okay);
            if(CheckLightActivateBit(ON, UnitToConfig))
            {
               bit_set(DisplayFlags, Okay);
            }
            MenuSubState = 1;
            L2 = L2OnAtDusk; // Ligar ao anoitecer?
            Prompt = P_DisplayYN;
            bit_set(DisplayFlags, PromptUpdateFlag);
         }
      }
   }

   return;

}
#endif

/*;----------------------------------------------------------------------
;TurnOnAtDusk
;----------------------------------------------------------------------*/
#if USE_LIGHT_SENSOR
void TurnOnAtDusk()
{

   //check buttons and perform appropriate action
   if(!bit_test(Toggled, BTN_UP)||!bit_test(Toggled, BTN_DOWN))
   {
      ToggleOkay();
      bit_set(DisplayFlags, PromptUpdateFlag);
   }
   else
   {
      if(!bit_test(Toggled, BTN_ENTER))
      {
         EnableOnOffAtDuskDawn(ON); //EnableOnAtDusk();

         if(CheckLightActivateBit(OFF, UnitToConfig))
         {
            bit_set(DisplayFlags, Okay);
         }
         else
         {
            bit_clear(DisplayFlags, Okay);
         }
         
         MenuSubState = 2;
         L2 = L2OffAtDawn; // Desligar ao amanhecer?
         Prompt = P_DisplayYN;
         bit_set(DisplayFlags, PromptUpdateFlag);
      }
   }

   return;

}
#endif

/*;----------------------------------------------------------------------
;TurnOffAtDawn
;----------------------------------------------------------------------*/
#if USE_LIGHT_SENSOR
void TurnOffAtDawn()
{

   //check buttons and perform appropriate action
   if(!bit_test(Toggled, BTN_UP)||!bit_test(Toggled, BTN_DOWN))
   {
      ToggleOkay();
      bit_set(DisplayFlags, PromptUpdateFlag);
   }
   else
   {
      if(!bit_test(Toggled, BTN_ENTER))
      {
         EnableOnOffAtDuskDawn(OFF);//EnableOffAtDawn();
         MenuSubState = 0;
         L2 = L2DisplayAddr;
         Prompt = P_SetUnit;
         bit_set(DisplayFlags, PromptUpdateFlag);
      }
   }

   return;
}
#endif

/*;----------------------------------------------------------------------
; SetLightSensorState:
;   SubMenuState=0, set threshold
;      UP- increments threshold
;      DOWN- decrements threshold
;      ENTER- goes to confirm threshold
;      EXIT- returns to select unit screen, without changing off-time hours
;   SubMenuState=1, confirm threshold:
;      UP- select between Yes and No
;      DOWN- select between Yes and No
;      ENTER- if Yes is selected, new threshold is saved
;         if No is selected, new threshold is not saved, exit to Main
;      EXIT- returns to Select Function screen, without changing threshold
;----------------------------------------------------------------------*/
#if USE_LIGHT_SENSOR
void SetLightSensorState()
{

   switch(MenuSubState) //Based on MenuSubState, call appropriate subroutine
   {
      case 0: //SLSS0:
         SetLightSensorUnit(); //select unit
         break;
      case 1: //POffTS1:
         TurnOnAtDusk(); //turn unit on at dusk?
         break;
      case 2: //POffTS2:
         TurnOffAtDawn(); //turn unit off at dawn?
         break;
   }

   if(!bit_test(Toggled, BTN_EXIT)) //check exit button
   {
      InitMainMenuState();
   }

   bit_clear(ControlFlags, ButtonPress);
   bit_set(DisplayFlags, UpdateDisplayFlag);

   return;

}
#endif

/*;----------------------------------------------------------------------
;SetDimmerLightState0 - SDLS0
;----------------------------------------------------------------------*/
#if DIMMER_TEST
void SetDimmerLightUnit()
{

   //check buttons and perform appropriate action
   if(!bit_test(Toggled, BTN_UP))
   {
      AdjustUnitUp();
      bit_set(DisplayFlags, PromptUpdateFlag);
   }
   else
   {
      if(!bit_test(Toggled, BTN_DOWN))
      {
         AdjustUnitDown();
         bit_set(DisplayFlags, PromptUpdateFlag);
      }
      else
      {
         if(!bit_test(Toggled, BTN_ENTER))
         {
            MenuSubState = 1;
            L2 = L2DisplayDimmerPercent; // Display percentage

            DimmerPercentage = read_eeprom(EEDimmerPercent);
            if(DimmerPercentage > 99)
            {
               DimmerPercentage = 0;
            }
         }
      }
   }

   return;

}
#endif

/*;----------------------------------------------------------------------
;SetDimmerLightState1 - SDLS1
;----------------------------------------------------------------------*/
#if DIMMER_TEST
void AdjustDimmerPercent()
{

   //BYTE AddrToRead;
   
  // AddrToRead = (UnitToConfig-1) + EEDimmerPercent;
   
   //DimmerPercentage = read_eeprom(AddrToRead);
   
   //if(DimmerPercentage > 99)
   //{
   //   DimmerPercentage = 0;
   //}
   
   //check buttons and perform appropriate action
   if(!bit_test(Toggled, BTN_UP))
   {
      AdjustDimmerPercentUp();
      bit_set(DisplayFlags, PromptUpdateFlag);
   }
   else
   {
      if(!bit_test(Toggled, BTN_DOWN))
      {
         AdjustDimmerPercentDown();
         bit_set(DisplayFlags, PromptUpdateFlag);
      }
      else
      {
         if(!bit_test(Toggled, BTN_ENTER))
         {
            MenuSubState = 2;
            L2 = L2PercentOkay; // Porcentagem OK?
            Prompt = P_DisplayYN;
            bit_set(DisplayFlags, PromptUpdateFlag);
         }
      }
   }

   return;

}
#endif
/*;----------------------------------------------------------------------
;SetLightSensorState2 - SLSS2
;----------------------------------------------------------------------*/
#if DIMMER_TEST
void WriteDimmerData()
{

   //BYTE AddrToWrite;

   //check buttons and perform appropriate action
   if(!bit_test(Toggled, BTN_UP)||!bit_test(Toggled, BTN_DOWN))
   {
      ToggleOkay();
      bit_set(DisplayFlags, PromptUpdateFlag);
   }
   else
   {
      if(!bit_test(Toggled, BTN_ENTER))
      {
         if(bit_test(DisplayFlags, Okay))
         {
            DimmerControl();
            //AddrToWrite = (UnitToConfig-1) + EEDimmerPercent;
            //WriteData(AddrToWrite, DimmerPercentage); //Record Dimmer Percentage
            WriteData(EEDimmerPercent, DimmerPercentage); //Record Dimmer Percentage
            InitMainMenuState();
         }
         else //if Okay is not 1, then return without saving the values
         {
            InitFunctionData(L1SetDimmerLight, L2DisplayAddr, P_SetUnit);
         }
      }
   }


   return;

}
#endif

/*;----------------------------------------------------------------------
; SetDimmerLightState:
;   SubMenuState=0, set threshold
;      UP- increments threshold
;      DOWN- decrements threshold
;      ENTER- goes to confirm threshold
;      EXIT- returns to select unit screen, without changing off-time hours
;   SubMenuState=1, confirm threshold:
;      UP- select between Yes and No
;      DOWN- select between Yes and No
;      ENTER- if Yes is selected, new threshold is saved
;         if No is selected, new threshold is not saved, exit to Main
;      EXIT- returns to Select Function screen, without changing threshold
;----------------------------------------------------------------------*/
#if DIMMER_TEST
void SetDimmerLightState()
{
   
   switch(MenuSubState) //Based on MenuSubState, call appropriate subroutine
   {
      case 0: //SDLS0:
         SetDimmerLightUnit(); //Select Unit
         break;
      case 1: //SDLS1:
         AdjustDimmerPercent(); //select Dimmer Percentage
         break;
      case 2: //SDLS2:
         WriteDimmerData(); //Confirm On Percentage?
        break;
   }

   if(!bit_test(Toggled, BTN_EXIT)) //check exit button
   {
      InitMainMenuState();
   }

   bit_clear(ControlFlags, ButtonPress);
   bit_set(DisplayFlags, UpdateDisplayFlag);

   return;

}
#endif


/*;----------------------------------------------------------------------
; ProgramOffTimeState:
;   SubMenuState=0, set off-time hours:
;      UP- increments hours
;      DOWN- decrements hours
;      ENTER- goes to set off-time minutes
;      EXIT- returns to select unit screen, without changing off-time hours
;   SubMenuState=1, set off-time minutes:
;      UP- increments minutes
;      DOWN- decrements minutes
;      ENTER- goes to confirm off-time.
;      EXIT- returns to select unit screen, without changing off-time minutes
;   SubMenuState=2, confirm off-time:
;      UP- select between Yes and No
;      DOWN- select between Yes and No
;      ENTER- if Yes is selected, new off-time is saved, go to set off-time minutes.
;         if No is selected, new off-time is not saved, exit to Main
;      EXIT- returns to Select Function screen, without changing off-time for selected unit.
;----------------------------------------------------------------------*/
void ProgramOffTimeState()
{

   switch(MenuSubState) //Based on MenuSubState, call appropriate subroutine
   {
      case 0: //POffTS0:
         ProgramUnitHours();
         break;
      case 1: //POffTS1:
         ProgramUnitMins();
         break;
      case 2: //POffTS2:
         ProgramUnitOnOffTime(OFF);
         break;
   }

   if(!bit_test(Toggled, BTN_EXIT)) //check exit button
   {
      enable_interrupts(INT_EXT); //Re-enable ZeroX detect when done programming units
      InitFunctionData(L1ProgramUnit, L2DisplayAddr, P_SetUnit);
   }

   bit_clear(ControlFlags, ButtonPress);
   bit_set(DisplayFlags, UpdateDisplayFlag);

   return;

}


/*;----------------------------------------------------------------------
;TurnUnitOnOffState0 - SUOOS0
;----------------------------------------------------------------------*/
void TurnUnitOnOffMenu(BYTE aOn_or_Off )
{

   //check buttons and perform appropriate action
   if(!bit_test(Toggled, BTN_UP))
   {
      AdjustUnitUp();
      bit_set(DisplayFlags, PromptUpdateFlag);
   }
   else
   {
      if(!bit_test(Toggled, BTN_DOWN))
      {
         AdjustUnitDown();
         bit_set(DisplayFlags, PromptUpdateFlag);
      }
      else
      {
         if(!bit_test(Toggled, BTN_ENTER))
         {
            if (aOn_or_Off == ON)
            {
               SetupTxFunction(aOn_or_Off, OnFnc, UnitToConfig);
            }
            else
            {
               SetupTxFunction(aOn_or_Off, OffFnc, UnitToConfig);
            }
            //SetOnOffFlag(aOn_or_Off, UnitToConfig);
            //bit_set(DisplayFlags, UnitSetFlag);
         }
      }
   }

   return;

}

/*;----------------------------------------------------------------------
; TurnUnitOnOffState:
;   SubMenuState=0, 
;      UP- increments unit
;      DOWN- decrements unit
;      ENTER- Turn on off unit
;      EXIT- returns to Main Menu, 
;----------------------------------------------------------------------*/
void TurnUnitOnOffState( BYTE aON_or_OFF )
{

   TurnUnitOnOffMenu( aON_or_OFF ); 

   if(!bit_test(Toggled, BTN_EXIT)) //check exit button
   {
      InitMainMenuState();
   }

   bit_clear(ControlFlags, ButtonPress);
   bit_set(DisplayFlags, UpdateDisplayFlag);

   return;

}

/*;----------------------------------------------------------------------
;UpdateState
;
;----------------------------------------------------------------------*/
void UpdateState()
{

// disable_interrupts(INT_EXT);

   switch(MenuState)
   {
      case US_MainMenuState:
            MainMenuState();
      break;
      case US_SelectFunctionState:
            SelectFunctionState();
      break;
      case US_SetSystemTimeState:
            SetSystemTimeState();
      break;
      case US_SetSystemAddrState:
            SetSystemAddrState();
      break;
      case US_ProgramUnitState:
            ProgramUnitState();
      break;
      case US_ProgramOnTimeState:
            ProgramOnTimeState();
      break;
      case US_ProgramOffTimeState:
            ProgramOffTimeState();
      break;
      #if USE_LIGHT_SENSOR
         case US_SetLightSensorState:
               SetLightSensorState();
         break;
      #endif
      #if DIMMER_TEST
         case US_SetDimmerLightState:
               SetDimmerLightState();
         break;
      #endif
      case US_TurnOnUnitState:
            TurnUnitOnOffState( ON );
      break;
      case US_TurnOffUnitState:
            TurnUnitOnOffState( OFF );
      break;

   }

   bit_clear(ControlFlags, ButtonPress);
   bit_set(DisplayFlags, UpdateDisplayFlag);
   
//   enable_interrupts(INT_EXT);
   return;

}
