
#include "application.h"
#include "x10.h"
#include "x10constants.h"
#include "HAProt.h"

/*Message field position */
#define HA_POS_COMMAND          (0)  //Command position
#define MAX_UNITS                16


/* weekdays
Particle    HA Prot
    1           1       = Sunday
    2           2       = Monday
    3           4       = Tuesday
    4           8       = Wednesday
    5           16      = Thursday
    6           32      = Friday
    7           64      = Saturday

*/

typedef enum
{
    HA_SET_UNIT                 = 0x30,  //ASCII 0
    HA_GET_UNIT                 = 0x31,  //ASCII 1
    HA_LIGHT_DIM                = 0x32,  //ASCII 2
    HA_PROG_UNIT                = 0x33,  //ASCII 3
    HA_SET_DATETIME             = 0x34,  //ASCII 4
    HA_GET_DATETIME             = 0x35,  //ASCII 5
    HA_SET_HOUSE_ADDR           = 0x36,  //ASCII 6
    HA_GET_HOUSE_ADDR           = 0x37,  //ASCII 7
    HA_SET_UNIT_ADDR            = 0x38,  //ASCII 8
    HA_GET_UNIT_ADDR            = 0x39,  //ASCII 9
    HA_SET_LIGHT_SENSOR         = 0x3A,  //ASCII :
    HA_GET_LIGHT_SENSOR         = 0x3B   //ASCII ;
} CmdHAProt_t;

Unit Units[MAX_UNITS-1];
uint8_t House = A;

