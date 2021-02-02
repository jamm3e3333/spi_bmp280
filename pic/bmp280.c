#include <stdint.h>
#define  NULL                  0
#define  IN_OUT_TERMINATOR     "~"//126
#define  VELIKOST_BUFFER       10
int32_t tepl;
float temp_f;
unsigned char temp_c[8];
unsigned char temp_w[6];
unsigned char temp_i[7];
unsigned char temp32_c[11];
unsigned char temp32i_c[12];
unsigned char temp_float[14];
uint8_t temp3;

uint16_t temp16_1;
int16_t temp16_2;
int16_t temp16_3;

int32_t adc_t, t_fine;

// Ukazatele na pole
char* pInPole =  NULL, pOutPole =  NULL;
// Vstupni/Vystupni buffer
char InPole[VELIKOST_BUFFER];
char OutPole[VELIKOST_BUFFER];

char Zarizeni, Adresa, Sta, Des, Jed, CRC;
unsigned char buffer;
uint8_t status_reg;

void init(void);
void Prijem(void);
int Prevod(char sta,char des,char jed);
int32_t vypocet_temp(int32_t temperature){
 int32_t var1, var2, T, t_fine;
 var1 = ((((temperature/8) - ((int32_t)temp16_1*2)))*((int32_t)temp16_2))/2048;
 var2 = (((((temperature/16)-((int32_t)temp16_1))*((temperature/16)-((int32_t)temp16_1)))/4096)*((int32_t)temp16_3))/16384;
 t_fine = var1 + var2;
 T = ((t_fine*5)+128) /256;
 return T;
}

void main() {
     init();
     do
       {
        Prijem();
        switch (Adresa)
             {
             /*nastaveni bitu*/
             case 'A':
                  //nacteni surovych dat pro teplotu
                  portc = 0x20;
                  SPI1_Write(0xF7);
                  adc_t = SPI1_Read(buffer);
                  adc_t = adc_t << 8;
                  
                  SPI1_Write(0xF8);
                  adc_t = adc_t | SPI1_Read(buffer);
                  adc_t = adc_t << 8;
                  
                  SPI1_Write(0xF9);
                  temp3 = SPI1_Read(buffer);
                  adc_t = adc_t | (temp3 >> 4);
                  portc = 0x24;
                  delay_ms(100);
                  //konec nacitani dat

                  //nacitani kalibracnich registru
                  portc = 0x20;
                  SPI1_Write(0x88);
                  temp16_1 = SPI1_Read(buffer);
                  temp16_1 = temp16_1 << 8;
                  
                  SPI1_Write(0x89);
                  temp16_1 = temp16_1 | SPI1_Read(buffer);

                  SPI1_Write(0x8A);
                  temp16_2 = SPI1_Read(buffer);
                  temp16_2 = temp16_2 << 8;
                  
                  SPI1_Write(0x8B);
                  temp16_2 = temp16_2 | SPI1_Read(buffer);

                  SPI1_Write(0x8C);
                  temp16_3 = SPI1_Read(buffer);
                  temp16_3 = temp16_3 << 8;
                  
                  SPI1_Write(0x8D);
                  temp16_3 = temp16_3 | SPI1_Read(buffer);
                  portc = 0x24;

                  UART1_Write(13);
                  delay_ms(500);
                  tepl = vypocet_temp(adc_t);
                  temp_f = (float)tepl*0.0025;
                  FloatToStr(temp_f,temp_float);
                  UART1_Write_Text(temp_float);
                  UART1_Write(13);
                  //konec nacitani kalibracnich regitru
                  delay_ms(1500);
                  break;
               }
        Adresa=0;
       } while(1);
    }
/*******************************************************/
void init(void)
     {
     //nastaveni vstupu a vystuu
     trisc = 0x28;
     pInPole  = &InPole[0];
     pOutPole = &OutPole[0];
     //inicializace uart komunikace
     UART1_Init(9600);
     //incializace spi komunikace
     SPI1_Init();
     //soft reset
     portc = 0x20;
     SPI1_Write(0xE0);
     SPI1_Write(0xB6);
     portc = 0x24;
     Delay_ms(100);
     portc = 0x20;
     SPI1_Write(0xE0);
     SPI1_Write(0x00);
     portc = 0x24;
     delay_ms(100);
     //cekani na nacteni dat z NVM ro registru
     do{
       portc = 0x20;
       SPI1_Write(0xF3);
       status_reg = SPI1_Read(buffer);
       portc = 0x24;
       delay_ms(100);
     }while((status_reg & 0x01) == 0x01);

     //konfiguarce registru 0xF4 (mod, vzorkovani dat pro teplotu a tlak)
     portc = 0x20;
     SPI1_Write(0xF4);
     SPI1_Write((0x03 | (0x03 << 5))& 0xFF);
     portc = 0x24;
     delay_ms(100);
     
     //konfigurace registru 0xF5 (prodleva, filtr, spi-3 piny)
     portc = 0x20;
     SPI1_Write(0xF5);
     SPI1_Write((0x06 << 5) & 0xE0);
     portc = 0x24;
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