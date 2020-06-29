#ifndef __I2C_SHT31_H
#define __I2C_SHT31_H
	 
#ifdef __cplusplus
 extern "C" {
#endif 

#include "main.h"

typedef enum
{
	SHT31_OK = 0,
	SHT31_FAIL
}SHT31_Strtus_Typdef;

typedef enum
{
  SHT31_DMA_TX = 0,
	SHT31_DMA_RX = 1
}SHT31_DMADirection_TypeDef;


#define SHT31_TIMEOUT_UserCallback() 	SHT31_FAIL
#define SHT31_FLAG_TIMEOUT         		((uint32_t)0x1000)
#define SHT31_LONG_TIMEOUT       		  ((uint32_t)(10 * SHT31_FLAG_TIMEOUT))
#define I2C_TIMEOUT         					((uint32_t)0x3FFFF) /*!< I2C Time out */

/* DMA */
#define SHT31_DMA_CLK     						RCC_AHBPeriph_DMA1
#define SHT31_I2C_DR                  ((uint32_t)0x40005410)

#define SHT31_DMA_TX_CHANNEL          DMA1_Channel6
#define SHT31_DMA_RX_CHANNEL          DMA1_Channel7

#define SHT31_DMA_TX_TCFLAG           DMA1_FLAG_TC6
#define SHT31_DMA_RX_TCFLAG     			DMA1_FLAG_TC7


/* I2C */
#define SHT31_I2C					I2C2	
#define SHT31_SDA					GPIO_Pin_11
#define SHT31_SCL 				GPIO_Pin_10
#define SHT31_I2C_PORT		RCC_APB1Periph_I2C2
#define SHT31_GPIO_PORT		RCC_APB2Periph_GPIOB
static const uint8_t LOW_REPEATABILITY_CLOCK_STRETCHING_ENABLE[]		= {0x2C, 0x10};
static const uint8_t HIGH_REPEATABILITY_CLOCK_STRETCHING_ENABLE[]		= {0x2C, 0x06};
static const uint8_t HIGH_REPEATABILITY_CLOCK_STRETCHING_DISABLE[]	=	{0x24, 0x00};

static __IO uint32_t SHT31_Timeout = SHT31_LONG_TIMEOUT;
 

#define SHT31_ADDR      	0x44
#define SHT31_I2C_SPEED		100000

/* Data buffer */
static __IO uint8_t SHT31_BufferRX[6] = {0};

void SHT31_Init (void);
uint8_t* SHT31_ReadTemp(void);
static void SHT31_DMA_Config(SHT31_DMADirection_TypeDef Direction, uint8_t* buffer, uint8_t NumData);

#ifdef __cplusplus
}
#endif

#endif
