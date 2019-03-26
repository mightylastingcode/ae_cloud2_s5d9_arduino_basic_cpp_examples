/*********************************************************************
This is the main thread for placing Arduino Sketch code to run on Renesas AE Cloud2 S5D9 board

Created on: September 17, 2018
First Released on: March 19, 2019
Author: Michael Li (michael.li@miketechuniverse.com)

An Arduino sketch is placed inside the section that is defined by two comment lines.  The section has the
example of setup() and loop() functions.   You can replace them with your example code.  It is very simple.
Most common functions like Serial and Wire functions are supported except of the SPI functions which need to
be modified to run.


The MIT License (MIT)


Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.


*********************************************************************/



#include "main_thread.h"
#include <Arduino.h>
#include <Wire.h>
#include <Wire1.h>
#include "SERIAL.h"
#include "SERIAL1.h"
#include "SPI.h"
#include "SPI1.h"
#include <stdio.h>


//====================== Your Arduino Example Sketch Begin ===========//

const byte interruptPin0 = 8;   // trigger by int0 : toggle Led pin 0
const byte interruptPin1 = 9;   // trigger by int1 : toggle Led pin 1
const byte togglePin     = 2;   // Connect toggle pin 3 to interrupt pin 8 and 9
                                // to view led Pin 0 and led Pin 1.
const byte ledPin0       = 3;   // probe this pin to see the square wave.
const byte ledPin1       = 4;   // probe this pin to see the square wave.

volatile byte state0 = LOW;
volatile byte state1 = LOW;


void setup() {
  pinMode(togglePin, OUTPUT);
  pinMode(ledPin0, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  //pinMode(interruptPin, INPUT_PULLUP);  // This must be comment out since the pin is already set as interrupt pin.
  //attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);  // passing the function name is not working.
  attachInterrupt(digitalPinToInterrupt(interruptPin0), isr0, CHANGE);     // led pin 0 on/off toggles whenever the toggle pin changes state
  attachInterrupt(digitalPinToInterrupt(interruptPin1), isr1, RISING);     // led pin 1 on/off toggles only when the toggle pin changes from lo to hi.
}

void loop() {
    for (int i=0; i<4; i++) {
        digitalWrite(togglePin, HIGH);
        delay(1000);                  // waits for a second
        digitalWrite(togglePin, LOW);        // sets the digital pin off
        delay(1000);                  // waits for a second
    }

    // test for interrupt disable.
    noInterrupts();
    for (int i=0; i<4; i++) {
        digitalWrite(togglePin, HIGH);
        delay(1000);                  // waits for a second
        digitalWrite(togglePin, LOW);        // sets the digital pin off
        delay(1000);                  // waits for a second
    }
    interrupts();

}


//void blink(void){
void isr0(void) {   // isr0 must be the name of the recall function.
  state0 = !state0;
  digitalWrite(ledPin0, state0);
}

void isr1(void) {  // isr1 must be the name of the recall function.
   state1 = !state1;
   digitalWrite(ledPin1, state1);
}

//====================== Your Arduino Example Sketch End ===========//



//============================= Renesas Main Thread entry function ====================================//
void main_thread_entry(void)
{
   system_setup();
   setup();

    while (1) {
        loop();
        delay(1);  // required for the thread if delay()does not exist in the loop() function.
    }
}

