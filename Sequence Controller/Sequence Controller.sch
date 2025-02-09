EESchema Schematic File Version 4
EELAYER 26 0
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
L Esp32~Devkit:ESP32Devkit30 U1
U 1 1 5D3799CF
P 4700 3250
F 0 "U1" H 4700 4215 50  0000 C CNN
F 1 "ESP32Devkit30" H 4700 4124 50  0000 C CNN
F 2 "Sequence Controller:Esp32_30_Pin" H 4150 3700 50  0001 C CNN
F 3 "" H 4150 3700 50  0001 C CNN
	1    4700 3250
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:BSS84 Q1
U 1 1 5D379B74
P 6400 2250
F 0 "Q1" H 6606 2296 50  0000 L CNN
F 1 "BSR202N" H 6606 2205 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 6600 2175 50  0001 L CIN
F 3 "http://assets.nexperia.com/documents/data-sheet/BSS84.pdf" H 6400 2250 50  0001 L CNN
	1    6400 2250
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:BSS84 Q5
U 1 1 5D37A966
P 7800 3450
F 0 "Q5" H 8006 3496 50  0000 L CNN
F 1 "BSR202N" H 8006 3405 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 8000 3375 50  0001 L CIN
F 3 "http://assets.nexperia.com/documents/data-sheet/BSS84.pdf" H 7800 3450 50  0001 L CNN
	1    7800 3450
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:BSS84 Q3
U 1 1 5D37A98E
P 7100 2850
F 0 "Q3" H 7306 2896 50  0000 L CNN
F 1 "BSR202N" H 7306 2805 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 7300 2775 50  0001 L CIN
F 3 "http://assets.nexperia.com/documents/data-sheet/BSS84.pdf" H 7100 2850 50  0001 L CNN
	1    7100 2850
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:BSS84 Q8
U 1 1 5D37A9AA
P 8850 4350
F 0 "Q8" H 9056 4396 50  0000 L CNN
F 1 "BSR202N" H 9056 4305 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 9050 4275 50  0001 L CIN
F 3 "http://assets.nexperia.com/documents/data-sheet/BSS84.pdf" H 8850 4350 50  0001 L CNN
	1    8850 4350
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:BSS84 Q7
U 1 1 5D37A9C6
P 8500 4050
F 0 "Q7" H 8706 4096 50  0000 L CNN
F 1 "BSR202N" H 8706 4005 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 8700 3975 50  0001 L CIN
F 3 "http://assets.nexperia.com/documents/data-sheet/BSS84.pdf" H 8500 4050 50  0001 L CNN
	1    8500 4050
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:BSS84 Q6
U 1 1 5D37A9E4
P 8150 3750
F 0 "Q6" H 8356 3796 50  0000 L CNN
F 1 "BSR202N" H 8356 3705 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 8350 3675 50  0001 L CIN
F 3 "http://assets.nexperia.com/documents/data-sheet/BSS84.pdf" H 8150 3750 50  0001 L CNN
	1    8150 3750
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:BSS84 Q4
U 1 1 5D37AA08
P 7450 3150
F 0 "Q4" H 7656 3196 50  0000 L CNN
F 1 "BSR202N" H 7656 3105 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 7650 3075 50  0001 L CIN
F 3 "http://assets.nexperia.com/documents/data-sheet/BSS84.pdf" H 7450 3150 50  0001 L CNN
	1    7450 3150
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:BSS84 Q2
U 1 1 5D37AA2E
P 6750 2550
F 0 "Q2" H 6956 2596 50  0000 L CNN
F 1 "BSR202N" H 6956 2505 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD:SOT-23" H 6950 2475 50  0001 L CIN
F 3 "http://assets.nexperia.com/documents/data-sheet/BSS84.pdf" H 6750 2550 50  0001 L CNN
	1    6750 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 2950 6400 2850
Wire Wire Line
	6400 2850 6900 2850
Wire Wire Line
	6250 4350 8650 4350
