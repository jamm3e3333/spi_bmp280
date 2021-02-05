--------------------------------------------------

0x88..0x8D - calibration registers for temperature
0xF7..0xF9 - data registers for temperature
0xFA..0xFD - data registers for pressure

--------------------------------------------------

connection
==================================================

RC2 - CS  - 0 (IF PIC IS MASTER)
RC3 - SCK - 0 (IF PIC IS MASTER)
RC4 - SDA - 1
RC5 - SDO - 0

0 - OUTPUT, 1 - INPUT

		    7 6 5 4 3 2 1 0
		    _ _ _ _ _ _ _ _
trisc = 0x10 -> 0 b x x 0 1_0 0 x x

CS LOW -> 0x20;
CS HIGH -> 0x00;

CKP = 1
CKE = 0


		      7 6 5 4 3 2 1 0
		      _ _ _ _ _ _ _ _
SSPSTAT = 0x80 -> 0 b 1 0 x x_x x x x

		      7 6 5 4 3 2 1 0
		      _ _ _ _ _ _ _ _
SSPCON = 0x30 ->  0 b x x 1 1_0 0 0 0

--------------------------------------------------

Data read
==================================================

30326 30326 30208
30326 30326 30208

7798543

--------------------------------------------------

65 - spi declaration
67 - spi declaration

--------------------------------------------------