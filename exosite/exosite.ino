//*****************************************************************************
//
// fluid_simple_cloud.ino - Main application function
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
#include <RCSwitch.h>
#define couloir 12449942
#define porte 8851353


// Fill in your MAC address and CIK 
byte macData[] = { 0x90, 0xA2, 0xDA, 0x00, 0x11, 0x22}; // <-- Fill in your MAC here! (You can find it on the bottom of Ethernet Shield)

String cikData = "3108be0930109d42b1ca0eb6bd66e08d10949450";      // <-- Fill in your CIK here! (https://portals.exosite.com -> Add Device)

// local Macros
#define ONE_WIRE_BUS 6

// global
Exosite exosite(&Ethernet, macData, cikData);

RCSwitch mySwitch = RCSwitch();



/*==============================================================================
* setup
*
* Arduino setup function.
*=============================================================================*/
void setup() 
{
  exosite.init();
  mySwitch.enableReceive(0);
}

/*==============================================================================
* loop
*
* Arduino loop function.
*=============================================================================*/
void loop() 
{
    if (mySwitch.available()) {

    int value = mySwitch.getReceivedValue();

    while (!Serial) ;

    switch (value) {
        case porte:
            exosite.sendToCloud("2", 1);
            delay(1000);
            break;

        case couloir:
            exosite.sendToCloud("1", 2);
            delay(1000);
            break;

        default:
            exosite.sendToCloud("1", 3);
            delay(500);
            break;
    }

    mySwitch.resetAvailable();
}
}