Wire Wire Line
	8300 4050 6350 4050
Wire Wire Line
	7950 3750 6450 3750
Wire Wire Line
	6450 3750 6450 3250
Wire Wire Line
	6550 3150 6550 3450
Wire Wire Line
	6550 3450 7600 3450
Wire Wire Line
	6650 3150 6650 3050
Wire Wire Line
	6650 3150 7250 3150
Wire Wire Line
	6850 3050 7200 3050
Wire Wire Line
	6850 2750 6850 3050
Wire Wire Line
	7200 3050 7200 3350
Wire Wire Line
	7200 3350 7550 3350
Connection ~ 7200 3050
Wire Wire Line
	7550 3350 7550 3650
Wire Wire Line
	7550 3650 7900 3650
Connection ~ 7550 3350
Wire Wire Line
	7900 3650 7900 3950
Wire Wire Line
	7900 3950 8250 3950
Connection ~ 7900 3650
Wire Wire Line
	8250 3950 8250 4250
Wire Wire Line
	8250 4250 8600 4250
Connection ~ 8250 3950
Wire Wire Line
	8600 4250 8600 4550
Wire Wire Line
	8600 4550 8950 4550
Connection ~ 8600 4250
Wire Wire Line
	6500 2750 6850 2750
Wire Wire Line
	6500 2450 6500 2750
Connection ~ 6850 2750
Wire Wire Line
	8950 4550 9600 4550
Connection ~ 8950 4550
Wire Wire Line
	8950 4150 9400 4150
Wire Wire Line
	8600 3850 9250 3850
Wire Wire Line
	8250 3550 9100 3550
Wire Wire Line
	7900 3250 9050 3250
Wire Wire Line
	6500 2050 9800 2050
Wire Wire Line
	9800 2050 9800 2750
Wire Wire Line
	9800 2750 9900 2750
Wire Wire Line
	6850 2350 9700 2350
Wire Wire Line
	9700 2350 9700 2850
Wire Wire Line
	9700 2850 9900 2850
Wire Wire Line
	7200 2650 9600 2650
Wire Wire Line
	9600 2650 9600 2950
Wire Wire Line
	7550 2950 9500 2950
Wire Wire Line
	9500 2950 9500 3050
Wire Wire Line
	9500 3050 9900 3050
$Comp
L Connector:Conn_01x09_Male ExtOut1
U 1 1 5D384C63
P 2700 3250
F 0 "ExtOut1" H 2806 3828 50  0000 C CNN
F 1 "Conn_01x09_Male" H 2806 3737 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x09_Pitch2.54mm" H 2700 3250 50  0001 C CNN
F 3 "~" H 2700 3250 50  0001 C CNN
	1    2700 3250
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x09_Male ExtIn1
U 1 1 5D384CB8
P 5700 3150
F 0 "ExtIn1" H 5806 3728 50  0000 C CNN
F 1 "Conn_01x09_Male" H 5806 3637 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x09_Pitch2.54mm" H 5700 3150 50  0001 C CNN
F 3 "~" H 5700 3150 50  0001 C CNN
	1    5700 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 2550 5900 2550
Wire Wire Line
	5500 2950 5900 2950
Connection ~ 5900 2850
Wire Wire Line
	5500 3050 5900 3050
Connection ~ 5900 2950
Wire Wire Line
	5500 3150 5900 3150
Connection ~ 5900 3050
Wire Wire Line
	5500 3250 5900 3250
Connection ~ 5900 3150
Wire Wire Line
	5500 3550 5550 3550
Wire Wire Line
	5550 3550 5550 3350
Wire Wire Line
	5550 3350 5900 3350
Connection ~ 5900 3250
Wire Wire Line
	5500 3650 5650 3650
Wire Wire Line
	5650 3650 5650 3450
Wire Wire Line
	5650 3450 5900 3450
Connection ~ 5900 3350
Wire Wire Line
	5500 2650 5600 2650
