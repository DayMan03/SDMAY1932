 
// Author: Tim Day
// Project: SDMAY1932 Hardware Synthesizer
// Algorithm for the mixer

#include <Wire.h>

//Global constants
int incomingByte = 0;
int done = 0;
int rStep = round(10000/255);
int RM = 10000;  //Mixer Circuit
int divider = 0;

void setup() {
  // put your setup code here, to run once:
   Wire.begin();
 Serial.begin(9600);
 
}

void loop() {
   while(done == 0){
  // Circuit Constants
  int i = 0;
  
  //Osc1 Vars
  double pOsc1 = 0;
  int comOsc1 = 0;

  //Osc2 Vars
  double pOsc2 = 0;
  int comOsc2 = 0;

  //Noise Vars
  double pNoise = 0;
  int comNoise = 0;

  //Ext1 Vars
  double pExt1 = 0;
  int comExt1 = 0;

   //Ext2 Vars
  double pExt2 = 0;
  int comExt2 = 0;

  //Ext3 Vars
  double pExt3 = 0;
  int comExt3 = 0;

  double I = 0;
  int resFeed = 0;
  
 
  //Set Oscillator 1
  Serial.print("Percentage for Osc1\n");
  while(i == 0){
    if(Serial.available()>0){
      pOsc1 = Serial.read();   
      pOsc1 = (pOsc1 - 48)*10; //char to number
      Serial.print("I received: ");
      Serial.println(pOsc1, DEC);
      delay(1000);
      i++;
      }
    }
  i = 0;

  //Set Oscillator 2
  Serial.print("\n\nPercentage for Osc2\n");
  while(i == 0){
    if(Serial.available()>0){
      pOsc2 = Serial.read();   
      pOsc2 = (pOsc2 - 48)*10;
      Serial.print("I received: ");
      Serial.print(pOsc2,DEC);
      Serial.print("\n");
       delay(1000);
        i++;
      }
  }
  i = 0;


  //Set Noise
  Serial.print("\n\nPercentage for Noise\n");
  while(i == 0){
    if(Serial.available()>0){
      pNoise = Serial.read();   
      pNoise = (pNoise - 48)*10;
      Serial.print("I received: ");
      Serial.print(pNoise,DEC);
      Serial.print("\n");
       delay(1000);
        i++;
      }
  }
  i = 0;

  //Set Ext1 
  Serial.print("\n\nPercentage for Ext1\n");
  while(i == 0){
    if(Serial.available()>0){
      pExt1 = Serial.read();   
      pExt1 = (pExt1 - 48)*10;
      Serial.print("I received: ");
      Serial.print(pExt1,DEC);
      Serial.print("\n");
       delay(1000);
        i++;
      }
  }
  i = 0;

  //Set Ext2
  Serial.print("\n\nPercentage for Ext2\n");
  while(i == 0){
    if(Serial.available()>0){
      pExt2 = Serial.read();   
      pExt2 = (pExt2 - 48)*10;
      Serial.print("I received: ");
      Serial.print(pExt2,DEC);
      Serial.print("\n");
       delay(1000);
        i++;
      }
  }
  i = 0;
  
    //Set Ext3
  Serial.print("\n\nPercentage for Ext3\n");
  while(i == 0){
    if(Serial.available()>0){
      pExt3 = Serial.read();   
      pExt3 = (pExt3 - 48)*10;
      Serial.print("I received: ");
      Serial.print(pExt3,DEC);
      Serial.print("\n");
       delay(1000);
        i++;
      }
  }
  i = 0;
       divider = Div(pOsc1,pOsc2,pNoise,pExt1,pExt2,pExt3);
       
       //Osc1 Algorithm 
      Serial.print("\nOscillator 1\n");
      pOsc1 = pOsc1/100;              //percentage of Osc1 scaled to high being 90 percent
      comOsc1 = (byte)attenRes(pOsc1);
      Serial.print("Digi preDivide: ");
      Serial.println(comOsc1, DEC);      
      comOsc1= comOsc1/divider;
      Serial.println(divider, DEC);
      Serial.print("Digipod Out: ");
      Serial.println(comOsc1, DEC);
      delay(1000);
      Serial.print("Transmission Begin\n");
      delay(1000);
      Wire.beginTransmission(0x28); //Transmit to certain device
      Wire.write(0xAA);            // sends instruction byte
      Wire.write(comOsc1);             // sends potentiometer value byte
      Wire.endTransmission();     // stop transmitting
      Serial.print("Transmission End\n");
      Serial.print("Osc1 Attenuated\n");

      //Osc2 Algorithm 
      Serial.print("\nOscillator 2\n");
      pOsc2 = pOsc2/100;              //percentage of Osc1 scaled to high being 90 percent
      comOsc2 = (byte)attenRes(pOsc2);
      Serial.print("Digi preDivide: ");
      Serial.println(comOsc2, DEC);
      comOsc2= comOsc2/divider;
      Serial.print("Divider: ");
      Serial.println(divider, DEC);
      Serial.print("Digipod Out: ");
      Serial.println(comOsc2, DEC);
      delay(1000);
      Serial.print("Transmission Begin\n");
      delay(1000);
      Wire.beginTransmission(0x28); 
      Wire.write(0xA9);            // sends instruction byte
      Wire.write(comOsc2);             // sends potentiometer value byte
      Wire.endTransmission();     // stop transmitting
      Serial.print("Transmission End\n");
      Serial.print("Osc2 Attenuated\n");

       //Noise Algorithm 
      Serial.print("\nNoise\n");
      pNoise = pNoise/100;              //percentage of Osc1 scaled to high being 90 percent
      comNoise = (byte)attenRes(pNoise);
      Serial.print("Digi preDivide: ");
      Serial.println(comNoise, DEC);
      comNoise = comNoise/divider;
      Serial.print("Divider: ");
      Serial.println(divider, DEC);
      Serial.print("Digipod Out: ");
      Serial.println(comNoise, DEC);
      delay(1000);
      Serial.print("Transmission Begin\n");
      delay(1000);
      Wire.beginTransmission(0x29); 
      Wire.write(0xAA);            // sends instruction byte
      Wire.write(comNoise);             // sends potentiometer value byte
      Wire.endTransmission();     // stop transmitting
      Serial.print("Transmission End\n");
      Serial.print("Osc2 Attenuated\n");

        //Ext1 
      Serial.print("\nExt1\n");
      pExt1 = pExt1/100;              //percentage of Osc1 scaled to high being 90 percent
      comExt1 = (byte)attenRes(pExt1);
      Serial.print("Digi preDivide: ");
      Serial.println(comExt1, DEC);
      comExt1 = comExt1/divider;
      Serial.print("Divider: ");
      Serial.println(divider, DEC);
      Serial.print("Digipod Out: ");
      Serial.println(comExt1, DEC);
      delay(1000);
      Serial.print("Transmission Begin\n");
      delay(1000);
      Wire.beginTransmission(0x29); 
      Wire.write(0xA9);            // sends instruction byte
      Wire.write(comExt1);             // sends potentiometer value byte
      Wire.endTransmission();     // stop transmitting
      Serial.print("Transmission End\n");
      Serial.print("Osc2 Attenuated\n");

        //Ext2 
      Serial.print("\nExt1\n");
      pExt2 = pExt2/100;              //percentage of Osc1 scaled to high being 90 percent
      comExt2 = (byte)attenRes(pExt2);
      Serial.print("Digi preDivide: ");
      Serial.println(comExt2, DEC);
      comExt2 = comExt2/divider;
      Serial.print("Divider: ");
      Serial.println(divider, DEC);
      Serial.print("Digipod Out: ");
      Serial.println(comExt2, DEC);
      delay(1000);
      Serial.print("Transmission Begin\n");
      delay(1000);
      Wire.beginTransmission(0x2A); 
      Wire.write(0xAA);            // sends instruction byte
      Wire.write(comExt2);             // sends potentiometer value byte
      Wire.endTransmission();     // stop transmitting
      Serial.print("Transmission End\n");
      Serial.print("Osc2 Attenuated\n");

      //Ext3 
      Serial.print("\nExt3\n");
      pExt3 = pExt3/100;              //percentage of Osc1 scaled to high being 90 percent
      comExt3 = (byte)attenRes(pExt3);
      Serial.print("Digi preDivide: ");
      Serial.println(comExt3, DEC);
      comExt3 = comExt3/divider;
      Serial.print("Divider: ");
      Serial.println(divider, DEC);
      Serial.print("Digipod Out: ");
      Serial.println(comExt3, DEC);
      delay(1000);
      Serial.print("Transmission Begin\n");
      delay(1000);
      Wire.beginTransmission(0x2A); 
      Wire.write(0xA9);            // sends instruction byte
      Wire.write(comExt3);             // sends potentiometer value byte
      Wire.endTransmission();     // stop transmitting
      Serial.print("Transmission End\n");
      Serial.print("Osc2 Attenuated\n");
  
  done = 1;
  delay(100);
  }
 }



