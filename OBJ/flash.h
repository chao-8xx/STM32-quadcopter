#ifndef _flash_h_
#define _flash_h_

#include "stm32f10x.h"
#include "pid.h"

#define PID_WRITE_ADDRESS  0x08000000 + 1024*61		//“≥µÿ÷∑

typedef struct
{
    uint16_t sensor;
//    uint16_t acc_offset;
//    uint8_t acc_scale;
//    uint8_t mag_offset;
//	uint8_t gyro_offset;
}_FLASH_flag;



#define sensor_cal_address      36


   

void flash_write_cal(uint32_t WriteAddress, float WriteData1,float WriteData2,float WriteData3,
                                            float WriteData4,float WriteData5,float WriteData6,
                                            float WriteData7,float WriteData8,float WriteData9);
                                            
uint16_t flash_read_cal(uint32_t ReadAddress,float *ReadData1,float *ReadData2,float *ReadData3,
                                            float *ReadData4,float *ReadData5,float *ReadData6,
                                            float *ReadData7,float *ReadData8,float *ReadData9);


extern _FLASH_flag flash_flag;

//void PidDataWriteToFlash(const AllPid *pidArr);
void PidDataWriteToFlash(uint32_t pidWriteAddr, AllPid *pidArr);
float STM32_FLASH_ReadHalfWord(u32 faddr);
void PidDataReadFromFlash(uint32_t PidReadAddr,AllPid *pidArr);
#endif


