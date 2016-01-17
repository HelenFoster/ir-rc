/*
Reads a comma-separated list of durations (in microseconds) from Serial,
and outputs the 38kHz modulated version on the specified pin.

If SPECIAL_PWM is defined, uses timer1 PWM on Arduino pin 10
(intended for SparkFun Pro Micro, but should work on other ATmega32U4 boards, 
and also ATmega328 which has the same timer1 settings).
Otherwise uses the Arduino tone function, as in previous versions.

Copyright (c) 2014, Helen Foster

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
*/

#define SPECIAL_PWM
const int irLedPin = 10; //should be 10 for special PWM

const int visibleLedPin = 17;
const int visibleLedOn = LOW;
const int visibleLedOff = HIGH;

const int bufSize = 200;

unsigned long buffer[bufSize];
int items;

void setup() {
    Serial.begin(9600);
    while (!Serial);
    Serial.setTimeout(200);
    pinMode(visibleLedPin, OUTPUT);
    pinMode(irLedPin, OUTPUT);

#ifdef SPECIAL_PWM
    //fast PWM with period OCR1A: WGM13:0 = 15
    //no clock prescaler (16MHz clock): CS12:0 = 1
    TCCR1A = _BV(WGM11) | _BV(WGM10);
    TCCR1B = _BV(WGM13) | _BV(WGM12)   | _BV(CS10);
    OCR1A = 421; //for 38Khz
    OCR1B = 100; //cycles it's on for
#endif
}

void startBurst() {
    digitalWrite(visibleLedPin, visibleLedOn);
#ifdef SPECIAL_PWM
    //output B non-inverting: COM1B1:0 = 2
    TCCR1A |= _BV(COM1B1);
#else
    tone(irLedPin, 38000);
#endif
}

void stopBurst() {
    digitalWrite(visibleLedPin, visibleLedOff);
#ifdef SPECIAL_PWM
    //turn off PWM output
    TCCR1A &= ~_BV(COM1B1);
#else
    noTone(irLedPin);
#endif
}

void loop() {
    int i = 0;
    byte level = 1;
    unsigned long time, prevTime;
    if (Serial.available()) {
        long data;
        while (data = Serial.parseInt()) {
            if (i < bufSize) {
                buffer[i++] = data;
            }
        }
        items = i;
        Serial.print("received ");
        Serial.println(items);
        prevTime = micros();
        for (i = 0; i < items; i ++) {
            unsigned long us = buffer[i];
            if (level) {
                startBurst();
            } else {
                stopBurst();
            }
            while ((time=micros()) - prevTime < us);
            prevTime = time;
            level = !level;
        }
    }
    stopBurst();
}
