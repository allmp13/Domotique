//*****************************************************************************
//
// cloud_read_write.ino - Simple read/write sample for the Exosite Cloud API
//
// Copyright (c) 2012 Exosite LLC.  All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:

//  * Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of Exosite LLC nor the names of its contributors may
//    be used to endorse or promote products derived from this software 
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
//*****************************************************************************

#include <SPI.h>
#include <Ethernet.h>
#include <Exosite.h>

//global variables
byte macData[] = { 0x90, 0xA2, 0xDA, 0x00, 0xF6, 0x27  }; // <-- Fill in your MAC here! (e.g. {0x90, 0xA2, 0xDA, 0x00, 0x22, 0x33}) 

String cikData = "3108be0930109d42b1ca0eb6bd66e08d10949450";      // <-- Fill in your CIK here! (https://portals.exosite.com -> Add Device)

Exosite exosite(&Ethernet, macData, cikData);

  float mesure=0;        //Variable pour le stockage mesure retournée par le capteur
  float tension=0;       //Variable pour le stockage de la tension 
  float temperature=0;   //Variable pour le stockage de la température 
  float tempmin=100;     //Temperature min
  float tempmax=0;       //Temperature max
  int portana0 = 5;      //Numéro du port analogique sur lequel la température est mesurée


/*==============================================================================
* setup
*
* Arduino setup function.
*=============================================================================*/
void setup()
{
  exosite.init();
      analogReference(INTERNAL); //Permet de fixer la temperature de refernce à 1,1 volt
    Serial.begin(9600);
}

/*==============================================================================
* loop 
*
* Arduino loop function.
*=============================================================================*/
void loop()
{

  String retVal;
  mesure = analogRead(portana0);  //Lecture de la valeur fournie par le capteur de température
  tension = mesure * 1.1 / 1024;  //Conversion en tension (en volt)
  temperature = tension * 100;    //Conversion en température (en degré Celsius)  delay(1000);
  Serial.println(temperature); 

    //Send a value + 100 to Exosite, use the alias (resource name) "1"
    exosite.sendToCloud("1", temperature);

  delay(10000);
}
