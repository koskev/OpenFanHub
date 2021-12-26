EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 5
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Device:Thermistor TH1
U 1 1 61C86A11
P 5300 1200
F 0 "TH1" H 5405 1246 50  0000 L CNN
F 1 "Thermistor" H 5405 1155 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Horizontal" H 5300 1200 50  0001 C CNN
F 3 "~" H 5300 1200 50  0001 C CNN
	1    5300 1200
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR013
U 1 1 61C87428
P 5300 900
F 0 "#PWR013" H 5300 750 50  0001 C CNN
F 1 "+5V" H 5315 1073 50  0000 C CNN
F 2 "" H 5300 900 50  0001 C CNN
F 3 "" H 5300 900 50  0001 C CNN
	1    5300 900 
	1    0    0    -1  
$EndComp
Text Label 5900 1400 2    50   ~ 0
TEMP1_IN
Wire Wire Line
	5300 1400 5900 1400
$Comp
L power:GND #PWR021
U 1 1 61C8879B
P 7700 2450
F 0 "#PWR021" H 7700 2200 50  0001 C CNN
F 1 "GND" V 7705 2322 50  0000 R CNN
F 2 "" H 7700 2450 50  0001 C CNN
F 3 "" H 7700 2450 50  0001 C CNN
	1    7700 2450
	0    1    1    0   
$EndComp
$Comp
L Device:R R1
U 1 1 61C88C91
P 5300 1650
F 0 "R1" H 5370 1696 50  0000 L CNN
F 1 "R" H 5370 1605 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 5230 1650 50  0001 C CNN
F 3 "~" H 5300 1650 50  0001 C CNN
	1    5300 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 1500 5300 1400
Connection ~ 5300 1400
Wire Wire Line
	5300 900  5300 1000
Wire Wire Line
	5300 1900 5300 1800
$Comp
L Device:Thermistor TH2
U 1 1 61C8E99E
P 6200 1200
F 0 "TH2" H 6305 1246 50  0000 L CNN
F 1 "Thermistor" H 6305 1155 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Horizontal" H 6200 1200 50  0001 C CNN
F 3 "~" H 6200 1200 50  0001 C CNN
	1    6200 1200
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR017
U 1 1 61C8E9A4
P 6200 900
F 0 "#PWR017" H 6200 750 50  0001 C CNN
F 1 "+5V" H 6215 1073 50  0000 C CNN
F 2 "" H 6200 900 50  0001 C CNN
F 3 "" H 6200 900 50  0001 C CNN
	1    6200 900 
	1    0    0    -1  
$EndComp
Text Label 6800 1400 2    50   ~ 0
TEMP2_IN
Wire Wire Line
	6200 1400 6800 1400
$Comp
L power:GND #PWR018
U 1 1 61C8E9AC
P 6200 1900
F 0 "#PWR018" H 6200 1650 50  0001 C CNN
F 1 "GND" H 6205 1727 50  0000 C CNN
F 2 "" H 6200 1900 50  0001 C CNN
F 3 "" H 6200 1900 50  0001 C CNN
	1    6200 1900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 61C8E9B2
P 6200 1650
F 0 "R2" H 6270 1696 50  0000 L CNN
F 1 "R" H 6270 1605 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 6130 1650 50  0001 C CNN
F 3 "~" H 6200 1650 50  0001 C CNN
	1    6200 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 1500 6200 1400
Connection ~ 6200 1400
Wire Wire Line
	6200 900  6200 1000
Wire Wire Line
	6200 1900 6200 1800
$Comp
L my_components:USB_HEADER U3
U 1 1 61C934C2
P 8000 2250
F 0 "U3" H 8050 2375 50  0000 C CNN
F 1 "USB_HEADER_IN" H 8050 2284 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x05_P2.54mm_Vertical" H 7900 2250 50  0001 C CNN
F 3 "" H 7900 2250 50  0001 C CNN
	1    8000 2250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR023
U 1 1 61C946E8
P 8400 2450
F 0 "#PWR023" H 8400 2200 50  0001 C CNN
F 1 "GND" V 8405 2322 50  0000 R CNN
F 2 "" H 8400 2450 50  0001 C CNN
F 3 "" H 8400 2450 50  0001 C CNN
	1    8400 2450
	0    -1   -1   0   
