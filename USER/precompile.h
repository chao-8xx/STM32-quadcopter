#ifndef _precompile_h
#define	_precompile_h
#include "stm32f10x.h"



//功能选择，同时只能有一个为1
//FOUR_AXIS_UAV		四轴空心杯无人机
//FIXED_WING_AIRCRAFT		固定翼手抛机
//BRUSHLESS_FOUR_AXIS_UAV		无刷四轴无人机
#define FOUR_AXIS_UAV 	1//



#if FOUR_AXIS_UAV

#elif FIXED_WING_AIRCRAFT

#elif BRUSHLESS_FOUR_AXIS_UAV

#endif

#endif

