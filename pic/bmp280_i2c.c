#include <stdint.h>
#define  NULL                  0
#define  IN_OUT_TERMINATOR     "~"
#define  VELIKOST_BUFFER       10

int32_t tepl;
unsigned char buffer;
uint8_t status_reg;
char temp_char[15];
char temp_b[4];
char temp_s[5];
char temp_w[6];
char temp_i[7];
char temp_l[12];

uint8_t chip_id;
uint16_t temp16[3];

uint16_t T1[2];
int16_t T2[4];


char* pInPole =  NULL, pOutPole =  NULL;
char InPole[VELIKOST_BUFFER];
char OutPole[VELIKOST_BUFFER];
char Zarizeni, Adresa, Sta, Des, Jed, CRC;

void init(){
     trisc = 0x18;
     status_reg = 0;
     pInPole  = &InPole[0];
     pOutPole = &OutPole[0];
     UART1_Init(9600);
     I2C1_Init(100000);
     Delay_ms(100);

     I2C1_Start();
     I2C1_Wr(0x76);
     I2C1_Wr(0xE0);
     I2C1_Wr(0xB6);
     I2C1_Stop(); 
     delay_ms(100);

     I2C1_Start();
     I2C1_Wr(0x76);
     I2C1_Wr(0xE0);
     I2C1_Wr(0x00);
     I2C1_Stop();
     
     do{
          UART1_Write_Text("Nacitani dat z NVM");
          I2C1_Start();
          I2C1_Wr(0x76);
          I2C1_Wr(0xF3);
          I2C1_Start();
          I2C1_Wr(0x76);
          status_reg = I2C1_Rd(1);
          I2C1_Stop();      
          delay_ms(200);
     }while((status_reg & 0x01) == 0x01);
     
     I2C1_Start();
     I2C1_Wr(0x76);
     I2C1_Wr(0xF4);
     I2C1_Wr(0x43);
     I2C1_Stop();

     delay_ms(100);

     I2C1_Start();
     I2C1_Wr(0x76);
     I2C1_Wr(0xF5);
     I2C1_Wr(0x80);
     I2C1_Stop();
     delay_ms(100);

     I2C1_Start();
     I2C1_Wr(0x76);
     I2C1_Wr(0x88);
     I2C1_Start();
     I2C1_Wr(0x76);
     T1[0] = I2C1_Rd(1);
     T1[1] = I2C1_Rd(1);
     I2C1_Stop();
     delay_ms(100);

     I2C1_Start();
     I2C1_Wr(0x76);
     I2C1_Wr(0x8A);
     I2C1_Start();
     I2C1_Wr(0x76);
     T2[0] = I2C1_Rd(1);
     T2[1] = I2C1_Rd(1);
     I2C1_Stop();
     delay_ms(100);

     I2C1_Start();
     I2C1_Wr(0x76);
     I2C1_Wr(0x8C);
     I2C1_Start();
     I2C1_Wr(0x76);
     T2[2] = I2C1_Rd(1);
     T2[3] = I2C1_Rd(1);
     I2C1_Stop();
     delay_ms(100);

     T1[0] = (T1[0] << 8) | T1[1];
     T2[0] = (T2[0] << 8) | T2[1];
     T2[2] = (T2[2] << 8) | T2[3];
     delay_ms(100);
     
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

    while(1){
     Prijem();
     switch (Adresa){
          case 'Z':

          // I2C1_Start();
          // I2C1_Wr(0x76);
          // I2C1_Wr(0xFA);
          // I2C1_Start();
          // I2C1_Wr(0x76); 
          // temp16[0] = I2C1_Rd(1);
          // temp16[1] = I2C1_Rd(0);
          // I2C1_Stop();
          // delay_ms(100);

          // I2C1_Start();
          // I2C1_Wr(0x76);
          // I2C1_Wr(0xFC);
          // I2C1_Start();
          // I2C1_Wr(0x76);
          // temp16[2] = I2C1_Rd(1);
          // I2C1_Stop();
          // delay_ms(100);

          // temp16[2] = temp16[2] >> 4;
          // temp16[1] = temp16[1] << 8;
          // tepl = temp16[0];
          // tepl = tepl << 16;

          // tepl = (tepl | temp16[1]) | temp16[2];

          // LongToStr(tepl,temp_l);
          // UART1_Write_Text(temp_l);
          // UART1_Write(13);

          WordToStr(T1[0],temp_w);
          UART1_Write_Text(temp_w);
          UART1_Write(13);
          delay_ms(100);

          IntToStr(T2[0],temp_i);
          UART1_Write_Text(temp_i);
          UART1_Write(13);
          delay_ms(100);

          IntToStr(T2[2],temp_i);
          UART1_Write_Text(temp_i);
          UART1_Write(13);

          break;
     }
     Adresa=0;
     delay_ms(500);
    }
}