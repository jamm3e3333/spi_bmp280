#include <stdint.h>
#define  NULL                  0
#define  IN_OUT_TERMINATOR     "~"//126
#define  VELIKOST_BUFFER       10

int32_t tepl, tlak;
float temp_f;

unsigned char temp_b[12];

uint16_t temp[3];
uint16_t press[3];

uint16_t temp16_1;
int16_t temp16_2[2];
uint16_t press16_1;
int16_t press16_2[8];

int32_t temperature, t_fine;

// Ukazatele na pole
char* pInPole =  NULL, pOutPole =  NULL;
// Vstupni/Vystupni buffer
char InPole[VELIKOST_BUFFER];
char OutPole[VELIKOST_BUFFER];

char Zarizeni, Adresa, Sta, Des, Jed, CRC;
unsigned char buffer;
uint8_t status_reg;

/*******************************************************/
void init(void)
     {
     SSPSTAT = 0x00;
     SSPCON = 0x20;
     //nastaveni vstupu a vystuu
     trisc = 0x10;
     trisb = 0x00;
     portb = 0x01;

     pInPole  = &InPole[0];
     pOutPole = &OutPole[0];
     //inicializace uart komunikace
     UART1_Init(9600);
     SPI1_Init();
     delay_ms(100);
     //soft reset
     portb = 0x00;
     SPI1_Write(0xE0);
     SPI1_Write(0xB6);
     portb = 0x01;
     delay_ms(100);
     //cekani na nacteni dat z NVM ro registru
     do{
        portb = 0x00;
        SPI1_Read(0xF3);
        status_reg = SPI1_Read(buffer);
        portb = 0x01;
       delay_ms(100);
     }while((status_reg & 0x01) == 0x01);

     //nastaveni ctrl registru 0xF4
     portb = 0x00;
     SPI1_Write(0xF4);
     SPI1_Write((((0x02<<5) | (0x05<<2)) | (0x03)) & 0xFF);
     //nastaveni cfg registru 0xF5
     SPI1_Write(0xF5);
     SPI1_Write(((0x04<<5) |  (0x00<<2)) & 0xFF);
     portb = 0x01;

     portb = 0x00;
     SPI1_Read(0x88);
     temp16_1 = SPI1_Read(buffer);
     temp16_1 = temp16_1 << 8;
     temp16_1 = SPI1_Read(buffer);
     portb = 0x01;
     delay_ms(100);

     portb = 0x00;
     SPI1_Read(0x8A);
     temp16_2[0] = SPI1_Read(buffer);
     temp16_2[0] = temp16_2[0] << 8;
     temp16_2[0] = SPI1_Read(buffer);
     portb = 0x01;
     delay_ms(100);

     portb = 0x00;
     SPI1_Read(0x8C);
     temp16_2[1] = SPI1_Read(buffer);
     temp16_2[1] = temp16_2[1] << 8;
     temp16_2[1] = SPI1_Read(buffer);
     portb = 0x01;
     delay_ms(100);

     portb = 0x00;
     SPI1_Read(0x8E);
     press16_1 = SPI1_Read(buffer);
     press16_1 = press16_1 << 8;
     press16_1 = SPI1_Read(buffer);
     portb = 0x01;
     delay_ms(100);

     portb = 0x00;
     SPI1_Read(0x90);
     press16_2[0] = SPI1_Read(buffer);
     press16_2[0] = press16_2[0] << 8;
     press16_2[0] = SPI1_Read(buffer);
     portb = 0x01;
     delay_ms(100);

     portb = 0x00;
     SPI1_Read(0x92);
     press16_2[1] = SPI1_Read(buffer);
     press16_2[1] = press16_2[1] << 8;
     press16_2[1] = SPI1_Read(buffer);
     portb = 0x01;
     delay_ms(100);

     portb = 0x00;
     SPI1_Read(0x94);
     press16_2[2] = SPI1_Read(buffer);
     press16_2[2] = press16_2[2] << 8;
     press16_2[2] = SPI1_Read(buffer);
     portb = 0x01;
     delay_ms(100);

     portb = 0x00;
     SPI1_Read(0x96);
     press16_2[3] = SPI1_Read(buffer);
     press16_2[3] = press16_2[3] << 8;
     press16_2[3] = SPI1_Read(buffer);
     portb = 0x01;
     delay_ms(100);

     portb = 0x00;
     SPI1_Read(0x98);
     press16_2[4] = SPI1_Read(buffer);
     press16_2[4] = press16_2[4] << 8;
     press16_2[4] = SPI1_Read(buffer);
     portb = 0x01;
     delay_ms(100);

     portb = 0x00;
     SPI1_Read(0x9A);
     press16_2[5] = SPI1_Read(buffer);
     press16_2[5] = press16_2[5] << 8;
     press16_2[5] = SPI1_Read(buffer);
     portb = 0x01;
     delay_ms(100);

     portb = 0x00;
     SPI1_Read(0x9C);
     press16_2[6] = SPI1_Read(buffer);
     press16_2[6] = press16_2[6] << 8;
     press16_2[6] = SPI1_Read(buffer);
     portb = 0x01;
     delay_ms(100);

     portb = 0x00;
     SPI1_Read(0x9E);
     press16_2[7] = SPI1_Read(buffer);
     press16_2[7] = press16_2[7] << 8;
     press16_2[7] = SPI1_Read(buffer);
     portb = 0x01;
     delay_ms(100);

     return;
     }

