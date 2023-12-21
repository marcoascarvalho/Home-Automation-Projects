/*;----------------------------------------------------------------------
; ClearEEprom - Clear EEPROM data
;----------------------------------------------------------------------*/
void ClearEEprom()
{
   BYTE EEProm_Addr;

   for(EEProm_Addr=0; EEProm_Addr<0xFF; EEProm_Addr++)
   {
      write_eeprom(EEProm_Addr,0);
   }

	/* write in the last memory byte */
	write_eeprom(0xFF,0);	

   return;
}

/*;----------------------------------------------------------------------
; ClearEEprom - Clear EEPROM data keeping the pre programmed
; positions
;----------------------------------------------------------------------*/
void ClearEEpromKeepingProgPositions()
{
   BYTE EEProm_Addr;
	BYTE EEpromData;

   for(EEProm_Addr=0; EEProm_Addr<0xFF; EEProm_Addr++)
   {
      EEpromData = read_eeprom(EEProm_Addr);
		if (EEpromData == 0xFF)
		{
			write_eeprom(EEProm_Addr,0);
		}
   }

	/* write in the last memory byte */
	EEpromData = read_eeprom(EEProm_Addr);
	if (EEpromData == 0xFF)
	{
		write_eeprom(0xFF,0);
	}	

   return;
}

/*;****************************************************************************************************
;Data EEPROM read and write code
;****************************************************************************************************
;----------------------------------------------------------------------
;WriteData- address to write is in ADDR, data to write is in VALUE
;----------------------------------------------------------------------*/
void WriteData(BYTE Address, BYTE Value)
{
   disable_interrupts(GLOBAL); // Disable interrupts during write
   write_eeprom (Address, Value);
   enable_interrupts(GLOBAL); // Re-enable interrupts

   return;

}
