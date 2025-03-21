#include "gyro_cal.h"
#include "parse_packet.h"
#include "flash.h"
#include "acc_cal.h"
//#include "mag_cal.h"


//_GYRO_CAL cal_gyro = {0};

#define gyro_cal_sum_value 500

extern PlaneData plane;

//陀螺仪零偏校准
//void gyro_cal(SI_F_XYZ *gyro_in)
//{
//    if(cal_gyro.flag==1)                                      //校准开始  
//    {
//        if(cal_gyro.i < gyro_cal_sum_value)		              //求取平均	
//        {                       
//            cal_gyro.offset.x += gyro_in->x; 
//            cal_gyro.offset.y += gyro_in->y;
//            cal_gyro.offset.z += gyro_in->z;
//			
//			cal_gyro.i++;
//        }
//        else
//        {
//            cal_gyro.i = 0;
//            
//            cal_gyro.offset_flash_write.x = cal_gyro.offset.x / gyro_cal_sum_value;    //得到三轴的零偏 
//            cal_gyro.offset_flash_write.y = cal_gyro.offset.y / gyro_cal_sum_value;    //得到三轴的零偏
//            cal_gyro.offset_flash_write.z = cal_gyro.offset.z / gyro_cal_sum_value;    //得到三轴的零偏

//            //将陀螺仪零偏写入flash
//            flash_write_cal(sensor_cal_address, cal_acc.offset_flash_read.x,
//                                                cal_acc.offset_flash_read.y,
//                                                cal_acc.offset_flash_read.z,
//                                                
//                                                cal_acc.scale_flash_read.x,
//                                                cal_acc.scale_flash_read.y,
//                                                cal_acc.scale_flash_read.z,
//    
//                                                cal_gyro.offset_flash_write.x,
//                                                cal_gyro.offset_flash_write.y,
//                                                cal_gyro.offset_flash_write.z);         
//            cal_gyro.flag = 0;                                                      //校准标志位清除
//        }
//    }
//}


//陀螺仪校准查询
//void gyro_cal_polling(void)
//{
//    if(plane.mode==1&&plane.pit>=25&&plane.rol==0)
//        cal_gyro.cnt++;
//    if(plane.mode==1&&plane.pit>=25&&plane.rol==0&&cal_gyro.cnt>=600)       //进入校准状态并持续3s 
//    {
//        cal_gyro.cnt = 0;
//        cal_gyro.flag = 1;
//        cal_gyro.start_flag = 1;                                    //作为陀螺仪校准时状态显示使用
//    }
//}


















