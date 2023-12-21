/*************************************************************************
 *
 *    Home Automation Protocol
 *
 *    (c) Copyright HomeAuto
 *
 *    File name   : haprot.c
 *    Description : Define a Home Automation Communication Protocol over 
 *                  Serial Port (UART)
 *    History :
 *    1. Data        : Feb 8, 2011
 *       Author      : Marco Carvalho
 *       Description : Create
 *
 *
 *    $Revision: 1.0 $
**************************************************************************/
#include "drv_app/haprot.h"
#include "drv/pic_uart.c"

/*************************************************************************
 * Function Name: GetMessageLength - RX
 * Parameters: void
 * Return: USHORT
 *         Number of Chars in UART RX Buffer
 *
 * Description: Get the Number of Chars in UART RX Buffer that can be a
 *              message including a valid command. This is not the size field
 *              in the HA protocol but the difference between was produced 
 *              (rx_next_in) and consumed (rx_next_out). 
 *      
 *************************************************************************/
USHORT GetMessageLength()
{   
    return UART_GetCharsInBuffer();
}

/*************************************************************************
 * Function Name: GetElement(USHORT aIdx) - RX
 * Parameters: USHORT aIdx (Index)
 * Return: UCHAR - One char from UART
 *
 * Description: Get one char from UART indexed from Index parameter (aIdx).
 *      
 *************************************************************************/
UCHAR GetElement(USHORT aIdx)
{
    //ASSERT(GetMessageLength() > aIdx);

    return UART_GetCharAtIndex(aIdx);
}

/*************************************************************************
 * Function Name: IncrCommandStart() - RX
 * Parameters: void
 * Return: UCHAR from RX UART
 *
 * Description: Get one char from UART. The main purpose of this function
 *              is to consume char from UART RX Buffer to free space in it.
 *      
 *************************************************************************/
UCHAR IncrCommandStart()
{
    unsigned char ucRetByte;

    //ASSERT(GetMessageLength() > 0);

    ucRetByte = UART_GetChar();

    return ucRetByte;
}

/*************************************************************************
 * Function Name: CommandIsNotEmpty - RX
 * Parameters: void
 * Return: BOOL
 *         0 - Command is empty
 *         1 - Command is not empty
 *
 * Description: Check if the message received is an Empty message or not by
 *              checking how many bytes are in RX UART Buffer.
 *      
 *************************************************************************/
BOOL HACommandIsNotEmpty()
{
    return (BOOL) (GetMessageLength() > 0);
}


/*************************************************************************
 * Function Name: CommandIsNotValid() - RX
 * Parameters: void
 * Return: BOOL
 *         0 - Command is valid
 *         1 - Command is not valid
 *
 * Description: Check if the message received is a valid command or not by
 *
 *      
 *************************************************************************/
BOOL HACommandIsNotValid()
{
    return (BOOL) (GetElement(HA_POS_SOH) != HA_ASCII_SOH);
}

/*************************************************************************
 * Function Name: GetCommandSize() - RX
 * Parameters: void
 * Return: USHORT
 *         Command Size (Virtual Node Protocol)
 *
 *
 * Description: Return the Size Field byte from the message received
 *              trough Virtual Node Protocol
 *      
 *************************************************************************/
USHORT GetCommandSize()
{

    //ASSERT(GetMessageLength() >= VNODE_HEADER_BYTES);

    return (USHORT) GetElement(HA_POS_SIZE);
}

/*************************************************************************
 * Function Name: MessageIsComplete() - RX
 * Parameters: void
 * Return: BOOL
 *         TRUE - The message received is complete
 *         FALSE - The message received is not complete

 * Description: Get and check some informations from the message received to
 *              verify if it is complete. Informations checked includes Start
 *              Header field and command size field. Command size field is
 *              compared to the number of bytes received in UART.
 *              why also not check the checksum here??
 *************************************************************************/
BOOL MessageIsComplete()
{
    USHORT usCommandSize, usMessageLength;

    if ((GetMessageLength() < HA_EXTRA_BYTES) ||(GetElement(HA_POS_SOH) != HA_ASCII_SOH))
    {
        return (BOOL) FALSE;
    }
    else
    {
        //ASSERT(CommandIsNotValid() == FALSE);

        usCommandSize = GetCommandSize();

        /* Get How many Bytes we have in TEST UART RXBuff*/
        usMessageLength = GetMessageLength();

        if (usMessageLength >= usCommandSize + HA_EXTRA_BYTES)
        {
            //return (BOOL) TRUE;
			return TRUE;
        }
        else
        {
            //return (BOOL) FALSE;
			return FALSE;
        }
    }
}