/*******************************************************/
void Prijem(void)
     {
     if (UART1_Data_Ready() == 1) {          // Jsou Data?
          UART1_Read_Text(pInPole, IN_OUT_TERMINATOR, VELIKOST_BUFFER);
          Zarizeni=InPole[0];
          Adresa=InPole[1];
          Sta=InPole[2];
          Des=InPole[3];
          Jed=InPole[4];
          CRC=InPole[5];
        }
     return;
     }
/*******************************************************/
int Prevod(char sta,char des,char jed)
    {
    int vystup;
    vystup = (sta-48)*100+(des-48)*10+(jed-48);
    return vystup;
    }

// int Prevod(char sta,char des,char jed);
// int32_t vypocet_temp(int32_t temperature){
//  int32_t var1, var2, T, t_fine;
//  var1 = ((((temperature/8) - ((int32_t)temp16_1*2)))*((int32_t)temp16_2))/2048;
//  var2 = (((((temperature/16)-((int32_t)temp16_1))*((temperature/16)-((int32_t)temp16_1)))/4096)*((int32_t)temp16_3))/16384;
//  t_fine = var1 + var2;
//  T = ((t_fine*5)+128) /256;
//  return T;
// }

void main() {
     init();
     do
       {
        Prijem();
        switch(Adresa)
             {
             /*nastaveni bitu*/
             case 'A':
                portb = 0x00;
                SPI1_Read(0xF7);
                temp[0] = SPI1_Read(buffer);
                temp[1] = SPI1_Read(buffer);
                portb = 0x01;
                delay_ms(100);

                portb = 0x00;
                SPI1_Read(0xF9);
                temp[2] = SPI1_Read(buffer);
                portb = 0x01;
                delay_ms(100);
                
                portb = 0x00;
                SPI1_Read(0xFA);
                press[0] = SPI1_Read(buffer);
                press[1] = SPI1_Read(buffer);
                portb = 0x01;
                delay_ms(100);
                
                portb = 0x00;
                SPI1_Read(0xFC);
                press[2] = SPI1_Read(buffer);
                portb = 0x01;
                delay_ms(100);

                temp[0] = temp[0] << 8;
                temp[1] = temp[1] << 8;
                temp[2] = temp[2] >> 4;
                
                press[0] = press[0] << 8;
                press[1] = press[1] << 8;
                press[2] = press[2] >> 4;

                tepl = temp[0];
                tepl = tepl << 8;
                tepl = (tepl | temp[1]) | temp[2];

                tlak = press[0];
                tlak = tlak << 8;
                tlak = (tlak | press[1]) | press[2];

                LongToStr(tepl,temp_b);
                UART1_Write_Text(temp_b);
                UART1_Write(13);
                
                LongToStr(tlak,temp_b);
                UART1_Write_Text(temp_b);
                UART1_Write(13);

                delay_ms(500);
                break;
               }
        Adresa=0;
       } while(1);
    }