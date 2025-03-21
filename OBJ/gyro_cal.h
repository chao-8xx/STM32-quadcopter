#ifndef _gyro_cal_h_
#define _gyro_cal_h_



#include "stm32f10x.h"
#include "mpu6050.h"


//typedef struct
//{
//    uint16_t cnt;
//    uint16_t i;
//    
//    uint8_t  flag;
//    uint8_t  start_flag;    
//    float    flash_finish_flag;    
//    uint32_t sum;
//    
//    SI_F_XYZ offset;
//    SI_F_XYZ offset_flash_read;
//    SI_F_XYZ offset_flash_write;    
//}_GYRO_CAL;

//void gyro_cal(SI_F_XYZ *gyro_in);
void gyro_cal_polling(void);
void read_gyro_cal(void);


//extern _GYRO_CAL cal_gyro;




#endif


