//////// Standard Header file for the PIC16F877 device ////////////////
#device PIC16F877 adc=8
#nolist
//#OPT 5
//////// Program memory: 8192x14  Data RAM: 367  Stack: 8
//////// I/O: 33   Analog Pins: 8
//////// Data EEPROM: 256
//////// C Scratch area: 77   ID Location: 2000
//////// Fuses: LP,XT,HS,RC,NOWDT,WDT,NOPUT,PUT,PROTECT,PROTECT_5%
//////// Fuses: PROTECT_50%,NOPROTECT,NOBROWNOUT,BROWNOUT,LVP,NOLVP,CPD
//////// Fuses: NOCPD,WRT,NOWRT,DEBUG,NODEBUG
////////
////////////////////////////////////////////////////////////////// I/O
// Discrete I/O Functions: SET_TRIS_x(), OUTPUT_x(), INPUT_x(),
//                         PORT_B_PULLUPS(), INPUT(),
//                         OUTPUT_LOW(), OUTPUT_HIGH(),
//                         OUTPUT_FLOAT(), OUTPUT_BIT()
// Constants used to identify pins in the above are:

#define LDR  40
#define AD_TEMP  41   
#define PIN_A2  42   
#define PIN_A3  43
#define PIN_A4  44
#define PIN_TRIAC  45

#define ZEROX   48
#define UP      49      //PIN_B1 - INPUT
#define DOWN    50      //PIN_B2 - INPUT
#define MENU    51      //PIN_B3 - INPUT
#define ENTER    52      //PIN_B4 - INPUT
#define EXIT    53      //PIN_B5 - INPUT
#define PIN_B6  54      //PIN_B6 - OUTPUT
#define PIN_B7  55      //PIN_B7 - OUTPUT


#define PIN_C0  56
#define PIN_C1  57
#define XOUT    58 //PIN_C2 clear to enable 120kHz output, set disable 120kHz output
#define XIN     59 //PIN_C3
#define IR_RX   60 //PIN_C4
#define IR_TX   61 //PIN_C5
#define TX      62 //PIN_C6
#define RX      63 //PIN_C7

#define PIN_D0  64
#define PIN_D1  65
#define PIN_D2  66
#define PIN_D3  67
#define PIN_D4  68
#define PIN_D5  69
#define PIN_D6  70
#define PIN_D7  71

#define RED_LED    72   //PIN_E0 
#define YELLOW_LED  73   //PIN_E1 
#define LCD_BLGHT  74   //PIN_E2 -- LCD Backlight Control

////////////////////////////////////////////////////////////////// Useful defines
#define FALSE 0
#define TRUE 1

#define BYTE int
#define BOOLEAN short int

#define getc getch
#define fgetc getch
#define getchar getch
#define putc putchar
#define fputc putchar
#define fgets gets
#define fputs puts

////////////////////////////////////////////////////////////////// Control
// Control Functions:  RESET_CPU(), SLEEP(), RESTART_CAUSE()
// Constants returned from RESTART_CAUSE() are:
#define WDT_FROM_SLEEP  0
#define WDT_TIMEOUT     8
#define MCLR_FROM_SLEEP 16
#define NORMAL_POWER_UP 24


////////////////////////////////////////////////////////////////// Timer 0
// Timer 0 (AKA RTCC)Functions: SETUP_COUNTERS() or SETUP_TIMER0(),
//                              SET_TIMER0() or SET_RTCC(),
//                              GET_TIMER0() or GET_RTCC()
// Constants used for SETUP_TIMER0() are:
#define RTCC_INTERNAL   0
#define RTCC_EXT_L_TO_H 32
#define RTCC_EXT_H_TO_L 48

#define RTCC_DIV_1      8
#define RTCC_DIV_2      0
#define RTCC_DIV_4      1
#define RTCC_DIV_8      2
#define RTCC_DIV_16     3
#define RTCC_DIV_32     4
#define RTCC_DIV_64     5
#define RTCC_DIV_128    6
#define RTCC_DIV_256    7


