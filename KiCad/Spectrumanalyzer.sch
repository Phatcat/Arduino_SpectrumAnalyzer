EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L Switch:SW_Push SW3
U 1 1 601BFDAE
P 5700 5100
F 0 "SW3" H 5800 5200 50  0000 C CNN
F 1 "SW_Push" H 5700 5294 50  0001 C CNN
F 2 "" H 5700 5300 50  0001 C CNN
F 3 "~" H 5700 5300 50  0001 C CNN
	1    5700 5100
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW1
U 1 1 601C072E
P 5700 4400
F 0 "SW1" H 5800 4500 50  0000 C CNN
F 1 "SW_Push" H 5900 4500 50  0001 C CNN
F 2 "" H 5700 4600 50  0001 C CNN
F 3 "~" H 5700 4600 50  0001 C CNN
	1    5700 4400
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW2
U 1 1 601C16DD
P 5700 4750
F 0 "SW2" H 5800 4850 50  0000 C CNN
F 1 "SW_Push" H 5900 4850 50  0001 C CNN
F 2 "" H 5700 4950 50  0001 C CNN
F 3 "~" H 5700 4950 50  0001 C CNN
	1    5700 4750
	1    0    0    -1  
$EndComp
$Comp
L power:+9V #PWR?
U 1 1 602DA37A
P 3900 3100
F 0 "#PWR?" H 3900 2950 50  0001 C CNN
F 1 "+9V" H 3915 3273 50  0000 C CNN
F 2 "" H 3900 3100 50  0001 C CNN
F 3 "" H 3900 3100 50  0001 C CNN
	1    3900 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 5300 7300 5300
Wire Wire Line
	4200 3100 3900 3100
Wire Wire Line
	5500 4400 5500 4600
$Comp
L Device:CP_Small C2
U 1 1 60992235
P 5900 4500
F 0 "C2" H 5988 4546 50  0000 L CNN
F 1 "1uF" H 5988 4455 50  0000 L CNN
F 2 "" H 5900 4500 50  0001 C CNN
F 3 "~" H 5900 4500 50  0001 C CNN
	1    5900 4500
	1    0    0    -1  
$EndComp
$Comp
L Device:CP_Small C3
U 1 1 60993204
P 5900 4850
F 0 "C3" H 5988 4896 50  0000 L CNN
F 1 "1uF" H 5988 4805 50  0000 L CNN
F 2 "" H 5900 4850 50  0001 C CNN
F 3 "~" H 5900 4850 50  0001 C CNN
	1    5900 4850
	1    0    0    -1  
$EndComp
$Comp
L Device:CP_Small C4
U 1 1 60994595
P 5900 5200
F 0 "C4" H 5988 5246 50  0000 L CNN
F 1 "1uF" H 5988 5155 50  0000 L CNN
F 2 "" H 5900 5200 50  0001 C CNN
F 3 "~" H 5900 5200 50  0001 C CNN
	1    5900 5200
	1    0    0    -1  
$EndComp
Connection ~ 5900 5100
Connection ~ 5500 4600
Wire Wire Line
	5900 5300 5500 5300
Wire Wire Line
	5500 4600 5500 4750
Connection ~ 5500 4750
Wire Wire Line
	5500 4750 5500 4950
Connection ~ 5500 5100
Wire Wire Line
	5500 5100 5500 5300
Connection ~ 5900 4750
Connection ~ 5900 4400
Wire Wire Line
	5900 4600 5500 4600
Wire Wire Line
	5900 4950 5500 4950
Connection ~ 5500 4950
Wire Wire Line
	5500 4950 5500 5100
Wire Wire Line
	5500 3250 5500 3600
Connection ~ 5100 3250
Wire Wire Line
	5500 4400 5500 4200
Wire Wire Line
	5300 4250 5900 4250
Wire Wire Line
	5300 4250 5300 4000
Wire Wire Line
	5900 3900 5900 4250
Wire Wire Line
	5100 3250 3900 3250
Wire Wire Line
	5100 3400 5100 3250
Wire Wire Line
	5500 3250 5100 3250
Wire Wire Line
	3900 3350 3900 3250
Wire Wire Line
	3900 3700 3900 3650
Connection ~ 3900 3700
Wire Wire Line
	4200 3700 3900 3700
Wire Wire Line
	3900 4000 3900 3700
Wire Wire Line
	3900 4400 4750 4400
Wire Wire Line
	5100 4400 5500 4400
Connection ~ 5100 4400
Wire Wire Line
	5100 4200 5100 4400
