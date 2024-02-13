EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
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
L Connector:Conn_01x04_Male J?
U 1 1 61C7F3E6
P 2550 2600
AR Path="/61C7F3E6" Ref="J?"  Part="1" 
AR Path="/61C7C7AC/61C7F3E6" Ref="J?"  Part="1" 
F 0 "J?" H 2658 2881 50  0000 C CNN
F 1 "Conn_01x04_Male" H 2658 2790 50  0000 C CNN
F 2 "" H 2550 2600 50  0001 C CNN
F 3 "~" H 2550 2600 50  0001 C CNN
	1    2550 2600
	1    0    0    -1  
$EndComp
Text Label 3250 2700 2    50   ~ 0
RPM_IN
Text Label 3250 2800 2    50   ~ 0
PWM_OUT
$Comp
L power:GND #PWR?
U 1 1 61C7F3EE
P 3200 2500
AR Path="/61C7F3EE" Ref="#PWR?"  Part="1" 
AR Path="/61C7C7AC/61C7F3EE" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3200 2250 50  0001 C CNN
F 1 "GND" V 3205 2372 50  0000 R CNN
F 2 "" H 3200 2500 50  0001 C CNN
F 3 "" H 3200 2500 50  0001 C CNN
	1    3200 2500
	0    -1   -1   0   
$EndComp
$Comp
L power:+12V #PWR?
U 1 1 61C7F3F4
P 3200 2600
AR Path="/61C7F3F4" Ref="#PWR?"  Part="1" 
AR Path="/61C7C7AC/61C7F3F4" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3200 2450 50  0001 C CNN
F 1 "+12V" V 3215 2728 50  0000 L CNN
F 2 "" H 3200 2600 50  0001 C CNN
F 3 "" H 3200 2600 50  0001 C CNN
	1    3200 2600
	0    1    1    0   
$EndComp
Wire Wire Line
	3200 2600 2750 2600
Wire Wire Line
	3250 2700 2750 2700
Wire Wire Line
	3250 2800 2750 2800
Wire Wire Line
	2750 2500 3200 2500
$Comp
L Connector:Conn_01x04_Male J?
U 1 1 61C80D4D
P 2500 3200
AR Path="/61C80D4D" Ref="J?"  Part="1" 
AR Path="/61C7C7AC/61C80D4D" Ref="J?"  Part="1" 
F 0 "J?" H 2608 3481 50  0000 C CNN
F 1 "Conn_01x04_Male" H 2608 3390 50  0000 C CNN
F 2 "" H 2500 3200 50  0001 C CNN
F 3 "~" H 2500 3200 50  0001 C CNN
	1    2500 3200
	1    0    0    -1  
$EndComp
Text Label 3200 3400 2    50   ~ 0
PWM_OUT
$Comp
L power:GND #PWR?
U 1 1 61C80D55
P 3150 3100
AR Path="/61C80D55" Ref="#PWR?"  Part="1" 
AR Path="/61C7C7AC/61C80D55" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3150 2850 50  0001 C CNN
F 1 "GND" V 3155 2972 50  0000 R CNN
F 2 "" H 3150 3100 50  0001 C CNN
F 3 "" H 3150 3100 50  0001 C CNN
	1    3150 3100
	0    -1   -1   0   
$EndComp
$Comp
L power:+12V #PWR?
U 1 1 61C80D5B
P 3150 3200
AR Path="/61C80D5B" Ref="#PWR?"  Part="1" 
AR Path="/61C7C7AC/61C80D5B" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3150 3050 50  0001 C CNN
F 1 "+12V" V 3165 3328 50  0000 L CNN
F 2 "" H 3150 3200 50  0001 C CNN
F 3 "" H 3150 3200 50  0001 C CNN
	1    3150 3200
	0    1    1    0   
$EndComp
Wire Wire Line
	3150 3200 2700 3200
Wire Wire Line
	3200 3400 2700 3400
Wire Wire Line
	2700 3100 3150 3100
NoConn ~ 2700 3300
$Comp
L Connector:Conn_01x04_Male J?
U 1 1 61C842EE
P 2500 3800
AR Path="/61C842EE" Ref="J?"  Part="1" 
AR Path="/61C7C7AC/61C842EE" Ref="J?"  Part="1" 
F 0 "J?" H 2608 4081 50  0000 C CNN
F 1 "Conn_01x04_Male" H 2608 3990 50  0000 C CNN
F 2 "" H 2500 3800 50  0001 C CNN
F 3 "~" H 2500 3800 50  0001 C CNN
	1    2500 3800
	1    0    0    -1  
$EndComp
Text Label 3200 4000 2    50   ~ 0
PWM_OUT
$Comp
L power:GND #PWR?
U 1 1 61C842F5
P 3150 3700
AR Path="/61C842F5" Ref="#PWR?"  Part="1" 
AR Path="/61C7C7AC/61C842F5" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3150 3450 50  0001 C CNN
F 1 "GND" V 3155 3572 50  0000 R CNN
F 2 "" H 3150 3700 50  0001 C CNN
F 3 "" H 3150 3700 50  0001 C CNN
	1    3150 3700
	0    -1   -1   0   
$EndComp
$Comp
L power:+12V #PWR?
U 1 1 61C842FB
P 3150 3800
AR Path="/61C842FB" Ref="#PWR?"  Part="1" 
AR Path="/61C7C7AC/61C842FB" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 3150 3650 50  0001 C CNN
F 1 "+12V" V 3165 3928 50  0000 L CNN
F 2 "" H 3150 3800 50  0001 C CNN
F 3 "" H 3150 3800 50  0001 C CNN
	1    3150 3800
	0    1    1    0   
$EndComp
Wire Wire Line
	3150 3800 2700 3800
Wire Wire Line
	3200 4000 2700 4000
Wire Wire Line
	2700 3700 3150 3700
NoConn ~ 2700 3900
Text HLabel 4350 3450 0    50   Input ~ 0
PWM_OUT
Text HLabel 4350 3550 0    50   Input ~ 0
RPM_IN
Text Label 4700 3550 2    50   ~ 0
RPM_IN
Text Label 4750 3450 2    50   ~ 0
PWM_OUT
Wire Wire Line
	4750 3450 4350 3450
Wire Wire Line
	4700 3550 4350 3550
$EndSCHEMATC
