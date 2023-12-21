#use delay(clock=20000000)
#fuses NOWDT,HS, PUT, NOPROTECT, BROWNOUT, NOLVP, NOCPD, NOWRT, NODEBUG
#use rs232(baud=115200, xmit=TX, rcv=RX)
#use fast_io(B)

#define BOOL short int
#define UCHAR unsigned char
#define USHORT unsigned char

#define ON 0x01
#define OFF 0x00

#byte STATUS = 0x03
#byte PORTA = 0x05
#byte PORTB = 0x06
#byte PORTC = 0x07

#byte OPTION_REG = 0x81
#byte TRISA = 0x85
#byte TRISB = 0x86
#byte TRISC = 0x87

#byte INTCON = 0x0B
#byte PIR1 = 0x0C
#byte PIR2 = 0x0D
#byte PIE1 = 0x8C
#byte PIE2 = 0x8D

#define DEBUG 0

#define USING_TW523 1
#define 3_PHASE 1
//#define PRINT_IN_OUT 1
//#define CONTINUOUS_TX 0

#if DEBUG
   //#define DEBUG_KEY         1
   //#define DEBUG_CDS         1
   //#define DEBUG_TURN_ON_OFF 1
   //#define X10_TX_SIM          1
   //#define X10_RX_SIM          1
   //#define X10_RX_SIM_PIN      1
   //#define DEBUG_THERM 1
   //#define LOOP_X10_TX 1
   //#define PROG_RLM20    1
#endif

#define GIEF            7
#define INTF            1
#define T0IF            2
#define INTEDG          6

#define ALL_INPUTS         0xFF

/*;----------------------------------------------------------------------
;Data EEPROM Storage Locations
;----------------------------------------------------------------------*/
#define EEHouse                0x01
#define EEUnit                   0x02
#define EEOnOffHours             0x10
#define EEOnMins                0x20
#define EEOffMins                0x30
//#define EETemperatureTableBase 0x70
#define EEDimmerPercent        0x70

#define EE_PM            7
#define EE_LightActivate 6

//#rom  EETemperatureTableBase = {0x72,0x70,0x6D,0x6B,0x69,0x66,0x64}

#define P_SetHrs             0
#define P_SetMin             1
#define P_Okay                2
#define P_SetUnit             3
#define P_SetHouse          4
#define P_DisplayYN          5

//ControlFlags
#define      ButtonPress        0 // To check if button was pressed
#define      MinElapsedFlag     1 // To check if one minute has elapsed after button was pressed or at init
#define      CheckTimesFlag     2 // To check one by one minute what needs to happen
#define      Match              3 // To check if the time to turn or or off a unit matches
//#define      LightActivate      4 // To check if a unit must be turned on or off based on light sensor activation
#define      InitFlag           5 // To check if display backlight must be turned on or off after initialization
//#if USE_LIGHT_SENSOR
#define      DuskFlag           6
#define      DawnFlag           7
//#endif

//DisplayFlags
#define      UpdateDisplayFlag   0
#define      L1UpdateFlag      1
#define     UnitSetFlag       2 // new - checked if unit has to be turned ON/OFF
#define      PromptUpdateFlag  3
#define      Okay               4
#define      PM                  5
#define      TempPM            6
#define      X10AddressSent      7



//States used in UpdateState() function
#define   US_MainMenuState           0
#define   US_SelectFunctionState     1
#define   US_SetSystemTimeState      2
#define   US_SetSystemAddrState      3
#define   US_ProgramUnitState        4
#define   US_ProgramOnTimeState      5
#define   US_ProgramOffTimeState     6
#define   US_SetLightSensorState     7
#define  US_SetDimmerLightState     8
#define  US_TurnOnUnitState         9
#define  US_TurnOffUnitState        10

//States used in SelectFunctionState() function
#define  SFS0     0
#define  SFS1     1
#define  SFS2     2
#define  SFS3     3
#define  SFS4     4
#define  SFS5     5
#define  SFS6     6



#define LowLightTreshold  0x20
#define HighLightTreshold 0x50
#define MinutesToConfirmLightLevel 5


/* Function Prototypes */
void SetOnOffFlag(BOOLEAN aOn_or_Off, BYTE aUnitToSet);
void ClearOnOffFlag(BOOLEAN aOn_or_Off, BYTE aUnitToClear);
void SetupTxFunction(BOOLEAN aOn_or_Off, BYTE aX10Function, BYTE aUnitToSet);
void TurnUnitOnOff(BOOLEAN aOn_or_Off, unsigned long aOnOffFlags);
void ControlX10Units();
BOOLEAN CheckLightActivateBit(BOOLEAN aOn_or_Off, BYTE aUnitToCheck);
void WriteUnitOnOffTime(BOOLEAN aOn_or_Off);
void EnableOnOffAtDuskDawn(BOOLEAN aON_or_OFF);
BYTE ReadUnitOnOffMins(BOOLEAN On_or_Off, BYTE aUnitToRead);
BYTE ReadUnitOnOffHours(BOOLEAN On_or_Off, BYTE aUnitToRead);



