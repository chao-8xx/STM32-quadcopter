#include "flash.h"
#include "parse_packet.h"


_FLASH_flag flash_flag = {0};
      					
volatile FLASH_Status flashStatus = FLASH_COMPLETE;		//FLASH操作状态变量
#define  start_address  0x08000000 + 1024*60

/* 从内部FLASH中读出连续的Len字节数据 */
/* ReadAddress：数据地址              */
/* ReadBuf ：   数据指针              */
/* ReadLen：    数据大小              */
int flash_read_bytes(uint32_t ReadAddress, uint8_t *ReadBuf, int32_t ReadLen)
{
    int i = 0;
    
    ReadAddress = (uint32_t)start_address + ReadAddress;
    
    while(i < ReadLen)
    {
        *(ReadBuf + i) = *(volatile uint8_t*)ReadAddress++;
        i++;
    }
    return i;
}


/* 向内部FLASH写入一个字(32bits)数据 */
void flash_write_word(uint32_t writeAddr,uint32_t WriteData)
{
	FLASH_UnlockBank1();                                                                //写之前解除闪存锁
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);           //清除错误标志
    flashStatus = FLASH_ErasePage(start_address);                                      //写入前先进行页擦除
	if(flashStatus == FLASH_COMPLETE){                                                  //判断状态位
		flashStatus = FLASH_ProgramWord(start_address + writeAddr, WriteData);      //写入一个字 
	}
	FLASH_LockBank1();                                                                  //上锁
}

/* 向内部FLASH写入半字数据(16bits) */
void flash_write_harfword(uint32_t WriteAddress,uint16_t WriteData)
{
	FLASH_Unlock();                                                                //写之前解除闪存锁
    
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);           //清除错误标志
    flashStatus = FLASH_ErasePage(start_address);                                      //写入前先进行页擦除
	if(flashStatus == FLASH_COMPLETE){                                                  //判断状态位
		flashStatus = FLASH_ProgramHalfWord(start_address + WriteAddress, WriteData);  //写入半字  
	}
	WriteAddress += 2;
	FLASH_Lock();                                                                  //上锁    
}

/* 向FLASH中写入3个浮点数据 */
void flash_write_cal(uint32_t WriteAddress, float WriteData1,float WriteData2,float WriteData3,
                                            float WriteData4,float WriteData5,float WriteData6,
                                            float WriteData7,float WriteData8,float WriteData9)
{
    uint32_t Buf[9] = {0};
    
    Buf[0] = *(uint32_t *)(&WriteData1);
    Buf[1] = *(uint32_t *)(&WriteData2);
    Buf[2] = *(uint32_t *)(&WriteData3);
    Buf[3] = *(uint32_t *)(&WriteData4);
    Buf[4] = *(uint32_t *)(&WriteData5);
    Buf[5] = *(uint32_t *)(&WriteData6);
    Buf[6] = *(uint32_t *)(&WriteData7);
    Buf[7] = *(uint32_t *)(&WriteData8);
    Buf[8] = *(uint32_t *)(&WriteData9);
    
    FLASH_UnlockBank1();                                                                //解锁
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
    flashStatus = FLASH_ErasePage(start_address);                                      //擦除    
    if(flashStatus == FLASH_COMPLETE)                                                  //状态量判断
    {
        /* 进行数据写入 */
        flashStatus = FLASH_ProgramWord(start_address + WriteAddress,Buf[0]);
        flashStatus = FLASH_ProgramWord(start_address + WriteAddress+4,Buf[1]);
        flashStatus = FLASH_ProgramWord(start_address + WriteAddress+8,Buf[2]);  
        flashStatus = FLASH_ProgramWord(start_address + WriteAddress+12,Buf[3]);
        flashStatus = FLASH_ProgramWord(start_address + WriteAddress+16,Buf[4]);
        flashStatus = FLASH_ProgramWord(start_address + WriteAddress+20,Buf[5]);
        flashStatus = FLASH_ProgramWord(start_address + WriteAddress+24,Buf[6]);
        flashStatus = FLASH_ProgramWord(start_address + WriteAddress+28,Buf[7]);
        flashStatus = FLASH_ProgramWord(start_address + WriteAddress+32,Buf[8]);      
    }
    FLASH_LockBank1();                                                                  //上锁
}



