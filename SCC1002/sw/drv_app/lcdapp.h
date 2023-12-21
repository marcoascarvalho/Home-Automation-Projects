
/*;----------------------------------------------------------------------
;Display Messages are assigned indexes that are used in DisplayTable
;----------------------------------------------------------------------*/
//                            1234567890123456
#define M_Welcome            "Welcome Home!   "
#define M_SelectFunction     "Select Function:"
#define M_SetSystemTime      "Set System Time "
#define M_SetSystemAddr      "Set System Addr "
#define M_ProgramUnit        "Program Unit    "
#define M_SetHours           "Set hrs         "
#define M_SetMin             "Set min         "
#define M_SetHouse           "Set House       "
#define M_SetUnit            "Set Unit        "
#define M_Okay               "Ok?"
#define M_ProgramOnTime      "Program On-Time "
#define M_ProgramOffTime     "Program Off-Time"
#define M_Clear              "                "
#define M_TurnUnitOn         "Turn On Unit    "
#define M_TurnUnitOff        "Turn Off Unit   "
#define M_SetLightSensor     "Set Light Sensor"
#define M_OnAtDusk           "On at Dusk?"
#define M_OffAtDawn          "Off at Dawn?"


#define M_SetDimmerLight     "Set Dimmer Light"
#define M_PercentOn          "Percent On:"
#define M_PercentOkay        "Percentage Ok?"

/*----------------------------------------------------------------------
;Definitions for L1Update, L2Update, and PromptUpdate routines
;----------------------------------------------------------------------*/
#define L1Welcome             0
#define L1SelectFunction    1
#define L1SetSystemTime       2
#define L1SetSystemAddr       3
#define L1ProgramUnit       4
#define L1ProgramOnTime       5
#define L1ProgramOffTime    6
#define L1SetLightSensor    7
#define L1SetDimmerLight    8
#define L1TurnUnitOn        9
#define L1TurnUnitOff       10

/* defines to available Functions */
/* adjust L2_SCROLL_MAX to match the last function number */
#define L2_SCROLL_MIN         0
#define L2SetSystemTime           0
#define L2SetSystemAddr           1
#define L2SetLightSensor        2
#define L2ProgramUnit           3
#define L2SetDimmerLight        4
#define L2TurnUnitOn            5
#define L2TurnUnitOff           6
#define L2_SCROLL_MAX         6

#define L2DisplayTime           7
#define L2DisplaySetTime        8
#define L2DisplayAddr           9
#define L2OnAtDusk              10
#define L2OffAtDawn              11
#define L2DisplayDimmerPercent  12
#define L2PercentOkay           13


/*
#define L2SetSystemTime           0
#define L2SetSystemAddr           1
#define L2ProgramUnit           2
#define L2TurnUnitOn            3
#define L2TurnUnitOff           4
#define L2DisplayTime           5
#define L2DisplaySetTime        6
#define L2DisplayAddr           7
*/

#define L1homeLCD() lcd_gotoxy(1,1)
#define L2homeLCD() lcd_gotoxy(1,2)

#define TurnLCDBLOn output_high(LCD_BLGHT)
#define TurnLCDBLOff output_low(LCD_BLGHT)