void HAProcessCommand(uint8_t *HABuffer, uint8_t nBytesReceivedFromSocket)
{

	//int i;
	//unsigned char cks = 0;
	//unsigned short nBytesToTransmit;
	//unsigned char CmdValid = 0;
	//uint8_t HA_House = 0;
	uint8_t HA_UnitIndex = 0;
	uint8_t HA_DimmerCommand;
	uint8_t HA_DimmerDif = 0;
	uint8_t HA_DimmerCount;
	//uint8_t HA_State = 0;

// test command Android app Luiz: HA_SET_UNIT | HOUSE B| UNIT 2 | ON
//                                     0           B        2      3

    /* Process HA Command */
	switch (HABuffer[HA_POS_COMMAND])
    {

        case HA_SET_UNIT:

        	House = x10.EncodeX10House(HABuffer[1]); 
        	HA_UnitIndex = HABuffer[2] - 0x31; //get Unit ID
        	Units[HA_UnitIndex].ID = x10.EncodeX10Unit(HABuffer[2]);
        	Units[HA_UnitIndex].LastState = Units[HABuffer[2]].CurrentState;
        	Units[HA_UnitIndex].CurrentState = x10.EncodeX10State(HABuffer[3]);
        	//HA_State = x10.EncodeX10State(HABuffer[3]);
        	
        	/*
        	Serial.println();
        	Serial.println(HA_House);
        	Serial.println(HA_Unit);
        	Serial.println(HA_State);
        	*/
        	
        	x10.write(House, Units[HA_UnitIndex].ID, 1);
            x10.write(House, Units[HA_UnitIndex].CurrentState, 1);  
        	

        break;

        case HA_GET_UNIT:


        break;

        case HA_LIGHT_DIM:

// test command Android app Luiz: HA_LIGHT_DIM | HOUSE B| UNIT 2 | DIM Percent
//                                     2           B        3           50 (2)     90(Z) 100(d)    35 (#)


        	House = x10.EncodeX10House(HABuffer[1]); 
        	HA_UnitIndex = HABuffer[2] - 0x31; //get Unit ID
        	Units[HA_UnitIndex].ID = x10.EncodeX10Unit(HABuffer[2]);
        	
        	if (Units[HA_UnitIndex].CurrentState == OFF) { // 1st time
        	    Serial.println("DIM_FIRST");
        	    Units[HA_UnitIndex].CurrentState = DIM;  
        	    Units[HA_UnitIndex].LastState = HABuffer[3]; //.laststate store Dim percentage
        	    HA_DimmerCommand = BRIGHT;
        	    HA_DimmerDif = HABuffer[3];
        	}
        	else if (Units[HA_UnitIndex].CurrentState == DIM){
        	    if ((HABuffer[3]) >= Units[HA_UnitIndex].LastState) { // Bright command
        	        Serial.println("BRIGHT");
        	        HA_DimmerCommand = BRIGHT;
        	        HA_DimmerDif = HABuffer[3] - Units[HA_UnitIndex].LastState;
                } else { // DIM command
                    Serial.println("DIM");
                    HA_DimmerCommand = DIM;
                    HA_DimmerDif = Units[HA_UnitIndex].LastState - HABuffer[3];
                }
                Units[HA_UnitIndex].LastState = HABuffer[3];
        	}
        	
        	if (HA_DimmerDif > 0) {
        	    HA_DimmerCount = (uint8_t) (HA_DimmerDif / 5);
        	    x10.write(House, Units[HA_UnitIndex].ID, 1);
        	    x10.write(House, HA_DimmerCommand, HA_DimmerCount);
        	}
        	
        	
        	Serial.println();
        	Serial.print("Unit:");
        	Serial.println(HA_UnitIndex);
        	Serial.println(House);
        	Serial.println(Units[HA_UnitIndex].ID);
        	Serial.print("CurrentState:");
        	Serial.println(Units[HA_UnitIndex].CurrentState);
        	Serial.print("LastState:");
        	Serial.println(Units[HA_UnitIndex].LastState);
        	
        break;

        case HA_PROG_UNIT:

// test command Android app Luiz: HA_PROG_UNIT | HOUSE B| UNIT 2 | ONMonth |  ONDay  |  ONHour  |  ONMinute  |  OffMonth |  OffDay  |  OffHour  |  OffMinute  
//                                     3           B        2         :(10)     M(29)     9           1            :10       M(29)          9            2

            House = x10.EncodeX10House(HABuffer[1]); 
        	HA_UnitIndex = HABuffer[2] - 0x31; //get Unit ID
        	Units[HA_UnitIndex].ID = x10.EncodeX10Unit(HABuffer[2]);
        	Units[HA_UnitIndex].OnMonth = HABuffer[3] - 0x30;
        	Units[HA_UnitIndex].OnDay = HABuffer[4] - 0x30;
            Units[HA_UnitIndex].OnHour = HABuffer[5] - 0x30;
            Units[HA_UnitIndex].OnMinute = HABuffer[6] - 0x30;
            Units[HA_UnitIndex].OffMonth = HABuffer[7] - 0x30;
            Units[HA_UnitIndex].OffDay = HABuffer[8] - 0x30;
            Units[HA_UnitIndex].OffHour = HABuffer[9] - 0x30;
            Units[HA_UnitIndex].OffMinute = HABuffer[10] - 0x30;
            //Units[HA_UnitIndex].Weekday = HABuffer[11] - 0x30;
            //Units[HA_UnitIndex].RepeatMonth = HABuffer[12] - 0x30;
            
            
            
        	Serial.println();
        	Serial.print("Unit:");
        	Serial.println(HA_UnitIndex);
        	Serial.println(House);
        	Serial.println(Units[HA_UnitIndex].ID);
        	Serial.println(Units[HA_UnitIndex].OnMonth);
        	Serial.println(Units[HA_UnitIndex].OnDay);
        	Serial.println(Units[HA_UnitIndex].OnHour);
        	Serial.println(Units[HA_UnitIndex].OnMinute);
        	Serial.println(Units[HA_UnitIndex].OffMonth);
        	Serial.println(Units[HA_UnitIndex].OffDay);
        	Serial.println(Units[HA_UnitIndex].OffHour);
        	Serial.println(Units[HA_UnitIndex].OffMinute);
        	
        	
            
        break;

        case HA_SET_DATETIME:

        // not necessary just when in day light savings time

        break;

        case HA_GET_DATETIME:

    

        break;

        case HA_SET_HOUSE_ADDR:

    

        break;

        case HA_GET_HOUSE_ADDR:


        break;

        case HA_SET_UNIT_ADDR:

    

        break;

        case HA_GET_UNIT_ADDR:


        break;

        case HA_SET_LIGHT_SENSOR:
        
        
        // test command Android app Luiz: HA_SET_LIGHT_SENSOR | HOUSE B| UNIT | ENABLE |  NIGHT_VALUE | DAY_VALUE
        //                                    :                    B       2       1         3(ON)        4 (OFF)

            House = x10.EncodeX10House(HABuffer[1]); 
        	HA_UnitIndex = HABuffer[2] - 0x31; //get Unit ID
        	Units[HA_UnitIndex].ID = x10.EncodeX10Unit(HABuffer[2]);
        	if ((HABuffer[3] - 0x30) == 0x01) {
        	    Units[HA_UnitIndex].LightSensorControl = true;
                Units[HA_UnitIndex].LightNightValue = x10.EncodeX10State(HABuffer[4]);
                Units[HA_UnitIndex].LightDayValue = x10.EncodeX10State(HABuffer[5]);
        	}
        	else {
        	    Units[HA_UnitIndex].LightSensorControl = false;
                Units[HA_UnitIndex].LightNightValue = OFF;
                Units[HA_UnitIndex].LightDayValue = OFF;
        	}
        	
        	
            
            Serial.println();
        	Serial.print("Unit:");
        	Serial.println(HA_UnitIndex);
        	Serial.println(House);
        	Serial.println(Units[HA_UnitIndex].LightSensorControl);
        	Serial.println(Units[HA_UnitIndex].LightNightValue);
        	Serial.println(Units[HA_UnitIndex].LightDayValue);
     
        break;

        case HA_GET_LIGHT_SENSOR:

     

        break;

        default:
          break;

    }



    return;

}