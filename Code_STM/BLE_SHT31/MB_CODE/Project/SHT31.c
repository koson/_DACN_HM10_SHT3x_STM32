#include "SHT31.h"
#include "user_delay.h"



static void SHT31_DMA_Config(SHT31_DMADirection_TypeDef Direction, uint8_t* buffer, uint8_t NumData){
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(SHT31_DMA_CLK, ENABLE);
	
	/* Initialize the DMA_PeripheralBaseAddr member */
	DMA_InitStructure.DMA_PeripheralBaseAddr = SHT31_I2C_DR;
	/* Initialize the DMA_MemoryBaseAddr member */
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)buffer;
	/* Initialize the DMA_PeripheralInc member */
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	/* Initialize the DMA_MemoryInc member */
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	/* Initialize the DMA_PeripheralDataSize member */
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	/* Initialize the DMA_MemoryDataSize member */
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	/* Initialize the DMA_Mode member */
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	/* Initialize the DMA_Priority member */
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	/* Initialize the DMA_M2M member */
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	/* If using DMA for Reception */
  if (Direction == SHT31_DMA_RX)
  {
    /* Initialize the DMA_DIR member */
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    
    /* Initialize the DMA_BufferSize member */
    DMA_InitStructure.DMA_BufferSize = NumData;
    
    DMA_DeInit(SHT31_DMA_RX_CHANNEL);
    
    DMA_Init(SHT31_DMA_RX_CHANNEL, &DMA_InitStructure);
  }
   /* If using DMA for Transmission */
  else if (Direction == SHT31_DMA_TX) { 
    /* Initialize the DMA_DIR member */
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    
    /* Initialize the DMA_BufferSize member */
    DMA_InitStructure.DMA_BufferSize = NumData;
    
    DMA_DeInit(SHT31_DMA_TX_CHANNEL);
    
    DMA_Init(SHT31_DMA_TX_CHANNEL, &DMA_InitStructure);
  }
}

void SHT31_Init(void){
	I2C_InitTypeDef	  I2C_InitStructure; 
	GPIO_InitTypeDef 	GPIO_InitStructure;
	
	I2C_DeInit(SHT31_I2C);

//Configure SCL and SDA Pin Clock, I2C1 clock
	RCC_APB1PeriphClockCmd(SHT31_I2C_PORT, ENABLE);							// su dung kenh I2C1 cua STM32
	RCC_APB2PeriphClockCmd(SHT31_GPIO_PORT, ENABLE);

//Confugure SDA and SCL Pin Used
	GPIO_InitStructure.GPIO_Pin = SHT31_SCL | SHT31_SDA;						//PB6 - SCL, PB7 - SDA
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

//SHT31_I2C Configuration
	I2C_InitStructure.I2C_Mode = I2C_Mode_SMBusHost;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00; // 
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;       
	I2C_InitStructure.I2C_ClockSpeed = SHT31_I2C_SPEED;										
	I2C_Init(SHT31_I2C, &I2C_InitStructure);
	
//Enable SMBus Alert interrupt
	I2C_ITConfig(SHT31_I2C, I2C_IT_ERR, ENABLE);
	
//Init I2C
	I2C_Cmd(SHT31_I2C, ENABLE);
}	

/* 
Descrip	: Read SHT31 6 continuous byte using DMA
param		:	None
ret			: pointer to an 6 byte array of data
*/