Wire Wire Line
	5600 2650 5600 2850
Wire Wire Line
	5600 2850 5900 2850
Wire Wire Line
	5900 2950 6400 2950
Wire Wire Line
	5900 3150 6550 3150
Wire Wire Line
	5900 3050 6650 3050
Wire Wire Line
	5900 3250 6450 3250
Wire Wire Line
	6350 3350 6350 4050
Wire Wire Line
	5900 3350 6350 3350
Wire Wire Line
	6250 3450 6250 4350
Wire Wire Line
	5900 3450 6250 3450
Wire Wire Line
	6300 2850 6300 2550
Wire Wire Line
	6300 2550 6550 2550
Wire Wire Line
	5900 2850 6300 2850
Wire Wire Line
	6200 2750 6200 2250
Wire Wire Line
	5900 2750 6200 2750
Connection ~ 5900 3450
Wire Wire Line
	5900 2550 5900 2750
Connection ~ 5900 2750
Wire Wire Line
	5900 3550 5900 3850
Wire Wire Line
	5900 3850 5500 3850
Wire Wire Line
	3900 3550 2900 3550
Wire Wire Line
	2900 3450 3900 3450
Wire Wire Line
	3900 3350 2900 3350
Wire Wire Line
	2900 3250 3900 3250
Wire Wire Line
	2900 3050 3900 3050
Wire Wire Line
	2900 3150 3900 3150
Wire Wire Line
	2900 2950 3350 2950
Wire Wire Line
	3350 2950 3350 3650
Wire Wire Line
	3350 3650 3900 3650
Wire Wire Line
	2900 2850 3450 2850
Wire Wire Line
	3450 2850 3450 3750
Wire Wire Line
	3450 3750 3900 3750
Wire Wire Line
	2900 3650 3050 3650
Wire Wire Line
	3050 3650 3050 3850
Wire Wire Line
	3050 3850 3900 3850
Wire Wire Line
	8600 4550 5900 4550
Wire Wire Line
	5900 4550 5900 3850
Connection ~ 8600 4550
Connection ~ 5900 3850
Wire Wire Line
	3050 4550 3050 3850
Connection ~ 5900 4550
Connection ~ 3050 3850
Text Label 9600 4550 0    50   ~ 0
GND
$Comp
L Connector:Conn_01x11_Male J1
U 1 1 5D6D343D
P 10100 2950
F 0 "J1" H 10073 2973 50  0000 R CNN
F 1 "Conn_01x11_Male" H 10073 2882 50  0000 R CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x11_Pitch2.54mm" H 10100 2950 50  0001 C CNN
F 3 "~" H 10100 2950 50  0001 C CNN
	1    10100 2950
	-1   0    0    -1  
$EndComp
Wire Wire Line
	9600 2950 9900 2950
Wire Wire Line
	9050 3150 9050 3250
Wire Wire Line
	9050 3150 9900 3150
Wire Wire Line
	9100 3250 9100 3550
Wire Wire Line
	9100 3250 9900 3250
Wire Wire Line
	9250 3350 9250 3850
Wire Wire Line
	9250 3350 9900 3350
Wire Wire Line
	9400 3450 9400 4150
Wire Wire Line
	9400 3450 9900 3450
Wire Wire Line
	9600 3650 9600 4550
Wire Wire Line
	3900 4800 9800 4800
Wire Wire Line
	9800 4800 9800 3750
Wire Wire Line
	3050 4550 5900 4550
Wire Wire Line
	3900 3950 3900 4800
Wire Wire Line
	10350 3650 10350 2650
Wire Wire Line
	9600 3650 10350 3650
Wire Wire Line
	9900 2650 9900 2550
Connection ~ 9900 2650
Wire Wire Line
	10600 3750 10600 2450
Wire Wire Line
	10600 2450 9900 2450
Wire Wire Line
	9800 3750 10600 3750
Wire Wire Line
	9900 2650 10350 2650
$EndSCHEMATC