int attenRes(double p)
{
  double pIn = 0;
  double resIn = 0;
  double comIn = 0;
  int comOut = 0;
  
 //percentage of Osc1 scaled to high being 90 percent
  resIn = (p*RM);     //attenuator adjustment
  comIn = resIn/rStep;            //divides by voltage steps
  comOut = comIn;

  Serial.print("Oscialtion Percent: ");
  Serial.println(p, DEC);

  
  return comOut;

} 

int Div(int comOsc1, int comOsc2, int comNoise, int comExt1, int comExt2, int comExt3){
  int  value = 0;
  int dividend = 0;

  if(comOsc1 == 0){
    value++;
      Serial.print("value: ");
  Serial.println(value, DEC);    
  }
  if(comOsc2 == 0){
    value++;
          Serial.print("value: ");
  Serial.println(value, DEC);  
  }
  if(comNoise == 0){
    value++;
          Serial.print("value: ");
  Serial.println(value, DEC);  
  }
  if(comExt1 == 0){
    value++;
          Serial.print("value: ");
  Serial.println(value, DEC);  
  }
  if(comExt2 == 0){
    value++;
          Serial.print("value: ");
  Serial.println(value, DEC);  
  }
  if(comExt3 == 0){
    value++;
          Serial.print("value: ");
  Serial.println(value, DEC);  
  }

  dividend = 6 - value;
  if (dividend == 0){
    dividend = 1;
  }
  Serial.print("Divider t: ");
  Serial.println(dividend, DEC);
  
  return dividend;

}