$EndComp
NoConn ~ 7700 2750
NoConn ~ 8400 2750
$Comp
L power:GND #PWR024
U 1 1 61C98CC0
P 9100 2450
F 0 "#PWR024" H 9100 2200 50  0001 C CNN
F 1 "GND" V 9105 2322 50  0000 R CNN
F 2 "" H 9100 2450 50  0001 C CNN
F 3 "" H 9100 2450 50  0001 C CNN
	1    9100 2450
	0    1    1    0   
$EndComp
$Comp
L my_components:USB_HEADER U4
U 1 1 61C98CC6
P 9400 2250
F 0 "U4" H 9450 2375 50  0000 C CNN
F 1 "USB_HEADER_OUT" H 9450 2284 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_2x05_P2.54mm_Vertical" H 9300 2250 50  0001 C CNN
F 3 "" H 9300 2250 50  0001 C CNN
	1    9400 2250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR025
U 1 1 61C98CCC
P 9800 2450
F 0 "#PWR025" H 9800 2200 50  0001 C CNN
F 1 "GND" V 9805 2322 50  0000 R CNN
F 2 "" H 9800 2450 50  0001 C CNN
F 3 "" H 9800 2450 50  0001 C CNN
	1    9800 2450
	0    -1   -1   0   
$EndComp
NoConn ~ 9100 2750
NoConn ~ 9800 2750
Wire Wire Line
	8400 2550 9100 2550
Wire Wire Line
	8400 2650 9100 2650
NoConn ~ 9800 2550
NoConn ~ 9800 2650
Wire Wire Line
	8400 2350 8800 2350
Wire Wire Line
	7700 2350 7700 2000
Wire Wire Line
	7700 2000 8550 2000
Wire Wire Line
	9800 2000 9800 2350
$Comp
L my_components:STM32F103C8T8_Board U1
U 1 1 61C9F093
P 4600 3050
F 0 "U1" H 4800 3097 60  0000 C CNN
F 1 "STM32F103C8T8_Board" H 4800 2991 60  0000 C CNN
F 2 "Package_DIP:DIP-40_W15.24mm_Socket" H 4600 3050 60  0001 C CNN
F 3 "" H 4600 3050 60  0001 C CNN
	1    4600 3050
	1    0    0    -1  
$EndComp
Text Label 7350 2650 0    50   ~ 0
USB_D+
Text Label 7350 2550 0    50   ~ 0
USB_D-
Wire Wire Line
	7350 2550 7700 2550
Wire Wire Line
	7700 2650 7350 2650
Text Label 5650 4350 2    50   ~ 0
USB_D+
Text Label 5650 4450 2    50   ~ 0
USB_D-
Wire Wire Line
	5650 4450 5300 4450
Wire Wire Line
	5300 4350 5650 4350
$Comp
L power:GND #PWR010
U 1 1 61CA3F9D
P 4250 5100
F 0 "#PWR010" H 4250 4850 50  0001 C CNN
F 1 "GND" V 4255 4972 50  0000 R CNN
F 2 "" H 4250 5100 50  0001 C CNN
F 3 "" H 4250 5100 50  0001 C CNN
	1    4250 5100
	0    1    1    0   
$EndComp
Wire Wire Line
	4300 5050 4250 5050
Wire Wire Line
	4250 5050 4250 5100
Wire Wire Line
	4300 5150 4250 5150
Wire Wire Line
	4250 5150 4250 5100
Connection ~ 4250 5100
$Comp
L power:GND #PWR016
U 1 1 61CA4D35
P 5300 3350
F 0 "#PWR016" H 5300 3100 50  0001 C CNN
F 1 "GND" V 5305 3222 50  0000 R CNN
F 2 "" H 5300 3350 50  0001 C CNN
F 3 "" H 5300 3350 50  0001 C CNN
	1    5300 3350
	0    -1   -1   0   
$EndComp
$Comp
L power:+3.3V #PWR011
U 1 1 61CA5DE7
P 4300 4950
F 0 "#PWR011" H 4300 4800 50  0001 C CNN
F 1 "+3.3V" V 4315 5078 50  0000 L CNN
F 2 "" H 4300 4950 50  0001 C CNN
F 3 "" H 4300 4950 50  0001 C CNN
	1    4300 4950
	0    -1   -1   0   