Connection ~ 5100 3800
Wire Wire Line
	5100 3900 5100 3800
Wire Wire Line
	5100 3800 5100 3700
Wire Wire Line
	5300 3800 5100 3800
Wire Wire Line
	4750 4400 5100 4400
Connection ~ 4750 4400
Text GLabel 4200 3900 0    50   Input ~ 0
LineIn
$Comp
L Device:R_US R3
U 1 1 602A4564
P 5100 4050
F 0 "R3" H 5168 4096 50  0000 L CNN
F 1 "1M" H 5168 4005 50  0000 L CNN
F 2 "" V 5140 4040 50  0001 C CNN
F 3 "~" H 5100 4050 50  0001 C CNN
	1    5100 4050
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R2
U 1 1 602A2F86
P 5100 3550
F 0 "R2" H 5168 3596 50  0000 L CNN
F 1 "1M" H 5168 3505 50  0000 L CNN
F 2 "" V 5140 3540 50  0001 C CNN
F 3 "~" H 5100 3550 50  0001 C CNN
	1    5100 3550
	1    0    0    -1  
$EndComp
$Comp
L Connector:XLR3_Switched J?
U 2 1 60254135
P 4500 3800
F 0 "J?" H 4500 4142 50  0000 C CNN
F 1 "XLR3_Switched" H 4500 4051 50  0000 C CNN
F 2 "" H 4500 3900 50  0001 C CNN
F 3 " ~" H 4500 3900 50  0001 C CNN
	2    4500 3800
	-1   0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 60270033
P 4950 3800
F 0 "C1" V 4698 3800 50  0000 C CNN
F 1 "100u" V 4789 3800 50  0000 C CNN
F 2 "" H 4988 3650 50  0001 C CNN
F 3 "~" H 4950 3800 50  0001 C CNN
	1    4950 3800
	0    1    1    0   
$EndComp
$Comp
L Device:R_US R1
U 1 1 6024F135
P 3900 3500
F 0 "R1" H 3700 3550 50  0000 L CNN
F 1 "4.7k" H 3650 3450 50  0000 L CNN
F 2 "" V 3940 3490 50  0001 C CNN
F 3 "~" H 3900 3500 50  0001 C CNN
	1    3900 3500
	1    0    0    -1  
$EndComp
$Comp
L Device:Microphone MK1
U 1 1 6024C928
P 3900 4200
F 0 "MK1" V 3750 4200 50  0000 L CNN
F 1 "Microphone" V 3650 4000 50  0000 L CNN
F 2 "" V 3900 4300 50  0001 C CNN
F 3 "~" V 3900 4300 50  0001 C CNN
	1    3900 4200
	-1   0    0    1   
$EndComp
$Comp
L power:GNDREF #PWR?
U 1 1 601DCC77
P 4750 4400
F 0 "#PWR?" H 4750 4150 50  0001 C CNN
F 1 "GNDREF" H 4755 4227 50  0000 C CNN
F 2 "" H 4750 4400 50  0001 C CNN
F 3 "" H 4750 4400 50  0001 C CNN
	1    4750 4400
	1    0    0    -1  
$EndComp
Connection ~ 5500 4400
Wire Wire Line
	5900 5300 6350 5300
Connection ~ 5900 5300
Wire Wire Line
	5900 5100 6350 5100
Wire Wire Line
	5900 4400 6350 4400
Connection ~ 5500 3250
Wire Wire Line
	6350 3900 5900 3900
Wire Wire Line
	5900 4750 6350 4750