uint8_t* SHT31_ReadTemp(void)
{
//	uint16_t SHT31_Temp = 0;
	/* Test on BUSY Flag */
	memset((void *)SHT31_BufferRX, 0, sizeof(SHT31_BufferRX));
	
	SHT31_Timeout = SHT31_LONG_TIMEOUT;
	while (I2C_GetFlagStatus(SHT31_I2C,I2C_FLAG_BUSY)) 
	{
		if((SHT31_Timeout--) == 0) return 0;
	}

	/* Configure DMA Peripheral */
	SHT31_DMA_Config(SHT31_DMA_RX, (uint8_t*)SHT31_BufferRX, sizeof(SHT31_BufferRX));  

	/* Enable DMA NACK automatic generation */
	I2C_DMALastTransferCmd(SHT31_I2C, ENABLE);
	
	I2C_GenerateSTART(SHT31_I2C, ENABLE);
	
	/* Test on SB Flag */
	SHT31_Timeout = SHT31_FLAG_TIMEOUT;
	while(!I2C_GetFlagStatus(SHT31_I2C, I2C_FLAG_SB)){
		if((SHT31_Timeout--) == 0) return 0;
	}
	
	I2C_Send7bitAddress(SHT31_I2C, SHT31_ADDR, I2C_Direction_Transmitter);
	
	/* Test on ADDR Flag */
	SHT31_Timeout = SHT31_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(SHT31_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
		if((SHT31_Timeout--) == 0) return 0;
	}
	
	/* Set Data Reading mode */
	I2C_SendData(SHT31_I2C, HIGH_REPEATABILITY_CLOCK_STRETCHING_DISABLE[0]);
	SHT31_Timeout = SHT31_FLAG_TIMEOUT;
	while((!I2C_GetFlagStatus(SHT31_I2C, I2C_FLAG_TXE)) && (!I2C_GetFlagStatus(SHT31_I2C, I2C_FLAG_BTF))){
		if((SHT31_Timeout--) == 0) return 0;
	}
	
	I2C_SendData(SHT31_I2C, HIGH_REPEATABILITY_CLOCK_STRETCHING_DISABLE[1]);
	SHT31_Timeout = SHT31_FLAG_TIMEOUT;
	while((!I2C_GetFlagStatus(SHT31_I2C, I2C_FLAG_TXE)) && (!I2C_GetFlagStatus(SHT31_I2C, I2C_FLAG_BTF))){
		if((SHT31_Timeout--) == 0) return 0;
	}
	
	
	I2C_GenerateSTART(SHT31_I2C, ENABLE);
	
	/* Test on SB Flag */
	SHT31_Timeout = SHT31_FLAG_TIMEOUT;
	while(!I2C_GetFlagStatus(SHT31_I2C, I2C_FLAG_SB)){
		if((SHT31_Timeout--) == 0) return 0;
	}
	
	I2C_Send7bitAddress(SHT31_I2C, SHT31_ADDR, I2C_Direction_Transmitter);
	
	/* Test on ADDR Flag */
	SHT31_Timeout = SHT31_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(SHT31_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
		if((SHT31_Timeout--) == 0) return 0;
	}

	I2C_GenerateSTOP(SHT31_I2C, ENABLE);
	
	delay_ms(16);
	
	/* Enable DMA RX Channel */
	DMA_Cmd(SHT31_DMA_RX_CHANNEL, ENABLE);
	
	/* Wait until DMA Transfer Complete */
	SHT31_Timeout = SHT31_LONG_TIMEOUT;
	while (!DMA_GetFlagStatus(SHT31_DMA_RX_TCFLAG))
	{
		if((SHT31_Timeout--) == 0) return 0;
	}       
	
	/* Send STOP Condition */
	I2C_GenerateSTOP(SHT31_I2C, ENABLE);

	 /* Disable DMA RX Channel */
  DMA_Cmd(SHT31_DMA_RX_CHANNEL, DISABLE);

	/* Disable I2C DMA request */  
	I2C_DMACmd(SHT31_I2C,DISABLE);

	/* Clear DMA RX Transfer Complete Flag */
	DMA_ClearFlag(SHT31_DMA_RX_TCFLAG);
	
	/* Parse Received Data */
//	SHT31_Temp = (SHT31_BufferRX[0]<<8) + SHT31_BufferRX[1];
	
	/*!< Return Temperature value */
	return (uint8_t *)SHT31_BufferRX;
}

uint8_t SHT31_CRCCheck(uint8_t *data_frame)
{
	uint8_t bit;
	uint8_t crc = 0xFF; // calculated checksum
	uint8_t byteCtr;    // byte counter

	// calculates 8-Bit checksum with given polynomial
	for(byteCtr = 0; byteCtr < 2; byteCtr++) {
		crc ^= (data_frame[byteCtr]);
		for(bit = 8; bit > 0; --bit) {
		  if(crc & 0x80) {
			crc = (crc << 1) ^ CRC_POLYNOMIAL;
		  } else {
			crc = (crc << 1);
		  }
		}
	}
	
	if(crc != *(data_frame+2)) return 0;
	else return 1;
}