$EndComp
$Comp
L power:+3.3V #PWR015
U 1 1 61CA753D
P 5300 3250
F 0 "#PWR015" H 5300 3100 50  0001 C CNN
F 1 "+3.3V" V 5315 3378 50  0000 L CNN
F 2 "" H 5300 3250 50  0001 C CNN
F 3 "" H 5300 3250 50  0001 C CNN
	1    5300 3250
	0    1    1    0   
$EndComp
Text Label 3900 4050 0    50   ~ 0
TEMP1_IN
Text Label 3900 4150 0    50   ~ 0
TEMP2_IN
Wire Wire Line
	3900 4050 4300 4050
Wire Wire Line
	3900 4150 4300 4150
Text Label 3850 3650 0    50   ~ 0
FAN1_PWM
Text Label 3850 3750 0    50   ~ 0
FAN1_RPM
Wire Wire Line
	4300 3750 3850 3750
Wire Wire Line
	3850 3650 4300 3650
Text Label 3850 3850 0    50   ~ 0
FAN2_PWM
Text Label 3850 3950 0    50   ~ 0
FAN2_RPM
Wire Wire Line
	3850 3950 4300 3950
Wire Wire Line
	4300 3850 3850 3850
Text Label 3850 4250 0    50   ~ 0
FAN3_PWM
Text Label 3850 4350 0    50   ~ 0
FAN3_RPM
Wire Wire Line
	4300 4350 3850 4350
Wire Wire Line
	3850 4250 4300 4250
Text Label 3850 4450 0    50   ~ 0
FAN4_PWM
Text Label 3850 4550 0    50   ~ 0
FAN4_RPM
Wire Wire Line
	4300 4550 3850 4550
Wire Wire Line
	3850 4450 4300 4450
Text Label 5750 3750 2    50   ~ 0
FAN5_PWM
Text Label 5750 3850 2    50   ~ 0
FAN5_RPM
Wire Wire Line
	5300 3750 5750 3750
Wire Wire Line
	5750 3850 5300 3850
Text Label 5750 3550 2    50   ~ 0
FAN6_PWM
Text Label 5750 3650 2    50   ~ 0
FAN6_RPM
Wire Wire Line
	5300 3550 5750 3550
Wire Wire Line
	5750 3650 5300 3650
$Sheet
S 1850 1200 550  1700
U 61CBD2D4
F0 "PWM_GROUPS" 50
F1 "pwm_groups.sch" 50
F2 "FAN1_PWM" I L 1850 1400 50 
F3 "FAN1_RPM" I L 1850 1500 50 
F4 "FAN2_PWM" I L 1850 1700 50 
F5 "FAN2_RPM" I L 1850 1800 50 
F6 "FAN3_PWM" I L 1850 1950 50 
F7 "FAN3_RPM" I L 1850 2050 50 
F8 "FAN4_RPM" I L 1850 2200 50 
F9 "FAN4_PWM" I L 1850 2300 50 
F10 "FAN5_RPM" I L 1850 2450 50 
F11 "FAN5_PWM" I L 1850 2550 50 
F12 "FAN6_RPM" I L 1850 2700 50 
F13 "FAN6_PWM" I L 1850 2800 50 
$EndSheet
Text Label 1400 1400 0    50   ~ 0
FAN1_PWM
Text Label 1400 1500 0    50   ~ 0
FAN1_RPM
Wire Wire Line
	1850 1500 1400 1500
Wire Wire Line
	1400 1400 1850 1400
Text Label 1400 1700 0    50   ~ 0
FAN2_PWM
Text Label 1400 1800 0    50   ~ 0
FAN2_RPM
Wire Wire Line
	1400 1800 1850 1800
Wire Wire Line
	1850 1700 1400 1700
Text Label 1400 1950 0    50   ~ 0
FAN3_PWM
Text Label 1400 2050 0    50   ~ 0
FAN3_RPM
Wire Wire Line
	1850 2050 1400 2050
Wire Wire Line
	1400 1950 1850 1950
Text Label 1400 2200 0    50   ~ 0
FAN4_PWM
Text Label 1400 2300 0    50   ~ 0
FAN4_RPM
Wire Wire Line
	1850 2300 1400 2300
Wire Wire Line
	1400 2200 1850 2200
Text Label 1400 2450 0    50   ~ 0
FAN5_PWM
Text Label 1400 2550 0    50   ~ 0
FAN5_RPM
Wire Wire Line
	1400 2450 1850 2450
