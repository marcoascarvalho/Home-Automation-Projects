/****************************************************************************************************
;INITIALIZATION ROUTINES
;****************************************************************************************************
;----------------------------------------------------------------------
; InitTimer1-
;----------------------------------------------------------------------
;Timer1 is used with an external 32kHz crystal oscillator and configured
;to generate an interrupt 1/25ms.
;   TMR1 interrupt= 1/32kHz * prescaler * (FFFFh+1-preload)
;         1/32kHz * 1         * (FFFFh+1-FCE0h) = 25msec
;   - Initialize Timer1 with FCE0h
;      - Configure Timer1
;      - Prescale 1:1
;      - Enable Oscillator
;      - DO NOT SYNCHRONIZE
;      - External Clock Source
;      - Enable Timer1
;   - Clear Timer1 Overflow Interrupt Flag
;   - Enable Timer1 Overflow Interrupt
;----------------------------------------------------------------------*/
void InitTimer1()
{

   set_timer1(0xFCCE);

   setup_timer_1(T1_DIV_BY_1 | T1_CLK_OUT | T1_EXTERNAL);

   enable_interrupts(INT_TIMER1);

   return;
}

/*;----------------------------------------------------------------------
; InitTimer0
;----------------------------------------------------------------------
; Timer0 is used to time the duration of the 1ms 120kHz burst and the
;   onset of phase2 and phase3 bursts if implemented.
;   Fosc = 7.6800MHz, TMR0 clock source is instruction cycle clock, Prescaler = 64 so:
;      - 1ms delay with TMR0 preload of 0xE2 (.226)
;      - 1.7667ms delay with TMR0 preload of 0xCB (.203)
;     - 20 MHz Clock TMR0 preload with 0xB2 (.178)
;---------------------------------------------------------------------- */
void InitTimer0()
{

   setup_timer_0(RTCC_DIV_64 | RTCC_INTERNAL);

   return;
}

/*;----------------------------------------------------------------------
; InitZeroXDetect
;----------------------------------------------------------------------*/
void InitZeroXDetect()
{

   ext_int_edge(L_TO_H); //interrupt on rising edge

   enable_interrupts(INT_EXT); //Enable INT External interrupt on RB0

   return;
}

/*;----------------------------------------------------------------------
;InitPWM
;   - Output on RC2 is initially disabled
;   - Initialize PWM to produce 120kHz oscillation for X-10 transmitter
;   - Main oscillator is 7.680MHz, Tosc = 1/7.680MHz
;   - PWM period = 1/120kHz =  8.333us
;   - PWM period = [(PR2) + 1] * 4 * Tosc * (TMR2 prescale value)
;           = [(0) + 1] * 4 * 1/7.680MHz * (16) = 8.333us
;   - PR2 = 0, TMR2 prescaler = 16
;   - 50% PWM duty cycle = 8.333us/2 = DCxB9:DCxB0 * Tosc * (TMR2 prescale value)
;                      DCxB9:DCxB0 = 8.333us/2/Tosc/(TMR2 prescale value) = 2
;   - postscaler = 15 generates TMR2 interrupt every 15 * 8.333us= .125ms
;      for approximately 32 interrupts/half-cycle for dimmer control.
; 20 MHz:
;- PWM period = [(PR2) + 1] * 4 * Tosc * (TMR2 prescale value)
;           = [(9) + 1] * 4 * 1/20.0MHz * (4) = 8.0us
;   - PR2 = 9, TMR2 prescaler = 4
; 50% PWM duty cycle = 8.0us/2 = DCxB9:DCxB0 * Tosc * (TMR2 prescale value)
;                      DCxB9:DCxB0 = 8us/2/Tosc/(TMR2 prescale value) = 20
;

;----------------------------------------------------------------------*/
void InitPWM()
{

#if USING_TW523

   output_low(XOUT);

#else

   bit_set(TRISC, XOUT_pin); //disable output

   setup_ccp1(CCP_OFF); //CCP module is off

   set_timer2(0x00);  //Clear Timer2

   set_pwm1_duty(0x15); //set duty cycle to 50%

   disable_interrupts(INT_CCP1); //disable CCP1 interrupts

   setup_ccp1(CCP_PWM); //configure the CCP1 module for PWM operation

   setup_timer_2(T2_DIV_BY_1, 41, 15);

   bit_set(TRISC, XOUT_pin); //disable output

#endif

   return;

}

/*;----------------------------------------------------------------------
; InitADC
;----------------------------------------------------------------------*/
void InitADC()
{

   setup_adc_ports (RA0_ANALOG); //Analog channel is RA0
   setup_adc (ADC_CLOCK_DIV_32); //ADC conversion clock is Fosc/32
   set_adc_channel (0);

   return;

}


/*;----------------------------------------------------------------------
; InitVariables
;----------------------------------------------------------------------*/
void InitVariables()
{

   /* initialize clock variables */
   Secs = 0;
   Mins = 0;
   Hours = 12;

   /* Init TimeLimit Minute elapsed variables */
   TimeLimit = 59;

   /* initialize state variables */
   MenuSubState = 0;
   MenuState = 0;

   /* set flags to update display */
   bit_set(DisplayFlags, UpdateDisplayFlag);
   bit_set(DisplayFlags, L1UpdateFlag);
   bit_clear(DisplayFlags, PromptUpdateFlag);

   #if USE_KEYBOARD
      /* initialize ScanKey variables */
      bit_clear(ControlFlags, ButtonPress);
      Debounced = 0xFF;
      LastDebounced = 0xFF;
      Toggled = 0xFF;
      Held = 0xFF;
      ButtonPressLock = FALSE;
   #endif

   /* initialize unit flag display */
   bit_clear(DisplayFlags, UnitSetFlag);

   #if USE_LCD
      /*Initialize Display Lines*/
      L1 = L1Welcome; //L1 initialized to Welcome Message
      L2 = L2DisplayTime; //L2 initialized to display time
   #endif

   ClearEEpromKeepingProgPositions();

   /*Initialize House address to value stored in EEPROM*/
   House = read_eeprom(EEHouse);

   /*Make sure House is a value 0-(X10MaxUnits-1)*/
   if(House > (X10MaxUnits-1))
   {
      //House = 0;
      House = 1; //B
   }

   HouseToConfig = House;

   /*Initialize Unit address to value stored in EEPROM */
   Unit = read_eeprom(EEUnit);

   /*Make sure House is a value 0-(X10MaxUnits-1)*/
   if(Unit > (X10MaxUnits-1))
   {
      Unit = 0;
   }

   UnitToConfig = Unit;

   /* Init Light Sensor variables */
   #if USE_LIGHT_SENSOR
      DawnCount = 0;
      DuskCount = 0;
      bit_clear(ControlFlags, DawnFlag);
      bit_clear(ControlFlags, DuskFlag);
      // This flag must be used if we want to check light in a period different 
      // from 1 minute
      //bit_clear(ControlFlags, CheckLightFlag); 
   #endif

   /* Init to wait 3 cycles variable */
   bit_set(Wait3CyclesFlag);

   #if DIMMER_TEST
      DimmerCount = 1;
   #endif

   //clear the on and off flags for the X-10 units
   On_Flags = 0x0000;
   Off_Flags = 0x0000;
   
   /* Mark init Flag to init Display Backlight turned on */
   bit_set(ControlFlags, InitFlag);
   
   /* Turn LCD backlight On */
   TurnLCDBLOn;

   return;
}
