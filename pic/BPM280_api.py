def vypocet_temp(temperature):
 var1 = int(( ((temperature>>3) - (30463<<1) )*30463)>>11)
 var2 =  int ( ( ( ((temperature>>4)-(30463))*((temperature>>4)-(30463))>>12) *(30463))>>14 )
 t_fine = var1 + var2
 T = ((t_fine*5)+128)>>8

 return T

print(0.000012*vypocet_temp(7798535))
                          