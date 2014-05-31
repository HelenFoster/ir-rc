/*
Reads a comma-separated list of durations (in microseconds) from Serial,
and outputs the 38kHz modulated version on the specified pin,
using the Arduino tone function.

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

const int visibleLedPin = 13;
const int irLedPin = 3;

const int bufSize = 200;

unsigned long buffer[bufSize];
int items;

void setup() {
    Serial.begin(9600);
    while (!Serial);
    Serial.setTimeout(200);
    pinMode(visibleLedPin, OUTPUT);
    pinMode(irLedPin, OUTPUT);
}

void startBurst() {
    digitalWrite(visibleLedPin, HIGH);
    tone(irLedPin, 38000);
}

void stopBurst() {
    digitalWrite(visibleLedPin, LOW);
    noTone(irLedPin);
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