Text GLabel 7300 4700 2    50   Input ~ 0
A0
Text GLabel 4200 3100 2    50   Input ~ 0
DCIN
Text GLabel 7300 5300 2    50   Input ~ 0
5V
Text GLabel 6350 3100 2    50   Input ~ 0
AREF
Text GLabel 6350 3250 2    50   Input ~ 0
3.3V
Text GLabel 6350 3900 2    50   Input ~ 0
A6
Text GLabel 6350 4400 2    50   Input ~ 0
D19
Text GLabel 6350 4750 2    50   Input ~ 0
D20
Text GLabel 6350 5100 2    50   Input ~ 0
D21
Text GLabel 6350 5300 2    50   Input ~ 0
GND
Text GLabel 7300 4800 2    50   Input ~ 0
A1
Text GLabel 7300 4900 2    50   Input ~ 0
A2
Text GLabel 7300 5000 2    50   Input ~ 0
A3
Text GLabel 7300 5100 2    50   Input ~ 0
A4
Text GLabel 7250 4500 2    50   Input ~ 0
D13
Text GLabel 7250 4400 2    50   Input ~ 0
D12
Text GLabel 7250 4300 2    50   Input ~ 0
D11
Text GLabel 7250 4200 2    50   Input ~ 0
D10
Text GLabel 7300 4100 2    50   Input ~ 0
D9
Text GLabel 7300 4000 2    50   Input ~ 0
D8
Text GLabel 7300 3900 2    50   Input ~ 0
D7
Text GLabel 7300 3800 2    50   Input ~ 0
D6
Text GLabel 7300 3700 2    50   Input ~ 0
D5
Text GLabel 7300 3600 2    50   Input ~ 0
D4
Text GLabel 7300 3500 2    50   Input ~ 0
D3
Text GLabel 7300 3400 2    50   Input ~ 0
D2
$Comp
L Amplifier_Operational:OPA2340 U1
U 1 1 60992073
P 5600 3900
F 0 "U1" H 5700 4050 50  0000 C CNN
F 1 "OPA2340" H 5700 3700 50  0000 C CNN
F 2 "" H 5600 3900 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/opa4340.pdf" H 5600 3900 50  0001 C CNN
	1    5600 3900
	1    0    0    -1  
$EndComp
$Comp
L Amplifier_Operational:OPA2340 U?
U 3 1 6099E5CB
P 5600 3900
F 0 "U?" H 5558 3900 50  0001 L CNN
F 1 "OPA2340" H 5558 3855 50  0001 L CNN
F 2 "" H 5600 3900 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/opa4340.pdf" H 5600 3900 50  0001 C CNN
	3    5600 3900
	1    0    0    -1  
$EndComp
Connection ~ 5900 3900
Text GLabel 7000 3400 0    50   Input ~ 0
LCD_02
Text GLabel 7000 3500 0    50   Input ~ 0
LCD_03
Text GLabel 7000 3600 0    50   Input ~ 0
LCD_04
Text GLabel 7000 3700 0    50   Input ~ 0
LCD_05
Text GLabel 7000 3800 0    50   Input ~ 0
LCD_06
Text GLabel 7000 3900 0    50   Input ~ 0
LCD_07
Text GLabel 7000 4000 0    50   Input ~ 0
LCD_00
Text GLabel 7000 4100 0    50   Input ~ 0
LCD_01
Text GLabel 7000 4200 0    50   Input ~ 0
SD_SS
Text GLabel 7000 4300 0    50   Input ~ 0
SD_D1
Text GLabel 7000 4400 0    50   Input ~ 0
SD_D0
Text GLabel 7000 4500 0    50   Input ~ 0
SD_SCK
Wire Wire Line
	7000 3400 7300 3400
Wire Wire Line
	7300 3500 7000 3500
Wire Wire Line
	7000 3600 7300 3600
Wire Wire Line
	7300 3700 7000 3700
Wire Wire Line
	7000 3800 7300 3800
Wire Wire Line
	7300 3900 7000 3900
Wire Wire Line
	7000 4000 7300 4000
Wire Wire Line
	7300 4100 7000 4100
Wire Wire Line
	7000 4200 7250 4200
Wire Wire Line
	7250 4300 7000 4300
Wire Wire Line
	7250 4400 7000 4400
Wire Wire Line
	7250 4500 7000 4500
Text GLabel 6250 3100 0    50   Input ~ 0
3.3V
Text GLabel 7000 5300 0    50   Input ~ 0
5V
Text GLabel 7150 3200 2    50   Input ~ 0
Arduino
Text GLabel 7100 3200 0    50   Input ~ 0
TFT_Display
Text GLabel 7000 4700 0    50   Input ~ 0
LCD_RD
Text GLabel 7000 4800 0    50   Input ~ 0
LCD_WR
Text GLabel 7000 4900 0    50   Input ~ 0
LCD_RS
Text GLabel 7000 5000 0    50   Input ~ 0
LCD_CS
Text GLabel 7000 5100 0    50   Input ~ 0
LCD_RST
Wire Wire Line
	7000 4700 7300 4700
Wire Wire Line
	7000 4800 7300 4800
Wire Wire Line
	7000 4900 7300 4900
Wire Wire Line
	7000 5000 7300 5000
Wire Wire Line
	7000 5100 7300 5100
Wire Wire Line
	7150 3200 7100 3200
Wire Wire Line
	6250 3100 6250 3250
Wire Wire Line
	6250 3100 6350 3100
Connection ~ 6250 3250
Wire Wire Line
	6250 3250 6350 3250
Wire Wire Line
	5500 3250 6250 3250
$EndSCHEMATC
