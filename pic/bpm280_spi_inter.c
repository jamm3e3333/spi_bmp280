#include <stdint.h>
#define  NULL                  0
#define  IN_OUT_TERMINATOR     "~"//126
#define  VELIKOST_BUFFER       10

int32_t tepl;
float temp_f;

unsigned char temp_b[4];
unsigned char temp_s[5];
unsigned char temp_c[8];
unsigned char temp_w[6];
unsigned char temp_i[7];
unsigned char temp32_c[11];
unsigned char temp32i_c[12];
unsigned char temp_float[14];
uint8_t temp1, temp2, temp3;

uint16_t temp16_1;
int16_t temp16_2;
int16_t temp16_3;

int32_t temperature, t_fine;

// Ukazatele na pole
char* pInPole =  NULL, pOutPole =  NULL;
// Vstupni/Vystupni buffer
char InPole[VELIKOST_BUFFER];
char OutPole[VELIKOST_BUFFER];

char Zarizeni, Adresa, Sta, Des, Jed, CRC;
unsigned char buffer;
uint8_t status_reg;

void write8(unsigned char _reg_add, unsigned char _data){
    portc.f2 = 0;
    SPI1_Write(_reg_add);
    SPI1_Write(_data);
    portc.f2 = 1;
}

uint8_t read8(uint8_t _reg_add){
    uint8_t data_temp;
    portc.f2 = 0;
    SPI1_Write(_reg_add);
    data_temp = SPI1_Read(buffer);
    portc.f2 = 1;
}

/*******************************************************/
void init(void)
     {
     //nastaveni vstupu a vystuu
     trisc = 0x10;
     SPI1_Init();
     portc.f2 = 1;
     
     pInPole  = &InPole[0];
     pOutPole = &OutPole[0];
     //inicializace uart komunikace
     UART1_Init(9600);
     //soft reset
     write8(0xE0,0xB6);
     delay_ms(100);
     //cekani na nacteni dat z NVM ro registru
     do{
       status_reg = read8(0xF3);
       delay_ms(100);
     }while((status_reg & 0x01) == 0x01);

     //nastaveni cfg registru 0xF5
     write8(0xF5,((0x04<<5) |  (0x05<<2)) & 0xFF);
     delay_ms(100);

    //nastaveni ctrl registru 0xF4
     write8(0xF4,(((0x02<<5) | (0x05<<2)) | (0x03)) & 0xFF);
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
                temp3 = read8(0xF7);   
                delay_ms(100);
                temp2 = read8(0xF8);
                delay_ms(100);
                temp3 = read8(0xF9);
                delay_ms(100);
                
                ByteToStr(temp1,temp_b);
                UART1_Write_Text(temp_b);
                UART1_Write(13);

                ByteToStr(temp2,temp_b);
                UART1_Write_Text(temp_b);
                UART1_Write(13);

                ByteToStr(temp3,temp_b);
                UART1_Write_Text(temp_b);
                UART1_Write(13);
                //konec nacitani kalibracnich regitru
                delay_ms(500);
                break;
               }
        Adresa=0;
       } while(1);
    }