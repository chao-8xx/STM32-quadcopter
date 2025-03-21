#include "controller.h"
#include "pid.h"
#include "imu.h"
#include "nrf24l01.h"
#include "pwm.h"
#include "mpu6050.h"
#include "parse_packet.h"
#include "fc_status.h"
#include "precompile.h"

_CONTROLLER_CNT controller_cnt = {0};
Throttle  throttle = {0};
_CONTROLLER_MODE _control = {0};

#define THROTTLE_FLYING  10

_OUT_Motor Motor1 = {0};
_OUT_Motor Motor2 = {0};
_OUT_Motor Motor3 = {0};
_OUT_Motor Motor4 = {0};

extern AllPid allPid;
extern PlaneData plane;
extern Mpu6050_Data Mpu;

//外环角度控制器
void AngleController(void)
{
    static uint16_t yaw_init_cnt = 0;

    allPid.rolAngle.feedback = att.rol;              //姿态解算的roll角作为反馈量
    PidController(&allPid.rolAngle);                 //外环pid控制器      
    allPid.pitAngle.feedback = att.pit;              //姿态解算的pitch角作为反馈量
    PidController(&allPid.pitAngle);                 //外环pid控制器    
    
    if(yaw_init_cnt<300)
        yaw_init_cnt++;
    else
    {
        if(plane.yaw==0)                               //偏航杆位于中位时进行双环控制
        {
            if(allPid.yawAngle.expect==0)
            {
                allPid.yawAngle.expect = att.yaw;     //最初时和偏航打舵之后，当前偏航角作为目标角度       
            }
            allPid.yawAngle.feedback = att.yaw;
            PidController(&allPid.yawAngle);         //外环pid控制器
            
            allPid.yawGyro.expect = allPid.yawAngle.out; 
        }
        else                                        //偏航进行打舵时，遥感直接作为速度环期望    
        {
            allPid.yawAngle.expect = 0;
            allPid.yawGyro.expect = plane.yaw*5;      
        }        
    }          
}
//内环角速度控制器
void GyroController(void)
{
    allPid.pitGyro.expect = allPid.pitAngle.out;         //外环输出作为内环期望值
		allPid.pitGyro.feedback = Mpu.deg_s.x;             //角速度值作为反馈量 
    PidController(&allPid.pitGyro);                   //内环pid控制器     
                                                    
    allPid.rolGyro.expect = allPid.rolAngle.out;         //外环输出作为内环期望值
    allPid.rolGyro.feedback = Mpu.deg_s.y;             //角速度值作为反馈量
    PidController(&allPid.rolGyro);                   //内环pid控制器 
                                                    
    allPid.yawGyro.feedback = Mpu.deg_s.z;             //角速度值作为反馈量   
    PidController(&allPid.yawGyro);                   //内环pid控制器    
}

uint8_t high_mark_flag = 0;                     //进入定高时油门记录标志位
uint8_t fix_mark_flag = 0;


//控制模式选择
void ControModel(void)
{
    //自稳模式
//    if(plane.high_flag==0)
//    {
        _control.mode = 1;         
        
        allPid.rolAngle.expect = plane.rol;		//遥控器数据作为rol的期望角
        allPid.pitAngle.expect = plane.pit;		//遥控器数据作为pit的期望角         
        throttle.FINAL_OUT = plane.throttle;		//最终油门输出来至于遥控器油门
//    }    
		
}

//控制器
void _controller_perform(void)
{
    switch(_control.mode)
    {
        case 1:
						//自稳模式    
						AngleController();  
						GyroController();
						break;      
        default :                           
						break;
    }
}


//姿态异常上锁
void UnusualProtect(void)
{
	if(att.rol <= -80 || att.rol >= 80 || att.pit <= -80 || att.pit >= 80){
		plane.lock = LOCK;
	}
}


//控制器pwm输出
void ControllerOutput(void)
{
		
	#if FOUR_AXIS_UAV
		UnusualProtect();	//飞机保护
	#elif FIXED_WING_AIRCRAFT
		//NOT
	#elif BRUSHLESS_FOUR_AXIS_UAV
		UnusualProtect();	//飞机保护
	#endif
	
    if(plane.lock == UNLOCK)		//解锁才输出            
    {
        if(plane.throttle>THROTTLE_FLYING)		//大于起飞油门
        {
					#if FOUR_AXIS_UAV 
            Motor1.out = throttle.FINAL_OUT + allPid.pitGyro.out - allPid.rolGyro.out - allPid.yawGyro.out;
            Motor2.out = throttle.FINAL_OUT + allPid.pitGyro.out + allPid.rolGyro.out + allPid.yawGyro.out;   
            Motor3.out = throttle.FINAL_OUT - allPid.pitGyro.out + allPid.rolGyro.out - allPid.yawGyro.out;
            Motor4.out = throttle.FINAL_OUT - allPid.pitGyro.out - allPid.rolGyro.out + allPid.yawGyro.out; 
					#elif	FIXED_WING_AIRCRAFT
						Motor1.out = (throttle.FINAL_OUT + 2*allPid.yawGyro.out + allPid.pitGyro.out);  
						Motor2.out = (throttle.FINAL_OUT - 2*allPid.yawGyro.out + allPid.pitGyro.out);
						Motor3.out = (throttle.FINAL_OUT - 2*allPid.yawGyro.out + allPid.pitGyro.out);  
						Motor4.out = (throttle.FINAL_OUT + 2*allPid.yawGyro.out + allPid.pitGyro.out);
					#elif BRUSHLESS_FOUR_AXIS_UAV
					  Motor1.out = throttle.FINAL_OUT + allPid.pitGyro.out - allPid.rolGyro.out - allPid.yawGyro.out;
            Motor2.out = throttle.FINAL_OUT + allPid.pitGyro.out + allPid.rolGyro.out + allPid.yawGyro.out;   
            Motor3.out = throttle.FINAL_OUT - allPid.pitGyro.out + allPid.rolGyro.out - allPid.yawGyro.out;
            Motor4.out = throttle.FINAL_OUT - allPid.pitGyro.out - allPid.rolGyro.out + allPid.yawGyro.out;
					#endif
        }
        else		//小于起飞油门
        {
            Motor1.out = plane.throttle;  
            Motor2.out = plane.throttle;
            Motor3.out = plane.throttle;
            Motor4.out = plane.throttle;
            
            ClearIntegral(&allPid.pitAngle);         //清除积分
            ClearIntegral(&allPid.pitGyro);          //清除积分        
            ClearIntegral(&allPid.rolAngle);         //清除积分    
            ClearIntegral(&allPid.rolGyro);          //清除积分
            ClearIntegral(&allPid.yawAngle);         //清除积分
            ClearIntegral(&allPid.yawGyro);          //清除积分
        }          
    }
    else		//未解锁
    {
        Motor1.out = 0;
        Motor2.out = 0;
        Motor3.out = 0;
        Motor4.out = 0;
        
        ClearIntegral(&allPid.pitAngle);	//清除积分
        ClearIntegral(&allPid.pitGyro);		//清除积分        
        ClearIntegral(&allPid.rolAngle);	//清除积分    
        ClearIntegral(&allPid.rolGyro);		//清除积分
        ClearIntegral(&allPid.yawAngle);	//清除积分
        ClearIntegral(&allPid.yawGyro);		//清除积分        
    }
  
    PwmOut(Motor1.out,Motor2.out,Motor3.out,Motor4.out);
}


