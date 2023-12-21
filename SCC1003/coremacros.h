/*
  coremacros.h
  
  16/10/2016
 
 
 */

//extern const uint8_t PROGMEM digital_pin_to_port_PGM[];
// extern const uint8_t PROGMEM digital_pin_to_bit_PGM[];
//extern const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[];

//extern const uint16_t PROGMEM port_to_input_PGM[];

#define digitalPinToPort(P) PIN_MAP[P].gpio_peripheral->IDR  // READ P PIN 
#define digitalPinToBitMask(P) PIN_MAP[P].gpio_peripheral->IDR & PIN_MAP[P].gpio_pin  // READ P PIN ONLY, NO SAVE
#define portInputRegister(P)  (volatile uint8_t *)(PIN_MAP[P].gpio_peripheral->IDR)