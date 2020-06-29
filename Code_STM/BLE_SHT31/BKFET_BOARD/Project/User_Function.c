#include "User_Function.h"

/**************************************************
	Tra ve: Khong
	Tham so: 
		RCC_APB2Periph: LED_CLOCK_A, LED_CLOCK_B...
		GPIOx: LED_PORT_A, LED_PORT_B...
		GPIO_Pin: LED_1, LED_2...
		GPIO_Mode: OUTPUT
***************************************************/
void Init_GPIO(uint32_t RCC_APB2Periph, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode )
{
	GPIO_InitTypeDef GPIO_Structure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph,ENABLE);		// Clock
	
	GPIO_Structure.GPIO_Pin 	= GPIO_Pin;							// Pin
	GPIO_Structure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Structure.GPIO_Mode	= GPIO_Mode; 						// Mode
	
	GPIO_Init(GPIOx, &GPIO_Structure); 								// PORT
	
	GPIO_SetBits(GPIOx, GPIO_Pin);										// Gia tri ban dau
	
}

void Init_BT_ExInterrupt(void){
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_Structure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);	
	GPIO_Structure.GPIO_Pin= BT_UP|BT_CENTER|BT_DOWN;
	GPIO_Structure.GPIO_Mode= GPIO_Mode_IPU; 	
	GPIO_Init(BT_PORT_A,&GPIO_Structure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6|GPIO_PinSource7|GPIO_PinSource8);//Connect Interrupt
	
	/* Configure EXTI line */
	EXTI_InitStructure.EXTI_Line = EXTI_Line6|EXTI_Line7|EXTI_Line8;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/* Enable and set EXTI0 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}


void BlinkLeftToRight(void)
{
	// BAT LED 1
	GPIO_ResetBits(LED_PORT_B, LED_1);
	GPIO_SetBits(LED_PORT_A, LED_2);	
	GPIO_SetBits(LED_PORT_A, LED_3);	
	GPIO_SetBits(LED_PORT_B, LED_4);
	
	delay_ms(100); 
	
	// Bat LED 2
	GPIO_SetBits(LED_PORT_B, LED_1);
	GPIO_ResetBits(LED_PORT_A, LED_2);	
	GPIO_SetBits(LED_PORT_A, LED_3);	
	GPIO_SetBits(LED_PORT_B, LED_4);
	
	delay_ms(100); 
	
	// Bat LED 3
	GPIO_SetBits(LED_PORT_B, LED_1);
	GPIO_SetBits(LED_PORT_A, LED_2);	
	GPIO_ResetBits(LED_PORT_A, LED_3);	
	GPIO_SetBits(LED_PORT_B, LED_4);
	
	delay_ms(100);
	
	// Bat LED 4
	GPIO_SetBits(LED_PORT_B, LED_1);
	GPIO_SetBits(LED_PORT_A, LED_2);	
	GPIO_SetBits(LED_PORT_A, LED_3);	
	GPIO_ResetBits(LED_PORT_B, LED_4);
	
	delay_ms(100); 
}

void BlinkRightToLeft(void)
{
	// Bat LED 4
	GPIO_SetBits(LED_PORT_B, LED_1);
	GPIO_SetBits(LED_PORT_A, LED_2);	
	GPIO_SetBits(LED_PORT_A, LED_3);	
	GPIO_ResetBits(LED_PORT_B, LED_4);
	
	delay_ms(100); 
	
	// Bat LED 3
	GPIO_SetBits(LED_PORT_B, LED_1);
	GPIO_SetBits(LED_PORT_A, LED_2);	
	GPIO_ResetBits(LED_PORT_A, LED_3);	
	GPIO_SetBits(LED_PORT_B, LED_4);
	
	delay_ms(100);
	
	// Bat LED 2
	GPIO_SetBits(LED_PORT_B, LED_1);
	GPIO_ResetBits(LED_PORT_A, LED_2);	
	GPIO_SetBits(LED_PORT_A, LED_3);	
	GPIO_SetBits(LED_PORT_B, LED_4);
	
	delay_ms(100); 
	
	// BAT LED 1
	GPIO_ResetBits(LED_PORT_B, LED_1);
	GPIO_SetBits(LED_PORT_A, LED_2);	
	GPIO_SetBits(LED_PORT_A, LED_3);	
	GPIO_SetBits(LED_PORT_B, LED_4);
	
	delay_ms(100); 
}

//Khoi tao Timer2
void Init_Timer_2(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM2_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM2_TimeBaseStructure.TIM_Prescaler = 71; // frequency = 1000000
	TIM2_TimeBaseStructure.TIM_Period = 1000 - 1;	//Count to 1000 and restart to count from 0, or timer event every 1ms
	TIM2_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM2_TimeBaseStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);   
}

