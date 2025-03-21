#ifndef _imath_h_
#define _imath_h_

#include "stm32f10x.h"
#include "mpu6050.h"

float FastSqrt(float x);


float fast_atan2(float y, float x) ;


uint16_t  ThrottleLimit(uint16_t thr_in,uint16_t thr_min,uint16_t thr_max);
float direction_to_zero(float in_dat,float min_dat,float max_dat);
float MyAbs(float f);

//void  set_value(SI_F_XYZ *_in_data,float value);
//void _set_val(SI_F_XYZ *_out_data,SI_F_XYZ *_in_data);

#endif
