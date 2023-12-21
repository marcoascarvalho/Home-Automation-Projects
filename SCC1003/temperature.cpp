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
 *    1. Data        : Jan 15, 2017
 *       Author      : Marco Carvalho
 *       Description : Create
 *
 *
 *    $Revision: 1.0 $
**************************************************************************/
#include "application.h"
#include "temperature.h"


int const TEMPERATURE_VALUES [41] =
{0xA56,0xA2E,0xA05,0x9DD,0x9B4,0x98B,0x961,0x938,0x90F,0x8E6,0x8BC,0x893,0x86A,0x841,0x818,0x7F0,
0x7C8,0x7A0,0x778,0x750,0x729,0x703,0x6DC,0x6B6,0x691,0x66C,0x647,0x623,0x5FF,0x5DC,0x5BA,0x598,
0x576,0x555,0x535,0x515,0x4F6,0x4D7,0x4B9,0x49C,0x47F};

/*;----------------------------------------------------------------------
;GetTemperatureValue -
;Get the temperature Value
;----------------------------------------------------------------------*/
int temperatureClass::GetTemperatureValue()
{

    int Address;
    int TemperatureSensorPin = A7;  
    int TemperatureValue;
    unsigned int RawTemperatureValue;

   Address = 0;
   TemperatureValue = 0;

    /* Read Temperature sensor */
   RawTemperatureValue = analogRead(TemperatureSensorPin);
    Serial.print("RawTemperatureValue: 0x");
    Serial.println(RawTemperatureValue, HEX);

   do
   {
      TemperatureValue = TEMPERATURE_VALUES [Address];
      Address++;
   }
   while(TemperatureValue > RawTemperatureValue);

   TemperatureValue = BASE_TEMPERATURE + (Address-1);

    Serial.print("TemperatureValue: ");
    Serial.println(TemperatureValue);
    return TemperatureValue;

}