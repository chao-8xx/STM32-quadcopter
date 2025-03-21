#ifndef _imu_h_
#define _imu_h_

typedef struct 
{
	float pit;
	float rol;
	float yaw;
}Attitude;


//typedef struct
//{
//	float DCM[3][3];      //地理坐标系下的方向余弦
//  float DCM_T[3][3];    //机体坐标系下的方向余弦
//}_Matrix;

//extern _Matrix Mat;

extern Attitude att;
extern float sin_pit,sin_rol,sin_yaw;
extern float cos_pit,cos_rol,cos_yaw;

void IMUUpdate(float gx, float gy, float gz, float ax, float ay, float az) ;

void rotation_matrix(void);
void rotation_matrix_T(void);

void Matrix_ready(void);

void IMU(float *roll,float *pitch,float *yaw);
void GyroDataTransformDeg(float *gyroX,float *gyroY,float *gyroZ);

#endif

