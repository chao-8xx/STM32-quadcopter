#include "acc_cal.h"
#include "math.h"
#include "parse_packet.h"
#include "flash.h"
#include "imath.h"
#include "gyro_cal.h"

//_ACC_CAL cal_acc = {0};
//extern PlaneData plane;

//void Calibrate_Reset_Matrices(float dS[6], float JS[6][6])
//{
//    int16_t j,k;
//    for( j=0; j<6; j++ )
//    {
//        dS[j] = 0.0f;
//        for( k=0; k<6; k++ )
//        {
//            JS[j][k] = 0.0f;
//        }
//    }
//}
//void Calibrate_Update_Matrices(float dS[6],
//                               float JS[6][6],
//                               float beta[6],
//                               float data[3])
//{
//    int16_t j, k;
//    float dx, b;
//    float residual = 1.0;
//    float jacobian[6];
//    for(j=0;j<3;j++)
//    {
//        b = beta[3+j];
//        dx = (float)data[j] - beta[j];
//        residual -= b*b*dx*dx;
//        jacobian[j] = 2.0f*b*b*dx;
//        jacobian[3+j] = -2.0f*b*dx*dx;
//    }

//    for(j=0;j<6;j++)
//    {
//        dS[j]+=jacobian[j]*residual;
//        for(k=0;k<6;k++)
//        {
//            JS[j][k]+=jacobian[j]*jacobian[k];
//        }
//    }
//}
//void Calibrate_Find_Delta(float dS[6], float JS[6][6], float delta[6])
//{
//    //Solve 6-d matrix equation JS*x = dS
//    //first put in upper triangular form
//    int16_t i,j,k;
//    float mu;
//    //make upper triangular
//    for( i=0; i<6; i++ ) 
//    {
//        //eliminate all nonzero entries below JS[i][i]
//        for( j=i+1; j<6; j++ ) 
//        {
//            mu = JS[i][j]/JS[i][i];
//            if( mu != 0.0f ) 
//            {
//                dS[j] -= mu*dS[i];
//                for( k=j; k<6; k++ ) 
//                {
//                    JS[k][j] -= mu*JS[k][i];
//                }
//            }
//        }
//    }
//    //back-substitute
//    for( i=5; i>=0; i-- ) 
//    {
//        dS[i] /= JS[i][i];
//        JS[i][i] = 1.0f;

//        for( j=0; j<i; j++ ) 
//        {
//            mu = JS[i][j];
//            dS[j] -= mu*dS[i];
//            JS[i][j] = 0.0f;
//        }
//    }
//    for( i=0; i<6; i++ ) 
//    {
//        delta[i] = dS[i];
//    }
//}

//uint8_t Calibrate_accel(  SI_F_XYZ accel_sample[6],
//                          SI_F_XYZ *accel_offsets,
//                          SI_F_XYZ *accel_scale)
//{
//    int16_t i;
//    int16_t num_iterations = 0;
//    float eps = 0.000000001;
//    float change = 100.0;
//    float data[3]={0};
//    float beta[6]={0};
//    float delta[6]={0};
//    float ds[6]={0};
//    float JS[6][6]={0};
//    bool success = true;
//    // reset
//    beta[0] = beta[1] = beta[2] = 0;
//    beta[3] = beta[4] = beta[5] = 1.0f/gravity_mss;
//    while( num_iterations < 20 && change > eps ) 
//    {
//        num_iterations++;
//        Calibrate_Reset_Matrices(ds, JS);

//        for( i=0; i<6; i++ ) 
//        {
//            data[0] = accel_sample[i].x;
//            data[1] = accel_sample[i].y;
//            data[2] = accel_sample[i].z;
//            Calibrate_Update_Matrices(ds, JS, beta, data);
//        }
//        Calibrate_Find_Delta(ds, JS, delta);
//        change =    delta[0]*delta[0] +
//                    delta[0]*delta[0] +
//                    delta[1]*delta[1] +
//                    delta[2]*delta[2] +
//                    delta[3]*delta[3] / (beta[3]*beta[3]) +
//                    delta[4]*delta[4] / (beta[4]*beta[4]) +
//                    delta[5]*delta[5] / (beta[5]*beta[5]);
//        for( i=0; i<6; i++ ) 
//        {
//            beta[i] -= delta[i];
//        }
//    }
//    // copy results out
//    accel_scale->x = beta[3] * gravity_mss;
//    accel_scale->y = beta[4] * gravity_mss;
//    accel_scale->z = beta[5] * gravity_mss;
//    accel_offsets->x = beta[0] * accel_scale->x;
//    accel_offsets->y = beta[1] * accel_scale->y;
//    accel_offsets->z = beta[2] * accel_scale->z;

