/*
Duration-based IR remote control transmission, with PIC serial.
Using PIC12F683 with XC8 compiler.
Accepts binary data via a software UART at 9600 baud. Echoes data + error info.
First byte is carrier period; second byte is multiplier for burst durations;
 remaining bytes for durations on-off-on-etc (all times in microseconds).

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

#include <xc.h>
#include <string.h>

// CONFIG
#pragma config FOSC = INTOSCIO  // Internal Oscillator enabled; use pins for IO
#pragma config WDTE = OFF       // Watchdog Timer disabled
#pragma config PWRTE = OFF      // Power-up Timer disabled
#pragma config MCLRE = ON       // MCLR pin enabled
#pragma config CP = OFF         // Code Protection bit disabled
#pragma config CPD = OFF        // Data Code Protection bit disabled
#pragma config BOREN = OFF      // Brown Out Detect disabled
#pragma config IESO = OFF       // Internal External Switchover disabled
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor disabled

#define UTX_PIN GPIObits.GP5
#define URX_PIN GPIObits.GP4
// IRTX is GP2 for PWM
#define DEBUG_PIN GPIObits.GP0
#define UTX_DELAY 98
#define URX_DELAY 91

#define URX_OK 0
#define URX_ERROR_STOP_BIT 1

#define DATA_OK 0
#define DATA_BAD_FREQ 1
#define DATA_BAD_MUL 2
#define DATA_WARN_FULL 3

//defined here to allow inversion
#define UART_LOW 0
#define UART_HIGH 1

#define BUF_SIZE 80

unsigned char n;
unsigned char buf[BUF_SIZE];

unsigned char T0_ps(unsigned char ps) {
    asm("CLRWDT");
    TMR0 = 0;
    if (ps == 1) {
        OPTION_REGbits.PSA = 1;
    } else {
        OPTION_REGbits.PSA = 0;
        switch(ps) {
            case 2: OPTION_REGbits.PS = 0; break;
            case 4: OPTION_REGbits.PS = 1; break;
            case 8: OPTION_REGbits.PS = 2; break;
            case 16: OPTION_REGbits.PS = 3; break;
            case 32: OPTION_REGbits.PS = 4; break;
            case 64: OPTION_REGbits.PS = 5; break;
            case 128: OPTION_REGbits.PS = 6; break;
            case 0: OPTION_REGbits.PS = 7; break;
            default: return DATA_BAD_MUL;
        }
    }
    return DATA_OK;
}

void wait_T0(unsigned char until) {
    while (TMR0 < until);
    TMR0 = 0;
}

void wait_T2() {
    PIR1bits.TMR2IF = 0;
    while (PIR1bits.TMR2IF == 0);
    PIR1bits.TMR2IF = 0;
}

void uart_tx() {
    unsigned char i, j, c;
    T0_ps(1);
    for (i = 0; i < n; i ++) {
        while (TMR0 < UTX_DELAY);
        UTX_PIN = UART_LOW;
        TMR0 = 0;
        c = buf[i];
        for (j = 0; j < 8; j ++) {
            if (c & 1) {
                while (TMR0 < UTX_DELAY);
                UTX_PIN = UART_HIGH;
                TMR0 = 0;
            } else {
                while (TMR0 < UTX_DELAY);
                UTX_PIN = UART_LOW;
                TMR0 = 0;
            }
            c >>= 1;
        }
        while (TMR0 < UTX_DELAY);
        UTX_PIN = UART_HIGH;
        TMR0 = 0;
    }

}

unsigned char uart_rx() {
    unsigned char c, j, t;
    const unsigned char delay2 = URX_DELAY / 2 + URX_DELAY;
    T0_ps(1);
    n = 0;
    DEBUG_PIN = 1;
    while (URX_PIN == UART_HIGH);
    TMR0 = 0;
    DEBUG_PIN = 0;
    while (n < BUF_SIZE) {
        while (TMR0 < delay2);
        TMR0 = 0;
        c = 0;
        for (j = 0; j < 8; j ++) {
            c >>= 1;
            if (URX_PIN == UART_HIGH) {
                c |= 0x80;
                DEBUG_PIN = 1;
            } else {
                DEBUG_PIN = 0;
            }
            while (TMR0 < URX_DELAY);
            TMR0 = 0;
        }
        if (URX_PIN == UART_LOW) {
            //wrong stop bit
            return URX_ERROR_STOP_BIT;
        }
        buf[n++] = c;
        DEBUG_PIN = 1;
        t = 0;
        while (URX_PIN == UART_HIGH) {
            if (TMR0 >= URX_DELAY) {
                TMR0 = 0;
                t ++;
                if (t >= 20) {
                    return URX_OK;
                }
            }
        }
        TMR0 = 0;
        DEBUG_PIN = 0;
    }
    return URX_OK;
}

unsigned char ir_tx() {
    unsigned char c, i, r, lv;
    
    //setup PWM
    TRISIObits.TRISIO2 = 1;
    PR2 = buf[0];
    CCP1CONbits.CCP1M = 0b1110; //PWM active low
    CCPR1L = buf[0] >> 1; CCP1CONbits.DC1B = 0;
    PIR1bits.TMR2IF = 0; //clear interrupt
    T2CONbits.T2CKPS = 0; //1:1 prescaler
    T2CONbits.TMR2ON = 1;

    r = T0_ps(buf[1]);
    if (r != DATA_OK) {
        return r;
    }

    DEBUG_PIN = 1;
    lv = 1;
    wait_T0(0);
    wait_T2();
    for (i = 2; i < n; i ++) {
        DEBUG_PIN = lv;
        if (lv) {
            TRISIObits.TRISIO2 = 0;
        } else {
            TRISIObits.TRISIO2 = 1;
        }
        lv = 1 - lv;
        c = buf[i];
        wait_T0(c);
        wait_T2();
    }
    DEBUG_PIN = 0;
    TRISIObits.TRISIO2 = 1;
    if (n == BUF_SIZE) {
        return DATA_WARN_FULL;
    } else {
        return DATA_OK;
    }
}

void uart_tx_str(const char *s) {
    strcpy(buf, s);
    n = strlen(s);
    uart_tx();
}

void main(void) {
    unsigned char r;
    ANSEL = 0;
    WPUbits.WPU2 = 0;
    OPTION_REGbits.nGPPU = 0;
    OPTION_REGbits.T0CS = 0; //timer0 internal clock
    TRISIObits.TRISIO5 = 0; //GP5 output
    TRISIObits.TRISIO0 = 0;
    DEBUG_PIN = 0;

    //test ir_tx and uart_tx for when not connected to real uart
    buf[0] = 25;
    buf[1] = 32;
    buf[2] = 10;
    buf[3] = 20;
    buf[4] = 10;
    n = 5;
    ir_tx();
    uart_tx();
    T0_ps(0);
    wait_T0(255);
    
    while (1) {
        r = uart_rx();
        if (r == URX_OK) {
            r = ir_tx();
            uart_tx();
            if (r == DATA_OK) {
                uart_tx_str(" ok\n");
            } else if (r == DATA_WARN_FULL) {
                uart_tx_str(" full\n");
            } else {
                uart_tx_str(" unknown error\n");
            }
        } else {
            uart_tx();
            uart_tx_str(" UART RX error\n");
        }
    }
    return;
}
