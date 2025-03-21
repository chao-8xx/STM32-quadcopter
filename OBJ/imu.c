#include "imu.h"
#include "imath.h"
#include "math.h"
#include "mpu6050.h"
#include "timer.h"
#include "nav.h"
#include "inv_mpu.h"
#include "led.h"
#include "systick.h"


#define GYRO_RAW_TO_DEG_S       0.06103515625f   //速率 = ADC值/灵敏度；+-2000°/s:16.384LSB/°/s

Attitude att = {0};
_Time_test att_time;
extern PlaneData plane;

//获取欧拉角
void IMU(float *roll,float *pitch,float *yaw)
{
//	u8 res;
	
  time_check(&att_time);	 //姿态解算时间检测
	
//	res = 
	mpu_dmp_get_data(roll,pitch,yaw);		//飞机需稳定，才能获取到数据
//	if(res == 0){
////		plane.dmp = DMP_NORMAL;
////		RGB_LedBlink(WHITE);
////		RGB_LedBlink(CYAN);
//	} else {
////		plane.dmp = DMP_NOT;
////		RGB_LedBlink(WHITE);
//	}
}

//陀螺仪数据转换成度/秒
void GyroDataTransformDeg(float *gyroX,float *gyroY,float *gyroZ)
{
	short gX,gY,gZ;
	MPU6050_Get_Gyroscope(&gX,&gY,&gZ);
	
	*gyroX = (float)(gX * GYRO_RAW_TO_DEG_S);
	*gyroY = (float)(gY * GYRO_RAW_TO_DEG_S);
	*gyroZ = (float)(gZ * GYRO_RAW_TO_DEG_S);
}