#define RTCC_8_BIT      0

// Constants used for SETUP_COUNTERS() are the above
// constants for the 1st param and the following for
// the 2nd param:

////////////////////////////////////////////////////////////////// WDT
// Watch Dog Timer Functions: SETUP_WDT() or SETUP_COUNTERS() (see above)
//                            RESTART_WDT()
//
#define WDT_18MS        8
#define WDT_36MS        9
#define WDT_72MS       10
#define WDT_144MS      11
#define WDT_288MS      12
#define WDT_576MS      13
#define WDT_1152MS     14
#define WDT_2304MS     15

////////////////////////////////////////////////////////////////// Timer 1
// Timer 1 Functions: SETUP_TIMER_1, GET_TIMER1, SET_TIMER1
// Constants used for SETUP_TIMER_1() are:
//      (or (via |) together constants from each group)
#define T1_DISABLED         0
#define T1_INTERNAL         0x85
#define T1_EXTERNAL         0x87
#define T1_EXTERNAL_SYNC    0x83

#define T1_CLK_OUT          8

#define T1_DIV_BY_1         0
#define T1_DIV_BY_2         0x10
#define T1_DIV_BY_4         0x20
#define T1_DIV_BY_8         0x30

////////////////////////////////////////////////////////////////// Timer 2
// Timer 2 Functions: SETUP_TIMER_2, GET_TIMER2, SET_TIMER2
// Constants used for SETUP_TIMER_2() are:
#define T2_DISABLED         0
#define T2_DIV_BY_1         4
#define T2_DIV_BY_4         5
#define T2_DIV_BY_16        6

////////////////////////////////////////////////////////////////// CCP
// CCP Functions: SETUP_CCPx, SET_PWMx_DUTY
// CCP Variables: CCP_x, CCP_x_LOW, CCP_x_HIGH
// Constants used for SETUP_CCPx() are:
#define CCP_OFF                         0
#define CCP_CAPTURE_FE                  4
#define CCP_CAPTURE_RE                  5
#define CCP_CAPTURE_DIV_4               6
#define CCP_CAPTURE_DIV_16              7
#define CCP_COMPARE_SET_ON_MATCH        8
#define CCP_COMPARE_CLR_ON_MATCH        9
#define CCP_COMPARE_INT                 0xA
#define CCP_COMPARE_RESET_TIMER         0xB
#define CCP_PWM                         0xC
#define CCP_PWM_PLUS_1                  0x1c
#define CCP_PWM_PLUS_2                  0x2c
#define CCP_PWM_PLUS_3                  0x3c
long CCP_1;
#byte   CCP_1    =                      0x15
#byte   CCP_1_LOW=                      0x15
#byte   CCP_1_HIGH=                     0x16
long CCP_2;
#byte   CCP_2    =                      0x1B
#byte   CCP_2_LOW=                      0x1B
#byte   CCP_2_HIGH=                     0x1C
////////////////////////////////////////////////////////////////// PSP
// PSP Functions: SETUP_PSP, PSP_INPUT_FULL(), PSP_OUTPUT_FULL(),
//                PSP_OVERFLOW(), INPUT_D(), OUTPUT_D()
// PSP Variables: PSP_DATA
// Constants used in SETUP_PSP() are:
#define PSP_ENABLED                     0x10
#define PSP_DISABLED                    0

#byte   PSP_DATA=                       8

////////////////////////////////////////////////////////////////// SPI
// SPI Functions: SETUP_SPI, SPI_WRITE, SPI_READ, SPI_DATA_IN
// Constants used in SETUP_SSP() are:
#define SPI_MASTER       0x20
#define SPI_SLAVE        0x24
#define SPI_L_TO_H       0
#define SPI_H_TO_L       0x10
#define SPI_CLK_DIV_4    0
#define SPI_CLK_DIV_16   1
#define SPI_CLK_DIV_64   2
#define SPI_CLK_T2       3
#define SPI_SS_DISABLED  1