/*************************************************************************
 * Function Name: CheckSum - RX
 * Parameters: void
 * Return: BOOL
 *         0 - Checksum is not valid
 *         1 - Checksum is valid
 *
 * Description: Perform the xor operation with the command received bytes.
 *              If this operation result is zero, so the checksum is valid,
 *              otherwise is invalid.
 *      
 *************************************************************************/
BOOL CheckSum()
{
    USHORT i, sz;
    UCHAR chk;

    sz = GetCommandSize();

    chk = GetElement(0); // it's supposed to be always the Start Header Byte

    for (i = 1; i < sz + HA_EXTRA_BYTES; i++)
    {
        chk ^= GetElement(i);
    }

    return (BOOL) (chk == 0);
}

/*************************************************************************
 * Function Name: ProcessHACommand
 * Parameters: CmdHAProt_t aCommand,
 *             USHORT aSizeMessage,
 * Return: none
 * Description: ProcessTestCommand
 *
 *************************************************************************/
VOID ProcessHACommand(CmdHAProt_t aCommand, USHORT aSizeMessage)
{
    
    UCHAR HA_House, HA_Unit; 
    BOOLEAN HA_State;
    UCHAR HA_DimmerIntensity;
    UCHAR HA_OnDay, HA_OnMonth, HA_OnHour, HA_OnMin, HA_OnAMPM;
    UCHAR HA_OffDay, HA_OffMonth, HA_OffHour, HA_OffMin, HA_OffAMPM;
    UCHAR HA_Dusk, HA_Dawn;
    UCHAR HA_X10Function;
	//UCHAR HA_UnitLastState;
    
    switch (aCommand)
    {
        /* Android App: 0B20 - OFF 0B21 - ON */
		case HA_SET_UNIT: //tested OK
        
        HA_House = IncrCommandStart() - 0x41;
        HA_Unit = IncrCommandStart() - 0x30;
        HA_State = IncrCommandStart()- 0x30;

        //TurnUnitOnOffState( HA_State );
        //printf("HA_House = %c", HA_House);
        
        if (HA_House == House)
		{
			if(HA_State == ON)
        	{
            	HA_X10Function = OnFnc;
        	}
        	else
        	{
            	HA_X10Function = OffFnc;
        	}
		}
        
        SetupTxFunction(HA_State, HA_X10Function, HA_Unit);

        break;

		case HA_GET_UNIT:

        //HA_House = IncrCommandStart() - 0x41;
        //HA_Unit = IncrCommandStart() - 0x30;

		//if (HA_House == House)
		//{
			//HA_UnitLastState = UnitLastState[HA_Unit];
			//printf("HA_UnitLastState = %c", HA_UnitLastState);  // out of memory
		//}
                
        break;

        case HA_LIGHT_DIM: // to be tested
        
        HA_House = IncrCommandStart() - 0x41;
        HA_Unit = IncrCommandStart() - 0x30;
        HA_DimmerIntensity = IncrCommandStart() - 0x30;

		if (HA_House == House)
		{
			
			UnitToConfig = HA_Unit;
			//DimmerControl();  
			WriteData(EEDimmerPercent, HA_DimmerIntensity); //Record Dimmer Percentage

		}			
	
        break;

		/* Android App: 3B211;k011;k1 --> House B Unit 2 ON - 11:59AM OFF - 11:59PM */
        case HA_PROG_UNIT: //tested OK

        HA_House = IncrCommandStart() - 0x41;
        HA_Unit = IncrCommandStart() - 0x30;
        
        HA_OnDay = IncrCommandStart() - 0x30;
        HA_OnMonth = IncrCommandStart() - 0x30;
        HA_OnHour = IncrCommandStart() - 0x30;
        HA_OnMin = IncrCommandStart() - 0x30;
		HA_OnAMPM = IncrCommandStart() - 0x30;
        
        HA_OffDay = IncrCommandStart() - 0x30;
        HA_OffMonth = IncrCommandStart() - 0x30;
        HA_OffHour = IncrCommandStart() - 0x30;
        HA_OffMin = IncrCommandStart() - 0x30;
		HA_OffAMPM = IncrCommandStart() - 0x30;        

        if (HA_House == House)
		{
			UnitToConfig = HA_Unit;

			//ON
			HoursToConfig = HA_OnHour;
			MinsToConfig = HA_OnMin;

			if(HA_OnAMPM == 0x01)
			{
				bit_set(DisplayFlags, TempPM);
			}
			else
			{
				bit_clear(DisplayFlags, TempPM);
			}
	
			WriteUnitOnOffTime(ON);

			//OFF
			HoursToConfig = HA_OffHour;
			MinsToConfig = HA_OffMin;

			if(HA_OffAMPM == 0x01)
			{
				bit_set(DisplayFlags, TempPM);
			}
			else
			{
				bit_clear(DisplayFlags, TempPM);
			}

			WriteUnitOnOffTime(OFF);
		}


        break;

		/* Android App: 4 */
        case HA_SET_DATETIME: //tested OK
        
		HA_OnMonth = IncrCommandStart() - 0x30;
        HA_OnDay = IncrCommandStart() - 0x30;

        Hours = IncrCommandStart() - 0x30;
        Mins = IncrCommandStart() - 0x30;
		Secs = 0;
		HA_OnAMPM = IncrCommandStart() - 0x30;

		if(HA_OnAMPM == 0x01)
		{
			bit_set(DisplayFlags, PM);
		}
		else
		{
			bit_clear(DisplayFlags, PM);
		}

        break;

        case HA_GET_DATETIME:
        
        //HA_House = IncrCommandStart() - 0x41;
        //HA_Unit = IncrCommandStart() - 0x30;
       
       
        break;
        
		/* Android App: 6B */
        case HA_SET_HOUSE_ADDR://Tested OK

        House = IncrCommandStart()- 0x41;
		WriteData(EEHouse, House);
        
        break;
        
        case HA_GET_HOUSE_ADDR:

                
        break;     
        
		/* Android App: 81 */
        case HA_SET_UNIT_ADDR://Tested OK

       	Unit = IncrCommandStart() - 0x31;
		WriteData(EEUnit, Unit);
        
        break;
        
        case HA_GET_UNIT_ADDR:

                
        break;      
        
		/* Android App: :B311 */
        case HA_SET_LIGHT_SENSOR://tested OK

        HA_House = IncrCommandStart() - 0x41;;
        HA_Unit = IncrCommandStart() - 0x30;
        
        HA_Dusk = IncrCommandStart()- 0x30;
        HA_Dawn = IncrCommandStart()- 0x30;       
        	
		if (HA_House == House)
		{
			UnitToConfig = HA_Unit;

			//Dusk
			if(HA_Dusk == 0x01)
			{
				bit_set(DisplayFlags, Okay);
			}
			else
			{	
				bit_clear(DisplayFlags, Okay);
			}
			EnableOnOffAtDuskDawn(ON);
			
			//Dawn
			if(HA_Dawn == 0x01)
			{
				bit_set(DisplayFlags, Okay);
			}
			else
			{	
				bit_clear(DisplayFlags, Okay);
			}
			EnableOnOffAtDuskDawn(OFF);
			
		}

        break;
        
        case HA_GET_LIGHT_SENSOR:
        
        //HA_House = IncrCommandStart();
        //HA_Unit = IncrCommandStart();

        break;              
 
        case HA_GET_TEMPERATURE_VALUE:
        
        //HA_House = IncrCommandStart();
        
		//if (HA_House == House)
		//{
			//printf("%d",TemperatureValue);
		//}

        break;              

        default:
          break;

    }

}



