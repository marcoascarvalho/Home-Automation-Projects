
BYTE DimmerCount, DimmerPercentage;


/*;----------------------------------------------------------------------
; AdjustDimmerPercentUp:
;   Adjust Up the Dimmer Percentage. If percentage is 99 do nothing
;----------------------------------------------------------------------*/
void AdjustDimmerPercentUp()
{
  
  if (DimmerPercentage >= 100) //If DimmerPercent
  {
      DimmerPercentage = 0; //then make DimmerPercent = 0
  }
  else
  {
      DimmerPercentage+=5;
  }

   return;

}

/*;----------------------------------------------------------------------
; AdjustDimmerPercentDown:
;   Adjust Down the Dimmer Percentage. If percentage is 1 do nothing
;----------------------------------------------------------------------*/
void AdjustDimmerPercentDown()
{

   if(DimmerPercentage == 0) //check to see if DimmerPercent is at zero
   {
      DimmerPercentage=95;
   }
   else
   {
      DimmerPercentage-=5;
   }

   return;

}

/*
;----------------------------------------------------------------------
;DimmerControl
;----------------------------------------------------------------------*/
void DimmerControl()
{

   unsigned BYTE DimmerPercentStored, DimmerDif;
   BYTE AddrToRead;
   BYTE DimmerTxFunction;
   
   AddrToRead = (UnitToConfig-1) + EEDimmerPercent;

   DimmerPercentStored = read_eeprom(AddrToRead);

   if (DimmerPercentage > DimmerPercentStored)
   {
      DimmerTxFunction = Bright;
      DimmerDif = DimmerPercentage - DimmerPercentStored;
      //printf("TxFunction = Bright\n");
   }
   else
   {
      DimmerTxFunction = Dim;
      DimmerDif = DimmerPercentStored - DimmerPercentage;
      //printf("TxFunction = Dim\n");
   }

   /* to prevent DIV/0 and only send dim commands if
      Dimmer diference > 0 */
   if (DimmerDif == 0)
   {
      DimmerCount = 1;
   }
   else
   {
      bit_clear(Wait3CyclesFlag);
      //DimmerCount = (BYTE) (DimmerDif / 5); //out of ROM // Heavy function
	  SetupTxFunction(ON, DimmerTxFunction, UnitToConfig);
      //SetOnOffFlag(ON, UnitToConfig);
      //bit_set(DisplayFlags, UnitSetFlag);
   }

   //printf("DimmerDif = 0x%x\n", DimmerDif);
   //printf("DimmerCount = 0x%x\n", DimmerCount);


   return;
}