//    // sanity check scale
//    if(fabsf(accel_scale->x-1.0f) > 0.2f|| fabsf(accel_scale->y-1.0f) > 0.2f|| fabsf(accel_scale->z-1.0f) > 0.2f )
//    {
//        success = false;
//    }
//    // sanity check offsets (3.5 is roughly 3/10th of a G, 5.0 is roughly half a G)
//    if(fabsf(accel_offsets->x) > 3.5f|| fabsf(accel_offsets->y) > 3.5f|| fabsf(accel_offsets->z) > 3.5f )
//    {
//        success = false;
//    }
//    // return success or failure
//    return success;
//}
// 


////加速度计校准轮询
//void acc_cal_polling(void)
//{
//    if(plane.mode==1&&plane.pit>=25&&plane.rol<=-25)
//        cal_acc.cnt++;
//    if(plane.mode==1&&plane.pit>=25&&plane.rol<=-25&&cal_acc.cnt>=600)       //进入校准状态并持续3s 
//    {
//        cal_acc.cnt = 0;
//        cal_acc.flag = 1;
//        cal_acc.start_flag = 1;                                     //作为陀螺仪校准时状态显示使用
//        
//        for(uint8_t i=1;i<7;i++)                                        
//        {
//            set_value(&cal_acc.samples[i],0.0f);                    //进入校准时首先清空数据缓存
//        }
//        
//        for(uint8_t i=1;i<7;i++)
//        {
//            cal_acc.single_finish_flag[i] = 0;                      //进入时对应面标志位清零
//        }             
//    }
//    if(cal_acc.flag==1)                                             //进入加速度计校准模式
//    {
//        if(plane.mode==0)                                              //按键松开
//        {
//            //第一面（平放）：偏航杆右下方打满 >= 500ms
//            if(plane.yaw<=-25)
//            {
//                cal_acc.single_cnt++;
//                if(cal_acc.single_cnt==300)
//                    cal_acc.single = 1;    
//            }
//            //第二面（左面）：横滚杆左边打满 >= 500ms
//            if(plane.rol<=-25)
//            {
//                cal_acc.single_cnt++;
//                if(cal_acc.single_cnt==300)
//                    cal_acc.single = 2;    
//            }        
//            //第三面（右面）：横滚杆右边打满 >= 500ms
//            if(plane.rol>=25)
//            {
//                cal_acc.single_cnt++;
//                if(cal_acc.single_cnt==300)
//                    cal_acc.single = 3;    
//            }
//            //第四面（前面）：俯仰杆前打满 >= 500ms 
//            if(plane.pit<=-25)
//            {
//                cal_acc.single_cnt++;
//                if(cal_acc.single_cnt==300)
//                    cal_acc.single = 4;    
//            }        
//            //第五面 (后面) ：俯仰杆后打满 >= 500ms
//            if(plane.pit>=25)
//            {
//                cal_acc.single_cnt++;
//                if(cal_acc.single_cnt==300)
//                    cal_acc.single = 5;    
//            }            
//            //第六面（背面）：偏航杆左下方打满 >= 500ms        
//            if(plane.yaw>=25)
//            {
//                cal_acc.single_cnt++;
//                if(cal_acc.single_cnt==300)
//                    cal_acc.single = 6;    
//            }      
//            //未操作时，计数器清除
//            if(cal_acc.single_cnt>300)
//            {
//                cal_acc.single_cnt = 0;
//            }               
//        }
//    }           
//}
//#define acc_cal_sum_value 150000

//SI_F_XYZ acc_cal_samples[6] = {0};


////加速度校准进行
//void acc_cal(SI_F_XYZ *acc_in)
//{
//    if(cal_acc.single!=0)
//    {
//        if(cal_acc.i<acc_cal_sum_value)             //单面数据采集
//        {
//            cal_acc.samples_sum.x += (acc_in->x * acc_to_1g);
//            cal_acc.samples_sum.y += (acc_in->y * acc_to_1g);
//            cal_acc.samples_sum.z += (acc_in->z * acc_to_1g);
//            
//            cal_acc.i++;
//        }
//        else                                        //单面数据采集完毕
//        {
//            cal_acc.i = 0;
//                                       
//            cal_acc.samples[cal_acc.single].x = cal_acc.samples_sum.x / acc_cal_sum_value;
//            cal_acc.samples[cal_acc.single].y = cal_acc.samples_sum.y / acc_cal_sum_value;
//            cal_acc.samples[cal_acc.single].z = cal_acc.samples_sum.z / acc_cal_sum_value; 
//            
//            //对三轴累计数据进行清除，以免下次带入计算     
//            set_value(&cal_acc.samples_sum,0.0f);   

