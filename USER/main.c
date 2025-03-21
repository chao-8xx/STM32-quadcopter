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
    SystemInit();			//ϵͳ��ʼ��    
		SystickInit();		//ϵͳ�δ�ʱ����ʼ��
	
    RGB_LedInit();		//״̬�Ƴ�ʼ��
		
		Usart2Init(115200);		//����2��ʼ��
	
		PwmInit();		//pwm��ʼ��
	
		delay_ms(100);		//��ʱ�����Ա㿪��������ƫ�ɼ�����ֹ˲�䶶��
		IIC_Init();		//��ʼ��IIC����
		while(MPU6050_Init() != 0){		//mpu6050��ʼ�� 
			delay_ms(10);  
			RGB_LedBlink(YELLOW);	
		}
		
		//6050dmp��ʼ��
		while(mpu_dmp_init() != 0){
			RGB_LedBlink(PURPLE);	
			delay_ms(500);
		}

		delay_ms(100);
    SpiInit();
		NRF24L01Init();		//nrf24l01��ʼ��
		while(NRF24L01_Check()){		//nrf24l01��λ���
			delay_ms(50);
			RGB_LedBlink(RED);
		}
    NRF24L01ReceiveMode();		//����ģʽ
		
		AdcInit();
		
		AllPidInit();		//pid������ʼ��
		
		delay_ms(100);
		TimerInit();		//��ʱ����ʼ��    
    NVIC_config();		//�ж����ó�ʼ��
		
    while(1){
			VoltageDetect();		//�͵�ѹ���
//			LeftButtomLedBlinkSet();		//����˸
    }
}