Wire Wire Line
	1850 2550 1400 2550
Text Label 1400 2700 0    50   ~ 0
FAN6_PWM
Text Label 1400 2800 0    50   ~ 0
FAN6_RPM
Wire Wire Line
	1400 2700 1850 2700
Wire Wire Line
	1850 2800 1400 2800
$Comp
L power:GND #PWR014
U 1 1 61CCFAF1
P 5300 1900
F 0 "#PWR014" H 5300 1650 50  0001 C CNN
F 1 "GND" H 5305 1727 50  0000 C CNN
F 2 "" H 5300 1900 50  0001 C CNN
F 3 "" H 5300 1900 50  0001 C CNN
	1    5300 1900
	1    0    0    -1  
$EndComp
Text Label 5600 4750 2    50   ~ 0
ARGB1
Wire Wire Line
	5600 4750 5300 4750
Text Label 5600 4650 2    50   ~ 0
ARGB2
Text Label 5600 4550 2    50   ~ 0
ARGB3
Wire Wire Line
	5600 4550 5300 4550
Wire Wire Line
	5600 4650 5300 4650
$Comp
L my_components:AMS1117 U2
U 1 1 61CD5E20
P 7200 3550
F 0 "U2" H 7250 3597 60  0000 C CNN
F 1 "AMS1117" H 7250 3491 60  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-223" H 7200 3550 60  0001 C CNN
F 3 "" H 7200 3550 60  0000 C CNN
	1    7200 3550
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR019
U 1 1 61CD64B8
P 6800 3850
F 0 "#PWR019" H 6800 3700 50  0001 C CNN
F 1 "+5V" V 6815 3978 50  0000 L CNN
F 2 "" H 6800 3850 50  0001 C CNN
F 3 "" H 6800 3850 50  0001 C CNN
	1    6800 3850
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7700 3750 7700 3850
$Comp
L Device:R R3
U 1 1 61CD8463
P 7250 4400
F 0 "R3" H 7180 4354 50  0000 R CNN
F 1 "R" H 7180 4445 50  0000 R CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 7180 4400 50  0001 C CNN
F 3 "~" H 7250 4400 50  0001 C CNN
	1    7250 4400
	-1   0    0    1   
$EndComp
$Comp
L Device:R R4
U 1 1 61CD95B0
P 7500 4550
F 0 "R4" V 7707 4550 50  0000 C CNN
F 1 "R" V 7616 4550 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 7430 4550 50  0001 C CNN
F 3 "~" H 7500 4550 50  0001 C CNN
	1    7500 4550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7700 3850 7700 4550
Wire Wire Line
	7700 4550 7650 4550
Connection ~ 7700 3850
Wire Wire Line
	7250 4550 7350 4550
$Comp
L power:GND #PWR020
U 1 1 61CDCA64
P 7250 4600
F 0 "#PWR020" H 7250 4350 50  0001 C CNN
F 1 "GND" H 7255 4427 50  0000 C CNN
F 2 "" H 7250 4600 50  0001 C CNN
F 3 "" H 7250 4600 50  0001 C CNN
	1    7250 4600
	1    0    0    -1  
$EndComp
Wire Wire Line
	7250 4600 7250 4550
Connection ~ 7250 4550
$Comp
L power:+3.3V #PWR022
U 1 1 61CDE76D
P 7800 3850
F 0 "#PWR022" H 7800 3700 50  0001 C CNN
F 1 "+3.3V" V 7815 3978 50  0000 L CNN
F 2 "" H 7800 3850 50  0001 C CNN
F 3 "" H 7800 3850 50  0001 C CNN
	1    7800 3850
	0    1    1    0   
$EndComp
Wire Wire Line
	7800 3850 7700 3850
