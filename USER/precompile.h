#ifndef _precompile_h
#define	_precompile_h
#include "stm32f10x.h"



//����ѡ��ͬʱֻ����һ��Ϊ1
//FOUR_AXIS_UAV		������ı����˻�
//FIXED_WING_AIRCRAFT		�̶������׻�
//BRUSHLESS_FOUR_AXIS_UAV		��ˢ�������˻�
#define FOUR_AXIS_UAV 	1//



#if FOUR_AXIS_UAV

#elif FIXED_WING_AIRCRAFT

#elif BRUSHLESS_FOUR_AXIS_UAV

#endif

#endif