//            cal_acc.single_finish_flag[cal_acc.single] = 1;                 //对应面采集完毕使其对应面标志置位
//            
//            cal_acc.single = 0;
//        }
//        //六面数据全部采集完成
//        if(  cal_acc.single_finish_flag[1]
//            &cal_acc.single_finish_flag[2]
//            &cal_acc.single_finish_flag[3]
//            &cal_acc.single_finish_flag[4]
//            &cal_acc.single_finish_flag[5]
//            &cal_acc.single_finish_flag[6])
//        {
//            cal_acc.all_finish_flag = 1;                                    //所有面校准完成标志置位，作为指示灯提示
//       
//            //加速度计校准数据转存
//            for(uint8_t i=0;i<6;i++)
//            {
//                _set_val(&acc_cal_samples[i],&cal_acc.samples[i+1]);
//            }
//            //加速度计校准
//            cal_acc.return_flag = Calibrate_accel(acc_cal_samples,&cal_acc.offset_flash_write,&cal_acc.scale_flash_write);
//            
//            if(cal_acc.return_flag==true)
//            {
//                //校准成功后写入flash        
//                flash_write_cal(sensor_cal_address, cal_acc.offset_flash_write.x,
//                                                    cal_acc.offset_flash_write.y,
//                                                    cal_acc.offset_flash_write.z,
//        
//                                                    cal_acc.scale_flash_write.x,
//                                                    cal_acc.scale_flash_write.y,
//                                                    cal_acc.scale_flash_write.z,
//        
//                                                    cal_gyro.offset_flash_read.x,
//                                                    cal_gyro.offset_flash_read.y,
//                                                    cal_gyro.offset_flash_read.z);             
//            }                        
//            for(uint8_t i=1;i<7;i++)
//            {
//                cal_acc.single_finish_flag[i] = 0;                          //对应面标志位清零
//            }
//            
//            cal_acc.flag = 0;                                               //清除标志位，退出校准模式
//        }
//    }
//}

//bool read_cal_dat(void)
//{   
//    bool success = true;
//    
//    //初始化标度系数和零位误差系数
//    for(uint8_t i=0;i<3;i++)
//    {
//        cal_acc.K[i] = 1.0f;
//        cal_acc.B[i] = 0.0f;
//    }
//	//校准数据读取
//    flash_flag.sensor = flash_read_cal(sensor_cal_address,  &cal_acc.offset_flash_read.x,
//                                                            &cal_acc.offset_flash_read.y,
//                                                            &cal_acc.offset_flash_read.z, 

//                                                            &cal_acc.scale_flash_read.x,    
//                                                            &cal_acc.scale_flash_read.y,
//                                                            &cal_acc.scale_flash_read.z,

//                                                            &cal_gyro.offset_flash_read.x,
//                                                            &cal_gyro.offset_flash_read.y,
//                                                            &cal_gyro.offset_flash_read.z);
//    // sanity check offsets (3.5 is roughly 3/10th of a G, 5.0 is roughly half a G)
//    if(MyAbs(cal_acc.offset_flash_read.x) > 5.0f || MyAbs(cal_acc.offset_flash_read.y) > 5.0f || MyAbs(cal_acc.offset_flash_read.z) > 5.0f)
//    {
//        success = false;
//    }    
//    // sanity check scale
//    if(MyAbs(cal_acc.scale_flash_read.x-1.0f)>0.5f|| MyAbs(cal_acc.scale_flash_read.y-1.0f)>0.5f|| MyAbs(cal_acc.scale_flash_read.z-1.0f)>0.5f)
//    {
//        success = false;
//    }

//    if(success==true&&flash_flag.sensor!=0x01ff&&flash_flag.sensor!=0x003f)    
//    {
//        //加速度计零偏
//        cal_acc.B[0] = cal_acc.offset_flash_read.x;
//        cal_acc.B[1] = cal_acc.offset_flash_read.y;
//        cal_acc.B[2] = cal_acc.offset_flash_read.z;
//        
//        //加速度计比例因子
//        cal_acc.K[0] = cal_acc.scale_flash_read.x;
//        cal_acc.K[1] = cal_acc.scale_flash_read.y;
//        cal_acc.K[2] = cal_acc.scale_flash_read.z;
//    }
//    
//    //判断是否正确读出
//    if(flash_flag.sensor!=0x01ff&&flash_flag.sensor!=0x01C0)
//    {
//        //陀螺仪零偏设置
//        _set_val(&gyro_offset,&cal_gyro.offset_flash_read);
//    }   

//    return success;
//}    





