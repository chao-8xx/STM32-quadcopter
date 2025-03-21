#include "stm32f10x.h"
#include "systick.h"
#include "led.h"
#include "pwm.h"
#include "iic.h"
#include "mpu6050.h"
#include "spi.h"
#include "adc.h"
#include "acc_cal.h"
#include "pid.h"
#include "timer.h"
#include "nvic.h"
#include "usart2.h"
#include "inv_mpu.h"
#include "imu.h"
#include "pid.h"
#include "pair_freq.h"


int main(void)
{
    SystemInit();			//系统初始化    
		SystickInit();		//系统滴答定时器初始化
	
    RGB_LedInit();		//状态灯初始化
		
		Usart2Init(115200);		//串口2初始化
	
		PwmInit();		//pwm初始化
	
		delay_ms(100);		//延时处理，以便开机进行零偏采集，防止瞬间抖动
		IIC_Init();		//初始化IIC总线
		while(MPU6050_Init() != 0){		//mpu6050初始化 
			delay_ms(10);  
			RGB_LedBlink(YELLOW);	
		}
		
		//6050dmp初始化
		while(mpu_dmp_init() != 0){
			RGB_LedBlink(PURPLE);	
			delay_ms(500);
		}

		delay_ms(100);
    SpiInit();
		NRF24L01Init();		//nrf24l01初始化
		while(NRF24L01_Check()){		//nrf24l01在位检测
			delay_ms(50);
			RGB_LedBlink(RED);
		}
    NRF24L01ReceiveMode();		//接收模式
		
		AdcInit();
		
		AllPidInit();		//pid参数初始化
		
		delay_ms(100);
		TimerInit();		//定时器初始化    
    NVIC_config();		//中断配置初始化
		
    while(1){
			VoltageDetect();		//低电压检测
//			LeftButtomLedBlinkSet();		//灯闪烁
    }
}


