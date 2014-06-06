EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "IR LED driver for low-current USB circuits"
Date "6 jun 2014"
Rev "2"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MOSFET_N Q1
U 1 1 538A337F
P 5300 3400
F 0 "Q1" H 5310 3570 60  0000 R CNN
F 1 "MOSFET_N" H 5310 3250 39  0000 R CNN
F 2 "~" H 5300 3400 60  0000 C CNN
F 3 "~" H 5300 3400 60  0000 C CNN
	1    5300 3400
	1    0    0    -1  
$EndComp
$Comp
L LED D2
U 1 1 538A33D5
P 5400 2900
F 0 "D2" H 5400 3000 50  0000 C CNN
F 1 "LED" H 5400 2800 50  0000 C CNN
F 2 "~" H 5400 2900 60  0000 C CNN
F 3 "~" H 5400 2900 60  0000 C CNN
	1    5400 2900
	0    1    1    0   
$EndComp
$Comp
L R R2
U 1 1 538A343D
P 5400 1850
F 0 "R2" V 5480 1850 40  0000 C CNN
F 1 "10R" V 5407 1851 40  0000 C CNN
F 2 "~" V 5330 1850 30  0000 C CNN
F 3 "~" H 5400 1850 30  0000 C CNN
	1    5400 1850
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 538A344C
P 4500 2500
F 0 "R1" V 4580 2500 40  0000 C CNN
F 1 "220R" V 4507 2501 40  0000 C CNN
F 2 "~" V 4430 2500 30  0000 C CNN
F 3 "~" H 4500 2500 30  0000 C CNN
	1    4500 2500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 538A3486
P 4900 3800
F 0 "#PWR?" H 4900 3800 30  0001 C CNN
F 1 "GND" H 4900 3730 30  0001 C CNN
F 2 "" H 4900 3800 60  0000 C CNN
F 3 "" H 4900 3800 60  0000 C CNN
	1    4900 3800
	1    0    0    -1  
$EndComp
$Comp
L +5V #PWR?
U 1 1 538A3495
P 4500 1650
F 0 "#PWR?" H 4500 1740 20  0001 C CNN
F 1 "+5V" H 4500 1740 30  0000 C CNN
F 2 "" H 4500 1650 60  0000 C CNN
F 3 "" H 4500 1650 60  0000 C CNN
	1    4500 1650
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 538A3572
P 4700 3050
F 0 "C2" H 4700 3150 40  0000 L CNN
F 1 "100nF" H 4706 2965 40  0000 L CNN
F 2 "~" H 4738 2900 30  0000 C CNN
F 3 "~" H 4700 3050 60  0000 C CNN
	1    4700 3050
	1    0    0    -1  
$EndComp
$Comp
L CP1 C1
U 1 1 538A3581
P 4300 3050
F 0 "C1" H 4350 3150 50  0000 L CNN
F 1 "1000uF" H 4350 2950 39  0000 L CNN
F 2 "~" H 4300 3050 60  0000 C CNN
F 3 "~" H 4300 3050 60  0000 C CNN
	1    4300 3050
	1    0    0    -1  
$EndComp
Text GLabel 5000 3400 0    39   Input ~ 0
signal
$Comp
L LED D1
U 1 1 5391B7D5
P 5400 2400
F 0 "D1" H 5400 2500 50  0000 C CNN
F 1 "LED" H 5400 2300 50  0000 C CNN
F 2 "~" H 5400 2400 60  0000 C CNN
F 3 "~" H 5400 2400 60  0000 C CNN
	1    5400 2400
	0    1    1    0   
$EndComp
$Comp
L DIODE D3
U 1 1 5391B809
P 4500 1950
F 0 "D3" H 4500 2050 40  0000 C CNN
F 1 "DIODE" H 4500 1850 40  0000 C CNN
F 2 "~" H 4500 1950 60  0000 C CNN
F 3 "~" H 4500 1950 60  0000 C CNN
	1    4500 1950
	0    1    1    0   
$EndComp
Wire Wire Line
	4500 2250 4500 2150
Wire Wire Line
	4500 1750 4500 1650
Wire Wire Line
	5050 1600 5400 1600
Wire Wire Line
	5400 2700 5400 2600
Wire Wire Line
	5400 2200 5400 2100
Wire Wire Line
	5000 3400 5100 3400
Connection ~ 4900 3700
Connection ~ 4500 3300
Wire Wire Line
	4500 3700 4500 3300
Connection ~ 4700 2800
Wire Wire Line
	5050 1600 5050 2800
Wire Wire Line
	4300 3300 4300 3250
Wire Wire Line
	4300 3300 4700 3300
Wire Wire Line
	4700 3300 4700 3250
Connection ~ 4500 2800
Wire Wire Line
	4300 2800 4300 2850
Wire Wire Line
	4700 2800 4700 2850
Wire Wire Line
	5050 2800 4300 2800
Wire Wire Line
	4500 2750 4500 2800
Wire Wire Line
	4900 3700 4900 3800
Wire Wire Line
	4500 3700 5400 3700
Wire Wire Line
	5400 3700 5400 3600
Wire Wire Line
	5400 3100 5400 3200
$EndSCHEMATC
