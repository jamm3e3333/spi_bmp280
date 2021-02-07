﻿--------------------------------------------------

0x88..0x8D - calibration registers for temperature
0xF7..0xF9 - data registers for temperature
0xFA..0xFD - data registers for pressure

--------------------------------------------------

connection I2C
==================================================

RC2 - CS  - 0 (IF PIC IS MASTER)
RC3 - SCK - 1 (IF PIC IS MASTER)
RC4 - SDA - 1
RC5 - SDO - 0

0 - OUTPUT, 1 - INPUT

		    7 6 5 4 3 2 1 0
		    _ _ _ _ _ _ _ _
trisc = 0x18 -> 0 b x x 0 1_1 0 x x

CS LOW -> 0x20;
CS HIGH -> 0x00;

CKP = 1
CKE = 0


		      7 6 5 4 3 2 1 0
		      _ _ _ _ _ _ _ _
SSPSTAT = 0x00 -> 0 b 0 0 x x_x x x x

		      7 6 5 4 3 2 1 0
		      _ _ _ _ _ _ _ _
SSPCON = 0x20 ->  0 b x x 1 0_0 0 0 0

--------------------------------------------------

connection SPI
==================================================

RC2 - CS  - 0 (IF PIC IS MASTER)
RC3 - SCK - 0 (IF PIC IS MASTER)
RC4 - SDA - 1
RC5 - SDO - 0

0 - OUTPUT, 1 - INPUT

		    7 6 5 4 3 2 1 0
		    _ _ _ _ _ _ _ _
trisc = 0x20 -> 0 b x x 1 0_0 0 x x

CS LOW -> 0x04;
CS HIGH -> 0x00;

CKP = 1
CKE = 0


		      7 6 5 4 3 2 1 0
		      _ _ _ _ _ _ _ _
SSPSTAT = 0x00 -> 0 b 0 0 x x_x x x x

		      7 6 5 4 3 2 1 0
		      _ _ _ _ _ _ _ _
SSPCON = 0x20 ->  0 b x x 1 0_0 0 0 0

--------------------------------------------------

ctrl register 0xF4
==================================================

		      7 6 5 4 3 2 1 0
		      _ _ _ _ _ _ _ _
CTRL = 0x57  ->   0 b 0 1 0 1_0 1 1 1

==================================================

mode[1:0] = 1 1 -> normal mode

osrs_p[4:2] = 1 0 1 -> 20 bit = 0.16 Pa

osrs_t[7:5] = 0 1 0 -> 17 bit = 0.0025 °C

==================================================

cfg register 0xF5
==================================================

		      7 6 5 4 3 2 1 0
		      _ _ _ _ _ _ _ _
CFG = 0x80   ->   0 b 1 0 0 0_0 0 0 0

==================================================

spi_ew[1:0] = 0 0 -> spi 4 pin

filter[4:2] = 0 0 0 -> off

t_sb[7:5] = 1 0 0 -> 500 ms

--------------------------------------------------

function for calculating the temperature
==================================================

int32_t vypocet_temp(int32_t temperature){
 int32_t var1, var2, T, t_fine;
 var1 = ((((temperature/8) - ((int32_t)temp16_1*2)))*((int32_t)temp16_2[0]))/2048;
 var2 = (((((temperature/16)-((int32_t)temp16_1))*((temperature/16)-((int32_t)temp16_1)))/4096)*((int32_t)temp16_2[1]))/16384;
 t_fine = var1 + var2;
 T = ((t_fine*5)+128) /256;
 return T;
}

--------------------------------------------------

Data read
==================================================

30326 30326 30208
30326 30326 30208

7798543

--------------------------------------------------

PIC manual
==================================================

65 - spi declaration
67 - spi declaration

--------------------------------------------------