NoConn ~ 4300 3350
NoConn ~ 4300 3450
NoConn ~ 4300 3550
NoConn ~ 4300 3250
NoConn ~ 4300 4650
NoConn ~ 4300 4750
NoConn ~ 4300 4850
NoConn ~ 5300 5150
NoConn ~ 5300 5050
NoConn ~ 5300 4950
NoConn ~ 5300 4850
NoConn ~ 5300 4250
NoConn ~ 5300 4150
NoConn ~ 5300 4050
NoConn ~ 5300 3950
NoConn ~ 5300 3450
$Comp
L power:PWR_FLAG #FLG04
U 1 1 61CF584B
P 4500 800
F 0 "#FLG04" H 4500 875 50  0001 C CNN
F 1 "PWR_FLAG" H 4500 973 50  0000 C CNN
F 2 "" H 4500 800 50  0001 C CNN
F 3 "~" H 4500 800 50  0001 C CNN
	1    4500 800 
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR012
U 1 1 61CF694F
P 4500 950
F 0 "#PWR012" H 4500 800 50  0001 C CNN
F 1 "+5V" H 4515 1123 50  0000 C CNN
F 2 "" H 4500 950 50  0001 C CNN
F 3 "" H 4500 950 50  0001 C CNN
	1    4500 950 
	-1   0    0    1   
$EndComp
Wire Wire Line
	4500 950  4500 800 
$Comp
L power:+12V #PWR09
U 1 1 61CF8B16
P 4200 1000
F 0 "#PWR09" H 4200 850 50  0001 C CNN
F 1 "+12V" H 4215 1173 50  0000 C CNN
F 2 "" H 4200 1000 50  0001 C CNN
F 3 "" H 4200 1000 50  0001 C CNN
	1    4200 1000
	-1   0    0    1   
$EndComp
$Comp
L power:PWR_FLAG #FLG03
U 1 1 61CF9652
P 4200 800
F 0 "#FLG03" H 4200 875 50  0001 C CNN
F 1 "PWR_FLAG" H 4200 973 50  0000 C CNN
F 2 "" H 4200 800 50  0001 C CNN
F 3 "~" H 4200 800 50  0001 C CNN
	1    4200 800 
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 1000 4200 800 
$Comp
L power:PWR_FLAG #FLG05
U 1 1 61CFB1EE
P 8550 2000
F 0 "#FLG05" H 8550 2075 50  0001 C CNN
F 1 "PWR_FLAG" H 8550 2173 50  0000 C CNN
F 2 "" H 8550 2000 50  0001 C CNN
F 3 "~" H 8550 2000 50  0001 C CNN
	1    8550 2000
	1    0    0    -1  
$EndComp
Connection ~ 8550 2000
Wire Wire Line
	8550 2000 9800 2000
$Comp
L power:PWR_FLAG #FLG06
U 1 1 61CFB90A
P 8800 2350
F 0 "#FLG06" H 8800 2425 50  0001 C CNN
F 1 "PWR_FLAG" H 8800 2523 50  0000 C CNN
F 2 "" H 8800 2350 50  0001 C CNN
F 3 "~" H 8800 2350 50  0001 C CNN
	1    8800 2350
	1    0    0    -1  
$EndComp
Connection ~ 8800 2350
Wire Wire Line
	8800 2350 9100 2350
$Comp
L Connector:Conn_01x04_Male J1
U 1 1 61CFDBD2
P 950 5700
F 0 "J1" H 1058 5981 50  0000 C CNN
F 1 "Conn_01x04_Male" H 1058 5890 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Horizontal" H 950 5700 50  0001 C CNN
F 3 "~" H 950 5700 50  0001 C CNN
	1    950  5700
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR01
U 1 1 61CFEEF2
P 1400 5600
F 0 "#PWR01" H 1400 5450 50  0001 C CNN
F 1 "+5V" V 1415 5728 50  0000 L CNN
F 2 "" H 1400 5600 50  0001 C CNN
F 3 "" H 1400 5600 50  0001 C CNN
	1    1400 5600
	0    1    1    0   
$EndComp
Wire Wire Line
	1150 5600 1400 5600
NoConn ~ 1150 5800
$Comp
L power:GND #PWR02
U 1 1 61D028EB
P 1450 5900
F 0 "#PWR02" H 1450 5650 50  0001 C CNN
F 1 "GND" V 1455 5772 50  0000 R CNN
F 2 "" H 1450 5900 50  0001 C CNN
F 3 "" H 1450 5900 50  0001 C CNN
	1    1450 5900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1450 5900 1150 5900
Text Label 1450 5700 2    50   ~ 0
ARGB1
Wire Wire Line
	1450 5700 1150 5700
