enum {
  BMP280_REGISTER_DIG_T1 = 0x88,
  BMP280_REGISTER_DIG_T2 = 0x8A,
  BMP280_REGISTER_DIG_T3 = 0x8C,
  BMP280_REGISTER_DIG_P1 = 0x8E,
  BMP280_REGISTER_DIG_P2 = 0x90,
  BMP280_REGISTER_DIG_P3 = 0x92,
  BMP280_REGISTER_DIG_P4 = 0x94,
  BMP280_REGISTER_DIG_P5 = 0x96,
  BMP280_REGISTER_DIG_P6 = 0x98,
  BMP280_REGISTER_DIG_P7 = 0x9A,
  BMP280_REGISTER_DIG_P8 = 0x9C,
  BMP280_REGISTER_DIG_P9 = 0x9E,
  BMP280_REGISTER_CHIPID = 0xD0,
  BMP280_REGISTER_VERSION = 0xD1,
  BMP280_REGISTER_SOFTRESET = 0xE0,
  BMP280_REGISTER_CAL26 = 0xE1, //kalibrace
  BMP280_REGISTER_STATUS = 0xF3,
  BMP280_REGISTER_CONTROL = 0xF4,
  BMP280_REGISTER_CONFIG = 0xF5,
  BMP280_REGISTER_PRESSUREDATA = 0xF7,
  BMP280_REGISTER_TEMPDATA = 0xFA,
};

typedef long signed int BMP280_S32_t;
typedef long unsigned int BMP280_U32_t;

BMP280_S32_t bmp280_com_T_int32(BMP280_S32_t adc_T){
    BMP280_S32_t var1, var2, T;
    var1 = ((((adc_T >> 3) - ((BMP280_S32_t)BMP280_REGISTER_DIG_T1<<1))) * ((BMP280_S32_t)BMP280_REGISTER_DIG_T2)) >> 11;
    var2 = (((((adc_T>>4)-((BMP280_S32_t)BMP280_REGISTER_DIG_T1)) * ((adc_T>>4)- ((BMP280_S32_t)BMP280_REGISTER_DIG_T1)) >> 12)* ((BMP280_S32_t)BMP280_REGISTER_DIG_T3)) >> 14);
    t_fine = var1 + var2;
    T = (t_fine * 5 + 127) >> 8;
    return T;
}

BMP280_U32_t bmp280_com_P_int32(BMP280_U32_t adc_P){
    BMP280_S32_t var1, var2;
    BMP280_U32_t p;
    var1 = (((BMP280_S32_t)t_fine)>>1)-(BMP280_S32_t)64000;
    var2 = (((var1>>2)*(var1>>2))>>11)*((BMP280_S32_t)BMP280_REGISTER_DIG_P6);
    var2 = var2 + ((var1*((BMP280_S32_t)BMP280_REGISTER_DIG_P5))<<1);
    var2 = (var2>>2)+(((BMP280_S32_t)BMP280_REGISTER_DIG_P4)<<16);
    var1 = (((BMP280_REGISTER_DIG_P3*(((var1>>2)*(var1>>2))>>13))>>3)+((((BMP280_S32_t)BMP280_REGISTER_DIG_P2)*var1)>>1))>>18;
    var1 = ((((32768+var1))*((BMP280_S32_t)BMP280_REGISTER_DIG_P1))>>15);
    if(var1 == 0){ //deleni nulou
        return 0;
    }
    p = (((BMP280_U32_t)(((BMP280_S32_t)1048576)-adc_P)-(var2>>12)))*3125;
    if(p < 0x80000000){
        p = (p << 1) / ((BMP280_U32_t)var1);
    }
    else{
        p = (p /(BMP280_U32_t)var1) * 2;
    }
    var1 = (((BMP280_S32_t)BMP280_REGISTER_DIG_P9)*((BMP280_S32_t)(((p>>3)*(p>>3))>>13)))>>12;
    var2 = (((BMP280_S32_t)(p>>2))*((BMP280_S32_t)BMP280_REGISTER_DIG_P8))>>13;
    p = (BMP280_U32_t)((BMP280_S32_t)p+((var1+var2+BMP280_REGISTER_DIG_P7)>>4));
    return p;
}

