#include "fc_status.h"
#include "parse_packet.h"
#include "precompile.h"

extern PlaneData plane;

//检查锁定状态
void PlaneLockStatus(void)
{
	  //解锁
		#if FOUR_AXIS_UAV
    if(plane.throttle <= 10 && plane.yaw <= -25 && plane.pit >= 25 && plane.rol <= -25){
        plane.unlockCount++;
		}
    if(plane.throttle <= 10 && plane.yaw <= -25 && plane.pit >= 25 && plane.rol <= -25 && plane.unlockCount >= 600){
        plane.unlockCount = 0;
        plane.lock = UNLOCK;		//解锁标志置位，指示灯使用该标志                             
    }
		#elif	FIXED_WING_AIRCRAFT
		if(plane.throttle <= 10 && plane.yaw <= -25){
        plane.unlockCount++;
		}
    if(plane.throttle <= 10 && plane.yaw <= -25 && plane.unlockCount >= 600){
        plane.unlockCount = 0;
        plane.lock = UNLOCK;		//解锁标志置位，指示灯使用该标志                             
    }
		#elif BRUSHLESS_FOUR_AXIS_UAV
		if(plane.throttle <= 10 && plane.yaw <= -25 && plane.pit >= 25 && plane.rol <= -25){
        plane.unlockCount++;
		}
    if(plane.throttle <= 10 && plane.yaw <= -25 && plane.pit >= 25 && plane.rol <= -25 && plane.unlockCount >= 600){
        plane.unlockCount = 0;
        plane.lock = UNLOCK;		//解锁标志置位，指示灯使用该标志                             
    }
		#endif
		
    //上锁
    if(plane.throttle <= 10){
        plane.lockCount++;
		}
    if((plane.throttle <= 10 && plane.lockCount >= 4000) || (plane.throttle <= 10 && plane.yaw >= 25 && plane.lockCount >= 600)){
        plane.lockCount = 0;
        plane.lock = LOCK;		//上锁标志置位，指示灯使用该标志                               
    }
		
		if(plane.throttle > 10){
			plane.lockCount = 0;		//油门大于10要清空计数
		}
}



uint32_t  chip_id[3] = {0};  

//读取芯片ID
void get_chip_id(void)
{
    chip_id[0] = *(__IO u32 *)(0X1FFFF7F0); // 高字节
    chip_id[1] = *(__IO u32 *)(0X1FFFF7EC); //
    chip_id[2] = *(__IO u32 *)(0X1FFFF7E8); // 低字节
}









