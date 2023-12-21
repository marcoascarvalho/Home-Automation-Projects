//PORTA
#if USE_TRIAC
   #byte TRISA = 0x85
   #bit  TRIAC = 0x05.5
   #define TRIAC_pin 5
#endif

//PORTB
#if USE_KEYBOARD
   #define BTN_UP           1   //Button assignments on PORTB
   #define BTN_DOWN        2
   #define BTN_MENU        3
   #define BTN_ENTER        4
   #define BTN_EXIT        5
#endif

//PORTC
#define XOUT_pin       2
