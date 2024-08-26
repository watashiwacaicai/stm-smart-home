#ifndef __DSB_H
#define __DSB_H

int ReadTemperature(void);
void DS_DIR_IN(void);
void DS_DIR_OUT(void);
void Init_DS18B20(void);
void Write_Bit_0(void);
void Write_Bit_1(void);
unsigned char Read_Bit(void);
unsigned char ReadOneChar(void);
void WriteOneChar(unsigned char dat);
void show_tem(unsigned int bb);

#endif
