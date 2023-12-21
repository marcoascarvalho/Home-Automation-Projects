/*;****************************************************************************************************
 Light Sensor Routines
****************************************************************************************************/
#include "application.h"
#include "light_sensor.h"


uint16_t lightsensorClass::DayCount = 0;
uint16_t lightsensorClass::NightCount = 0;


/*----------------------------------------------------------------------
 CheckForDusk --> Check Low Light
 treshold for low limit is 0x20
----------------------------------------------------------------------*/
bool lightsensorClass::CheckForNight(int CurrentLight)
{
  

   if(CurrentLight <= NIGHT_TRESHOLD) //if ambient is darker than threshold
   {
        NightCount++;
        Serial.print("NightCount:");
        Serial.println(NightCount);
        if(NightCount >= LIGHT_MINUTES) //if X consecutive minutes of light lower than threshold
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        NightCount = 0; //clear the LowLightCounter
        return false;
    }
}

/*;----------------------------------------------------------------------
; CheckForDawn --> Check High Light
; treshold for high limit is 0x50
;----------------------------------------------------------------------*/
bool lightsensorClass::CheckForDay(int CurrentLight)
{

    if(CurrentLight >= DAY_TRESHOLD) //if Ambient is brighter than threshold
    {
        DayCount++;
        Serial.print("DayCount:");
        Serial.println(DayCount);
        if(DayCount >= LIGHT_MINUTES) //if 5 consecutive minutes of light lower than threshold
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        DayCount = 0; //clear the LowLightCounter
        return false;
    }
}

/*;----------------------------------------------------------------------
;CheckLightLevel - Read LDR sensor (A0) and check for Day or Night
;
;----------------------------------------------------------------------*/
byte lightsensorClass::CheckCurrentLight()
{

   int LightSensorPin = A0;  
   int LightSensorVal;
   String Light1;
   
   
   /* Read Light Sensor */
   LightSensorVal = analogRead(LightSensorPin);
   Serial.print("LightSensorVal: ");
   Serial.println(LightSensorVal);
   
   //webhook
   //Light1 = String(LightSensorVal, DEC);
   //Particle.publish("Light", Light1, PRIVATE);

   if (CheckForNight(LightSensorVal) == true){
       return IS_NIGHT;
   }
   
   if (CheckForDay(LightSensorVal) == true){
       return IS_DAY;
   }
   
   return IS_EVENING_MORNING;
   
}
