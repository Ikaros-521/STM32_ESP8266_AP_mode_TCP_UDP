#include "sys.h"
#include "delay.h"
#include "step.h"

//IN4: PF4  d
//IN3: PF3  c
//IN2: PF2  b
//IN1: PF1  a

u8 forward[4] = {0x03,0x06,0x0c,0x09}; // 正转
u8 reverse[4]= {0x03,0x09,0x0c,0x06}; // 反转

//引脚初始化
void Step_Motor_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_1;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
}

//引脚映射
void SetMotor(unsigned char InputData)
{
	if(InputData == 0x03)
	{
		GPIO_SetBits(GPIOF,GPIO_Pin_1);
		GPIO_SetBits(GPIOF,GPIO_Pin_2);
		GPIO_ResetBits(GPIOF,GPIO_Pin_3);
		GPIO_ResetBits(GPIOF,GPIO_Pin_4);
	}
	else if(InputData == 0x06)
	{
		GPIO_ResetBits(GPIOF,GPIO_Pin_1);
		GPIO_SetBits(GPIOF,GPIO_Pin_2);
		GPIO_SetBits(GPIOF,GPIO_Pin_3);
		GPIO_ResetBits(GPIOF,GPIO_Pin_4);
	}
	else if(InputData == 0x09)
	{
		GPIO_SetBits(GPIOF,GPIO_Pin_1);
		GPIO_ResetBits(GPIOF,GPIO_Pin_2);
		GPIO_ResetBits(GPIOF,GPIO_Pin_3);
		GPIO_SetBits(GPIOF,GPIO_Pin_4);
	}
	else if(InputData == 0x0c)
	{	
		GPIO_ResetBits(GPIOF,GPIO_Pin_1);
		GPIO_ResetBits(GPIOF,GPIO_Pin_2);
		GPIO_SetBits(GPIOF,GPIO_Pin_3);
		GPIO_SetBits(GPIOF,GPIO_Pin_4);
	}
	else if(InputData == 0x00)
	{
		GPIO_ResetBits(GPIOF,GPIO_Pin_1);
		GPIO_ResetBits(GPIOF,GPIO_Pin_2);
		GPIO_ResetBits(GPIOF,GPIO_Pin_3);
		GPIO_ResetBits(GPIOF,GPIO_Pin_4);
	}
}

/*
	功能：转1/64圈
	步距角5.625 360/5.625=64 减速比1/64
	故64*64个脉冲转一圈
	n 圈数
	direction 方向 1正转 非1反转
	delay delay时长ms >= 2
*/
void motor_circle(int n, int direction, int delay)
{
    int i, j;
    for(i = 0; i < n * 8; i++)
    {
		for(j = 0; j < 4; j++)
		{
			if(1 == direction)
			{
				SetMotor(0x00);
				SetMotor(forward[j]);
			}
			else
			{
				SetMotor(0x00);
				SetMotor(reverse[j]);
			}
			
			delay_ms(delay > 2 ? delay : 2);
		}
    }
}
