/*************************************************************************
 *
 *    Home Automation Project - scc1003 
 *
 *    (c) Copyright HomeAuto
 *
 *    File name   : scc1003.ino
 *    Description : Define a main function to control HA Project 
 *                  
 *    History :
 *    1003.0 Date        : Oct 22, 2016
 *           Author      : Marco Carvalho
 *           Description : Create
 *
 *    $Revision: 1003.0 $
**************************************************************************/
#include "x10.h"
#include "x10constants.h"
#include "HAProt.h"
#include "light_sensor.h"
#include "temperature.h"
//#include "coremacros.h"

#define BUFFSIZE 32

#define ONE_DAY_MILLIS (24 * 60 * 60 * 1000)
#define ONE_MINUTE (60 * 1000)

const int rxPin = D1;    // data receive pin
const int txPin = D3;    // data transmit pin
const int zcPin = D0;    // zero crossing pin

int ledPin = D7;

unsigned long lastSync = millis();

//x10Class x10 = x10Class();

TCPServer server = TCPServer(2222);
TCPClient client;

uint8_t SocketBuffer[BUFFSIZE];
uint8_t bytes_read = 0;
int CurrentLight;
int CurrentTemp;

void setup() {
  
  Time.zone(-3);
  Particle.syncTime();
  
  // initialize serial and X10:
  
  Serial.begin(9600);
  x10.begin(rxPin, txPin, zcPin);
  pinMode(ledPin, OUTPUT);
  
   /* Check current light */
   CurrentLight = lightsensor.CheckCurrentLight();
   
   /* Check current temperature */
   CurrentTemp = temperature.GetTemperatureValue();
   
   /*monitor variable status - IFTTT test */
   if (Particle.variable("LightVal", &CurrentLight, INT)==false)
   {
      Serial.println("variable not registered!");
   }
  
  // start listening for clients
  server.begin();
  
  // Now open your Serial Terminal, and hit any key to continue!
  while(!Serial.available()) Particle.process();
  
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.subnetMask());
  Serial.println(WiFi.gatewayIP());
  Serial.println(WiFi.SSID());
  Serial.println();
  //Time.zone(-3);
  //Time.setTime(1477731600);  // Sat Oct 29 09:00:00 2016
  Serial.println(Time.timeStr());
  
 
}

void loop() {
  
    uint8_t i;
    
    static volatile unsigned long lastFlag = 0;

    if (client.connected()) {
        // read all available bytes from the client
        bytes_read = client.available();
        if(bytes_read>0) {
            //Serial.println(bytes_read);
            for (i=0;i<bytes_read;i++) {
                SocketBuffer[i] = client.read();
                //Serial.println(SocketBuffer[i]);
                //Serial.println(bytes_read);
            }
            
            HAProcessCommand(SocketBuffer, bytes_read);
            /* reinicializa buffer de recepcao de dados do socket */
		    SocketBuffer[0] = '\0';
        }
    } else {
    // if no client is yet connected, check for a new connection
        client = server.available();
    }

    /* check current light level and temperature in each minute */
    if(millis() - lastFlag > ONE_MINUTE) {  
        lastFlag = millis();
        CurrentLight = lightsensor.CheckCurrentLight();
        CurrentTemp = temperature.GetTemperatureValue();
    }
  
    /* check timedate for units */
    for (i=0;i<MAX_UNITS;i++) {
    // ON first
        if (Units[i].CurrentState != ON) {
            if((Units[i].OnMonth == Time.month()) || (Units[i].RepeatMonth > 0)) {
                if((Units[i].OnDay == Time.day()) || ((Units[i].Weekday >> Time.weekday())&0x01)) {
                    if(Units[i].OnHour == Time.hour()) {
                        if(Units[i].OnMinute == Time.minute()) {
                            Serial.println("ON");
                            Units[i].LastState = Units[i].CurrentState;
                            Units[i].CurrentState = ON;
                            x10.write(House, Units[i].ID, 1);
                            x10.write(House, Units[i].CurrentState, 1);
                        }
                    }
                }
            }
        }
        //OFF
        if (Units[i].CurrentState != OFF) {
            if((Units[i].OffMonth == Time.month()) || (Units[i].RepeatMonth > 0)) {
                if((Units[i].OffDay == Time.day()) || ((Units[i].Weekday >> Time.weekday())&0x01)) {
                    if(Units[i].OffHour == Time.hour()) {
                        if(Units[i].OffMinute == Time.minute()) {
                            Serial.println("OFF");
                            Units[i].LastState = Units[i].CurrentState;
                            Units[i].CurrentState = OFF;
                            x10.write(House, Units[i].ID, 1);
                            x10.write(House, Units[i].CurrentState, 1);
                        }
                    }
                }
            }
        }
        
        /* light sensor calibration */
        /*
        CurrentLight = lightsensor.CheckCurrentLight();
        Serial.println("CurrentLight: ");
        Serial.print(CurrentLight);
        Serial.println();
        */
        
        /* control units based on light sensor */
        if (Units[i].LightSensorControl == true) {
            //Serial.println(i);
            if (CurrentLight == IS_DAY) {
                Serial.println("IS_DAY");
                if (Units[i].CurrentState != Units[i].LightDayValue) { // aqui pode dar um problema
                    Units[i].LastState = Units[i].CurrentState;
                    Units[i].CurrentState = Units[i].LightDayValue;
                    x10.write(House, Units[i].ID, 1);
                    x10.write(House, Units[i].CurrentState, 1);
                }
            }
            else if (CurrentLight == IS_NIGHT) {
                Serial.println("IS_NIGHT"); 
                if (Units[i].CurrentState != Units[i].LightNightValue) {
                    Units[i].LastState = Units[i].CurrentState;
                    Units[i].CurrentState = Units[i].LightNightValue;
                    x10.write(House, Units[i].ID, 1);
                    x10.write(House, Units[i].CurrentState, 1);
                }
            }
            else if (CurrentLight == IS_EVENING_MORNING) {
                Serial.println("IS_EVENING_MORNING");
            }
        }
    }


    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
  
    /* Time sync using particle cloud */
    if (millis() - lastSync > ONE_DAY_MILLIS) {
        // Request time synchronization from the Particle Cloud
        Particle.syncTime();
        lastSync = millis();
    }
  
}