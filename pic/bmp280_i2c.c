#include <stdint.h>
#define  NULL                  0
#define  IN_OUT_TERMINATOR     "~"
#define  VELIKOST_BUFFER       10

int32_t adc_T, adc_P, t_fine, temperature;
unsigned char buffer;
uint8_t status_reg;
char temp_char[15];
char temp_b[4];
char temp_s[5];
char temp_w[6];
char temp_i[7];
char temp_l[12];
double temper_double;

uint16_t T1;
int16_t T2, T3;

int8_t T4, T5, T6;

char* pInPole =  NULL, pOutPole =  NULL;
char InPole[VELIKOST_BUFFER];
char OutPole[VELIKOST_BUFFER];
char Zarizeni, Adresa, Sta, Des, Jed, CRC;
uint8_t _cfg_reg, _ctrl_reg;


void write8(unsigned char _reg_adr,unsigned char _data){
     I2C1_Start();
     I2C1_Wr(0x76);
     I2C1_Wr(_reg_adr);
     I2C1_Wr(_data);
     I2C1_Stop();
}

unsigned char read8(unsigned char _reg_adr){
        unsigned char _data;
        I2C1_Start();
        I2C1_Wr(0x76);
        I2C1_Wr(_reg_adr);
        _data = I2C1_Rd(0);
        I2C1_Stop();
        return _data;
}

void init(){
     status_reg = 0;
     pInPole  = &InPole[0];
     pOutPole = &OutPole[0];
     UART1_Init(9600);
     I2C1_Init(100000);
     Delay_ms(100);
     write8(0xE0,0xB6);
     delay_ms(100);
     write8(0xE0,0x00);
     delay_ms(100);
     
     do{
      UART1_Write_Text("Nacitani dat z NVM");
      status_reg = read8(0xF3);
      delay_ms(200);
     }while((status_reg & 0x01) == 0x01);
     
     _cfg_reg = ((0x04<<5) |  (0x05<<2)) & 0xFF;
     _ctrl_reg = (((0x02<<5) | (0x05<<2)) | (0x03)) & 0xFF;
     write8(0xF5,_cfg_reg);
     delay_us(20);
     write8(0xF4,_ctrl_reg);
     delay_us(20);
     
     T1 = read8(0x88);
     T1 = T1 << 8;

     T1 = T1 | read8(0x89);

     T2 = read8(0x8A);
     T2 = T2 << 8;
     T2 = T2 | read8(0x8B);

     T3 = read8(0x8C);
     T3 = T3 << 8;
     
     T3 = T3 | read8(0x8D);
     delay_us(20);
     return;
}

void Prijem(void){
     if (UART1_Data_Ready() == 1) {
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
int Prevod(char sta,char des,char jed){
    int vystup;
    vystup = (sta-48)*100+(des-48)*10+(jed-48);
    return vystup;
}

void main(){
    init();
    delay_ms(100);
    while(1){
              Prijem();
        switch (Adresa)
             {
                case 'Z':

                 I2C1_Start();
                 I2C1_Wr(0x76);
                 I2C1_Wr(0xF7);
                 adc_T = I2C1_Rd(0);
                 adc_T = adc_T << 8;
                 delay_us(20);
                 adc_T = adc_T | I2C1_Rd(0);
                 adc_T = adc_T << 8;
                 delay_us(20);
                 adc_T = adc_T | (I2C1_Rd(0) >> 4);
                 I2C1_Stop();
                 delay_us(20);


                 LongToStr(adc_T,temp_l);
                 UART1_Write_Text(temp_l);
                 UART1_Write(13);
                 delay_ms(500);
                 
                 break;
                }
        Adresa=0;
        delay_ms(100);
    }
}