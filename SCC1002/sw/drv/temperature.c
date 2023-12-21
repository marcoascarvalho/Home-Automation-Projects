/*************************************************************************
 *
 *    Home Automation Project - Temperature Driver
 *
 *    (c) Copyright HomeAuto
 *
 *    File name   : temperature.c
 *    Description : Define a driver for temperature sensor
 *                  
 *    History :
 *    1. Data        : Feb 27, 2012
 *       Author      : Marco Carvalho
 *       Description : Create
 *
 *
 *    $Revision: 1.0 $
**************************************************************************/

#define TEMPERATURE_ADC_CHANNEL 1

#define BASE_TEMPERATURE 5

BYTE const TEMPERATURE_VALUES [31] =
{0x98,0x96,0x93,0x90,0x8E,0x8B,0x89,0x86,0x84,0x81,0x7F,0x7C,0x7A,0x77,0x75,0x72,
0x70,0x6D,0x6B,0x69,0x66,0x64,0x62,0x60,0x5D,0x5B,0x59,0x57,0x55,0x53,0x51};

BYTE TemperatureValue;

/*;----------------------------------------------------------------------
; ReadADC - Temperature
;----------------------------------------------------------------------*/
unsigned BYTE ReadTemperatureSensor()
{

	unsigned BYTE TemperatureLevel;

   set_adc_channel (TEMPERATURE_ADC_CHANNEL);
   delay_us(10);
   TemperatureLevel = read_adc();

   return(TemperatureLevel);

}

/*;----------------------------------------------------------------------
;GetTemperatureValue -
;Get the temperature Value
;----------------------------------------------------------------------*/
void GetTemperatureValue()
{

   BYTE Address;
	unsigned BYTE RawTemperatureValue;

   Address = 0;
   TemperatureValue = 0;

   RawTemperatureValue = ReadTemperatureSensor();

   do
   {
      TemperatureValue = TEMPERATURE_VALUES [Address];
      Address++;
   }
   while(TemperatureValue > RawTemperatureValue);

   TemperatureValue = BASE_TEMPERATURE + (Address-1);

   return;

}