$Comp
L Connector:Conn_01x04_Male J2
U 1 1 61D0A9CB
P 1900 5700
F 0 "J2" H 2008 5981 50  0000 C CNN
F 1 "Conn_01x04_Male" H 2008 5890 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Horizontal" H 1900 5700 50  0001 C CNN
F 3 "~" H 1900 5700 50  0001 C CNN
	1    1900 5700
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR03
U 1 1 61D0A9D1
P 2350 5600
F 0 "#PWR03" H 2350 5450 50  0001 C CNN
F 1 "+5V" V 2365 5728 50  0000 L CNN
F 2 "" H 2350 5600 50  0001 C CNN
F 3 "" H 2350 5600 50  0001 C CNN
	1    2350 5600
	0    1    1    0   
$EndComp
Wire Wire Line
	2100 5600 2350 5600
NoConn ~ 2100 5800
$Comp
L power:GND #PWR04
U 1 1 61D0A9D9
P 2400 5900
F 0 "#PWR04" H 2400 5650 50  0001 C CNN
F 1 "GND" V 2405 5772 50  0000 R CNN
F 2 "" H 2400 5900 50  0001 C CNN
F 3 "" H 2400 5900 50  0001 C CNN
	1    2400 5900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2400 5900 2100 5900
Text Label 2400 5700 2    50   ~ 0
ARGB2
Wire Wire Line
	2400 5700 2100 5700
$Comp
L Connector:Conn_01x04_Male J3
U 1 1 61D0D3B9
P 2800 5700
F 0 "J3" H 2908 5981 50  0000 C CNN
F 1 "Conn_01x04_Male" H 2908 5890 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Horizontal" H 2800 5700 50  0001 C CNN
F 3 "~" H 2800 5700 50  0001 C CNN
	1    2800 5700
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR06
U 1 1 61D0D3BF
P 3250 5600
F 0 "#PWR06" H 3250 5450 50  0001 C CNN
F 1 "+5V" V 3265 5728 50  0000 L CNN
F 2 "" H 3250 5600 50  0001 C CNN
F 3 "" H 3250 5600 50  0001 C CNN
	1    3250 5600
	0    1    1    0   
$EndComp
Wire Wire Line
	3000 5600 3250 5600
NoConn ~ 3000 5800
$Comp
L power:GND #PWR07
U 1 1 61D0D3C7
P 3300 5900
F 0 "#PWR07" H 3300 5650 50  0001 C CNN
F 1 "GND" V 3305 5772 50  0000 R CNN
F 2 "" H 3300 5900 50  0001 C CNN
F 3 "" H 3300 5900 50  0001 C CNN
	1    3300 5900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	3300 5900 3000 5900
Text Label 3300 5700 2    50   ~ 0
ARGB3
Wire Wire Line
	3300 5700 3000 5700
$Comp
L power:+3.3V #PWR08
U 1 1 61D0F78E
P 3750 1100
F 0 "#PWR08" H 3750 950 50  0001 C CNN
F 1 "+3.3V" H 3765 1273 50  0000 C CNN
F 2 "" H 3750 1100 50  0001 C CNN
F 3 "" H 3750 1100 50  0001 C CNN
	1    3750 1100
	-1   0    0    1   
$EndComp
$Comp
L power:PWR_FLAG #FLG02
U 1 1 61D102B2
P 3750 850
F 0 "#FLG02" H 3750 925 50  0001 C CNN
F 1 "PWR_FLAG" H 3750 1023 50  0000 C CNN
F 2 "" H 3750 850 50  0001 C CNN
F 3 "~" H 3750 850 50  0001 C CNN
	1    3750 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 850  3750 1100
$Comp
L power:GND #PWR05
U 1 1 61D1300C
P 3250 1200
F 0 "#PWR05" H 3250 950 50  0001 C CNN
F 1 "GND" H 3255 1027 50  0000 C CNN
F 2 "" H 3250 1200 50  0001 C CNN
F 3 "" H 3250 1200 50  0001 C CNN
	1    3250 1200
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG01
U 1 1 61D13387
P 3250 1050
F 0 "#FLG01" H 3250 1125 50  0001 C CNN
F 1 "PWR_FLAG" H 3250 1223 50  0000 C CNN
F 2 "" H 3250 1050 50  0001 C CNN
F 3 "~" H 3250 1050 50  0001 C CNN
	1    3250 1050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 1050 3250 1200
$EndSCHEMATC