/* 从FLASH内存中读出三个float型数据 */
uint16_t flash_read_cal(uint32_t ReadAddress,float *ReadData1,float *ReadData2,float *ReadData3,
                                            float *ReadData4,float *ReadData5,float *ReadData6,
                                            float *ReadData7,float *ReadData8,float *ReadData9)
{ 
    uint8_t   buf[36];
    uint16_t  i = 0;
    uint16_t   flag = 0x0000;
    
    ReadAddress = (uint32_t)start_address + ReadAddress;
    
    *ReadData1 = *(float *)(ReadAddress);
    *ReadData2 = *(float *)(ReadAddress+4);
    *ReadData3 = *(float *)(ReadAddress+8);
    *ReadData4 = *(float *)(ReadAddress+12);
    *ReadData5 = *(float *)(ReadAddress+16);
    *ReadData6 = *(float *)(ReadAddress+20);
    *ReadData7 = *(float *)(ReadAddress+24);
    *ReadData8 = *(float *)(ReadAddress+28);
    *ReadData9 = *(float *)(ReadAddress+32);

    FLASH_LockBank1();                                              

    for(i=0;i<36;i++)    //单字节读取                           
    {
        *(buf+i) = *(volatile uint8_t*)ReadAddress++;
    }
    //是否成功进行判断
    if((buf[0]==0xff&&buf[1]==0xff&&buf[2]==0xff&&buf[3]==0xff))        flag = flag|0x0001;
    if((buf[4]==0xff&&buf[5]==0xff&&buf[6]==0xff&&buf[7]==0xff))        flag = flag|0x0002;
    if((buf[8]==0xff&&buf[9]==0xff&&buf[10]==0xff&&buf[11]==0xff))      flag = flag|0x0004;

    if((buf[12]==0xff&&buf[13]==0xff&&buf[14]==0xff&&buf[15]==0xff))    flag = flag|0x0008;
    if((buf[16]==0xff&&buf[17]==0xff&&buf[18]==0xff&&buf[19]==0xff))    flag = flag|0x0010;
    if((buf[20]==0xff&&buf[21]==0xff&&buf[22]==0xff&&buf[23]==0xff))    flag = flag|0x0020;
    
    if((buf[24]==0xff&&buf[25]==0xff&&buf[26]==0xff&&buf[27]==0xff))    flag = flag|0x0040;
    if((buf[28]==0xff&&buf[29]==0xff&&buf[30]==0xff&&buf[31]==0xff))    flag = flag|0x0080;
    if((buf[32]==0xff&&buf[33]==0xff&&buf[34]==0xff&&buf[35]==0xff))    flag = flag|0x0100;

    return flag;
}


