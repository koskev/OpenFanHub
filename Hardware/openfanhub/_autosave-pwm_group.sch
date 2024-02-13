EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 12 17
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 2100 2050 0    50   Input ~ 0
PWM
Text HLabel 2100 2150 0    50   Output ~ 0
RPM
Text Label 2450 2150 2    50   ~ 0
RPM
Text Label 2500 2050 2    50   ~ 0
PWM
Wire Wire Line
	2500 2050 2100 2050
Wire Wire Line
	2450 2150 2100 2150
$Sheet
S 1850 1000 850  650 
U 61CA67D4
F0 "Subfan 1" 50
F1 "fan_connector.sch" 50
F2 "PWM_IN" I L 1850 1150 50 
F3 "RPM_OUT" O L 1850 1400 50 
F4 "PWM_SWITCH" I L 1850 1550 50 
$EndSheet
$Sheet
S 3250 1000 850  650 
U 61CA6CD1
F0 "Subfan 2" 50
F1 "fan_connector.sch" 50
F2 "PWM_IN" I L 3250 1150 50 
F3 "RPM_OUT" O L 3250 1400 50 
F4 "PWM_SWITCH" I L 3250 1550 50 
$EndSheet
$Sheet
S 4750 1000 850  650 
U 61CA6ECF
F0 "Subfan 3" 50
F1 "fan_connector.sch" 50
F2 "PWM_IN" I L 4750 1150 50 
F3 "RPM_OUT" O L 4750 1400 50 
F4 "PWM_SWITCH" I L 4750 1550 50 
$EndSheet
Text Label 1450 1150 0    50   ~ 0
PWM
Wire Wire Line
	1450 1150 1850 1150
Text Label 1450 1400 0    50   ~ 0
RPM
Wire Wire Line
	1450 1400 1850 1400
Wire Wire Line
	2950 1150 3250 1150
Text Label 4400 1150 0    50   ~ 0
PWM
Wire Wire Line
	4400 1150 4750 1150
Text Label 1350 1550 0    50   ~ 0
PWM_SWITCH
Wire Wire Line
	1350 1550 1850 1550
Text Label 2750 1550 0    50   ~ 0
PWM_SWITCH
Text Label 2950 1150 0    50   ~ 0
PWM
Wire Wire Line
	2750 1550 3250 1550
Text Label 4250 1550 0    50   ~ 0
PWM_SWITCH
Wire Wire Line
	4250 1550 4750 1550
Text HLabel 2050 2250 0    50   Input ~ 0
PWM_SWITCH
Text Label 2550 2250 2    50   ~ 0
PWM_SWITCH
Wire Wire Line
	2550 2250 2050 2250
NoConn ~ 3250 1400
NoConn ~ 4750 1400
$EndSCHEMATC