#define SPI_SAMPLE_AT_END 0x8000
#define SPI_XMIT_L_TO_H  0x4000

////////////////////////////////////////////////////////////////// UART
// Constants used in setup_uart() are:
// FALSE - Turn UART off
// TRUE  - Turn UART on
#define UART_ADDRESS 2
#define UART_DATA    4
// TRUE  - Turn UART on
////////////////////////////////////////////////////////////////// ADC
// ADC Functions: SETUP_ADC(), SETUP_ADC_PORTS() (aka SETUP_PORT_A),
//                SET_ADC_CHANNEL(), READ_ADC()
// Constants used in SETUP_ADC_PORTS() are:
#define NO_ANALOGS             0x86         // None
#define ALL_ANALOG             0x80         // A0 A1 A2 A3 A5 E0 E1 E2 Ref=Vdd
#define ANALOG_RA3_REF         0x81         // A0 A1 A2 A5 E0 E1 E2 Ref=A3
#define A_ANALOG               0x82         // A0 A1 A2 A3 A5 Ref=Vdd
#define A_ANALOG_RA3_REF       0x83         // A0 A1 A2 A5 Ref=A3
#define RA0_RA1_RA3_ANALOG     0x84         // A0 A1 A3 Ref=Vdd
#define RA0_RA1_ANALOG_RA3_REF 0x85         // A0 A1 Ref=A3
#define ANALOG_RA3_RA2_REF              0x88   // A0 A1 A5 E0 E1 E2 Ref=A2,A3
#define ANALOG_NOT_RE1_RE2              0x89   // A0 A1 A2 A3 A5 E0 Ref=Vdd
#define ANALOG_NOT_RE1_RE2_REF_RA3      0x8A   // A0 A1 A2 A5 E0 Ref=A3
#define ANALOG_NOT_RE1_RE2_REF_RA3_RA2  0x8B   // A0 A1 A5 E0 Ref=A2,A3
#define A_ANALOG_RA3_RA2_REF            0x8C   // A0 A1 A5 Ref=A2,A3
#define RA0_RA1_ANALOG_RA3_RA2_REF      0x8D   // A0 A1 Ref=A2,A3
#define RA0_ANALOG                      0x8E   // A0
#define RA0_ANALOG_RA3_RA2_REF          0x8F   // A0 Ref=A2,A3
// Constants used for SETUP_ADC() are:
#define ADC_OFF                0              // ADC Off
#define ADC_CLOCK_DIV_2        1
#define ADC_CLOCK_DIV_8     0x41
#define ADC_CLOCK_DIV_32    0x81
#define ADC_CLOCK_INTERNAL  0xc1              // Internal 2-6us

// Constants used in READ_ADC() are:
#define ADC_START_AND_READ     7   // This is the default if nothing is specified
#define ADC_START_ONLY         1
#define ADC_READ_ONLY          6

////////////////////////////////////////////////////////////////// INT
// Interrupt Functions: ENABLE_INTERRUPTS(), DISABLE_INTERRUPTS(),
//                      EXT_INT_EDGE()
//
// Constants used in EXT_INT_EDGE() are:
#define L_TO_H              0x40
#define H_TO_L                 0
// Constants used in ENABLE/DISABLE_INTERRUPTS() are:
#define GLOBAL                    0x0BC0
#define INT_RTCC                  0x0B20
#define INT_RB                    0x0B08
#define INT_EXT                   0x0B10
#define INT_AD                    0x8C40
#define INT_TBE                   0x8C10
#define INT_RDA                   0x8C20
#define INT_TIMER1                0x8C01
#define INT_TIMER2                0x8C02
#define INT_CCP1                  0x8C04
#define INT_CCP2                  0x8D01
#define INT_SSP                   0x8C08
#define INT_PSP                   0x8C80
#define INT_BUSCOL                0x8D08
#define INT_EEPROM                0x8D10
#define INT_TIMER0                0x0B20

#list
