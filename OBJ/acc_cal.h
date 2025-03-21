#ifndef _acc_cal_h_
#define _acc_cal_h_
#include "mpu6050.h"


#include "stm32f10x.h"

typedef enum
{
	false = 0,
	true = !false
}bool;


//typedef struct
//{
//    uint16_t cnt;
//    uint16_t single_cnt;
//    uint8_t  single;
//    uint32_t i;
//    uint8_t  flag;
//		uint8_t  return_flag;
//    uint8_t  start_flag;    
//		uint8_t  single_finish_flag[7];
//		uint8_t all_finish_flag;
//    SI_F_XYZ samples_sum;
//    SI_F_XYZ samples[7];
//	
//    SI_F_XYZ offset_flash_read;
//    SI_F_XYZ offset_flash_write;
//    SI_F_XYZ scale_flash_read;
//    SI_F_XYZ scale_flash_write;
//    
//    float flash_finish_flag;
//    
//		float B[3];
//		float K[3];
//	
//}_ACC_CAL;


//void acc_cal(SI_F_XYZ *acc_in);

//void acc_cal_polling(void);
//bool read_cal_dat(void);

////extern _ACC_CAL cal_acc;




#endif

