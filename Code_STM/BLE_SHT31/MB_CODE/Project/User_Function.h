#ifndef USER_FUNCTION_H
#define USER_FUNCTION_H

#include "main.h"
#include "user_delay.h"
#include "nokia_5110.h"
#include "SHT31.h"
#include "myiic.h" 
#include <string.h>
#include <stdio.h>


// Define pin
// Clock
#define LED_CLOCK_A RCC_APB2Periph_GPIOA
#define LED_CLOCK_B RCC_APB2Periph_GPIOB
#define BUZZ_CLOCK	RCC_APB2Periph_GPIOA

#define BT_CLOCK_A	RCC_APB2Periph_GPIOA
#define BT_CLOCK_B	RCC_APB2Periph_GPIOB

// PORT
#define LED_PORT_A	GPIOA					
#define LED_PORT_B	GPIOB
#define BUZZ_PORT		GPIOA

#define BT_PORT_A		GPIOA
#define BT_PORT_B		GPIOB

// Pin
#define LED_1				GPIO_Pin_5			// PB5
#define LED_2				GPIO_Pin_12			// PA11
#define LED_3				GPIO_Pin_11			// PA12
#define LED_4				GPIO_Pin_8			// PB8
#define BUZZ				GPIO_Pin_4			// PA4

#define BT_UP				GPIO_Pin_6			// PA6
#define BT_CENTER		GPIO_Pin_7			// PA7
#define BT_DOWN			GPIO_Pin_8			// PA8
#define BT_RIGHT		GPIO_Pin_2			// PB2			
#define BT_LEFT			GPIO_Pin_3			// PB3			
#define BT_MODE			GPIO_Pin_4			// PB4	

// Mode
#define OUTPUT			GPIO_Mode_Out_PP
#define INPUT				GPIO_Mode_IN_FLOATING

//MY_BOARD
#define MB_BUZZ_CLK		  	RCC_APB2Periph_GPIOA
#define MB_BT_CLOCK_A			RCC_APB2Periph_GPIOA

#define MB_BT_PORT			GPIOA
#define MB_BUZZ_PORT		GPIOA
#define MB_BT_CENTER		GPIO_Pin_5
#define MB_BT_RIGHT			GPIO_Pin_6
#define MB_BT_LEFT			GPIO_Pin_7
#define MB_BUZZ					GPIO_Pin_4



// Khai bao ham
/**************************************************
	Tra ve: Khong
	Tham so: 
		RCC_APB2Periph: LED_CLOCK_A, LED_CLOCK_B...
		GPIOx: LED_PORT_A, LED_PORT_B...
		GPIO_Pin: LED_1, LED_2...
		GPIO_Mode: OUTPUT
***************************************************/
void Init_GPIO(uint32_t RCC_APB2Periph, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode );
void BlinkLeftToRight(void);
void BlinkRightToLeft(void);
void Init_BT_ExInterrupt(void);
void Init_Timer_2(void);
void Init_Timer_3(void);
#endif
