/*
Monitors a digital signal on the specified pin, waits for trigger level,
and outputs a list of comma-separate durations (in microseconds) on Serial.
The first duration applies to the trigger level, alternating afterwards.

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

const int sensorPin = 11;
const int ledPin = 13;

const byte trigLevel = LOW;
const int bufSize = 800;
const unsigned long timeout_us = 200000;
const byte shift = 3;

unsigned short buffer[bufSize];
int bufPos;
byte prevLevel;
unsigned long prevTime;

void setup() {
    Serial.begin(9600);
    while (!Serial);
    pinMode(ledPin, OUTPUT);
}

void startScan() {
    while (digitalRead(sensorPin) != trigLevel);
    bufPos = 0;
    prevLevel = trigLevel;
    prevTime = micros();
}

byte nextSample() {
    byte level = digitalRead(sensorPin);
    unsigned long time = micros();
    unsigned long tdiff = time - prevTime;
    if (level != prevLevel || tdiff > timeout_us) {
        buffer[bufPos] = (unsigned short)(tdiff >> shift);
        bufPos ++;
        if (bufPos >= bufSize) {
            return 1;
        }
        if (tdiff > timeout_us) {
            return 2;
        }
        prevTime = time;
        prevLevel = level;
    }
    return 0;
}

void sendResults() {
    int i;
    for (i = 0; i < bufPos; i ++) {
        Serial.print(((unsigned long)(buffer[i])) << shift);
        Serial.print(',');
    }
    Serial.println();
}

void loop() {
    digitalWrite(ledPin, HIGH);
    startScan();
    digitalWrite(ledPin, LOW);
    while(nextSample() == 0);
    sendResults();
    delay(200);
}