//往flash中写入pid数据
void PidDataWriteToFlash(uint32_t pidWriteAddr, AllPid *pidArr)
{
	u8 arrLen = 0;
	u8 i;
	uint32_t arrTemp[20] = {0};

	arrTemp[arrLen++] = 10;		//添加帧头，判断是否有数据
	
	arrTemp[arrLen++] = *(uint32_t *)&pidArr->rolAngle.kp;
	arrTemp[arrLen++] = *(uint32_t *)&pidArr->rolAngle.ki;
	arrTemp[arrLen++] = *(uint32_t *)&pidArr->rolAngle.kd;
	arrTemp[arrLen++] = *(uint32_t *)&pidArr->pitAngle.kp;
	arrTemp[arrLen++] = *(uint32_t *)&pidArr->pitAngle.ki;
	arrTemp[arrLen++] = *(uint32_t *)&pidArr->pitAngle.kd;
	arrTemp[arrLen++] = *(uint32_t *)&pidArr->yawAngle.kp;
	arrTemp[arrLen++] = *(uint32_t *)&pidArr->yawAngle.ki;
	arrTemp[arrLen++] = *(uint32_t *)&pidArr->yawAngle.kd;

	arrTemp[arrLen++] = *(uint32_t *)&pidArr->rolGyro.kp;
	arrTemp[arrLen++] = *(uint32_t *)&pidArr->rolGyro.ki;
	arrTemp[arrLen++] = *(uint32_t *)&pidArr->rolGyro.kd;
	arrTemp[arrLen++] = *(uint32_t *)&pidArr->pitGyro.kp;
	arrTemp[arrLen++] = *(uint32_t *)&pidArr->pitGyro.ki;
	arrTemp[arrLen++] = *(uint32_t *)&pidArr->pitGyro.kd;
	arrTemp[arrLen++] = *(uint32_t *)&pidArr->yawGyro.kp;
	arrTemp[arrLen++] = *(uint32_t *)&pidArr->yawGyro.ki;
	arrTemp[arrLen++] = *(uint32_t *)&pidArr->yawGyro.kd;
	
	FLASH_UnlockBank1();                                                                //写之前解除闪存锁
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);           //清除错误标志
  flashStatus = FLASH_ErasePage(pidWriteAddr);                                      //写入前先进行页擦除
	for(i = 0; i < arrLen; i++){
		if(flashStatus == FLASH_COMPLETE){                                                  //判断状态位
			flashStatus = FLASH_ProgramWord(pidWriteAddr, arrTemp[i]);      //写入一个字 
		}
		pidWriteAddr += 4;
	}

	FLASH_LockBank1();      
	
}

//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
float STM32_FLASH_ReadHalfWord(u32 faddr)
{
	return *(float*)faddr; 
} 


//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//Num:字节数
void STM32_FLASH_Read(u32 ReadAddr,float *pBuffer,u16 Num)   	
{
	u16 i;
	for( i = 0; i < Num; i++){
		pBuffer[i] = *(float*)ReadAddr;		
		ReadAddr += 4;//偏移4个字节.	
	}
	
}


//从flash中读取pid数据
void PidDataReadFromFlash(uint32_t PidReadAddr,AllPid *pidArr)
{
	float arrTemp[20] = {0};
	uint8_t arrLen = 0;

	STM32_FLASH_Read(PidReadAddr,arrTemp,20);
	
	arrLen++;  //除去帧头

	pidArr->rolAngle.kp = arrTemp[arrLen++];
	pidArr->rolAngle.ki = arrTemp[arrLen++];
	pidArr->rolAngle.kd = arrTemp[arrLen++];
	pidArr->pitAngle.kp = arrTemp[arrLen++];
	pidArr->pitAngle.ki = arrTemp[arrLen++];
	pidArr->pitAngle.kd = arrTemp[arrLen++];
	pidArr->yawAngle.kp = arrTemp[arrLen++];
	pidArr->yawAngle.ki = arrTemp[arrLen++];
	pidArr->yawAngle.kd = arrTemp[arrLen++];
	
	pidArr->rolGyro.kp = arrTemp[arrLen++];
	pidArr->rolGyro.ki = arrTemp[arrLen++];
	pidArr->rolGyro.kd = arrTemp[arrLen++];
	pidArr->pitGyro.kp = arrTemp[arrLen++];
	pidArr->pitGyro.ki = arrTemp[arrLen++];
	pidArr->pitGyro.kd = arrTemp[arrLen++];
	pidArr->yawGyro.kp = arrTemp[arrLen++];
	pidArr->yawGyro.ki = arrTemp[arrLen++];
	pidArr->yawGyro.kd = arrTemp[arrLen++];
	
}