/*************************************************************************
 * Function Name: DecodeHACommand()
 * Parameters: void
 * Return: void
 *
 * Description: Decode HA commands received from controller. If the
 *              command is completed and checksumed process it. The test
 *              command processor must be implemented in TestApp.c file
 *      
 *************************************************************************/
VOID DecodeHACommand()
{

    USHORT usMaxCmds = 2;
	
	
    /* buffer occupation management */
    //ManageOccupation();

    /* flush any garbage */
    while ((HACommandIsNotEmpty() != FALSE) && (HACommandIsNotValid() != FALSE))
    {
        (VOID) IncrCommandStart();
    }


	/* Check if receive some HA Command */
    while ((MessageIsComplete() != FALSE) && (usMaxCmds > 0))
    {
        //BOOL bMsgIsOk;
        CmdHAProt_t command;
        //MsgCounter = 0;
        USHORT  usCommandSize;
        UCHAR   ucStx;

        usMaxCmds--;

        /* hey, there is a complete message received ! But not yet checked ("checksumed") */

        if (CheckSum() != FALSE)
        {
            //bMsgIsOk = TRUE; // now, the complete command was received correctly
        	
			/* consumes STX and command */
        	ucStx         = IncrCommandStart();
        	command     = (CmdHAProt_t) IncrCommandStart();

        	/* compute size */
        	usCommandSize = IncrCommandStart();

			/* issue HA command */
			ProcessHACommand(command, usCommandSize);

			/* skip check sum */
			(VOID) IncrCommandStart();

		}
		/* Everything its OK in the message but the checksum is wrong. So we need to consume one
		   byte in order to clean up the rx_buffer */
        else
        {
            (VOID) IncrCommandStart();
			/* Could be implemented here to send the command again */
        }

       /* asks for retransmission if msg was in error */

        /* 
        if (bMsgIsOk == FALSE)
        {
            EncodeNack(ucMsgCounter);
        }
        */

        /* flush any garbage */

        while ((HACommandIsNotEmpty() != FALSE) && (HACommandIsNotValid() != FALSE))
        {
            (VOID) IncrCommandStart();
        }
    }
}


