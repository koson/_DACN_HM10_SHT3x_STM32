#include "main.h"

#define USART_PRINT	USART1

float TEMP_ALERT = 30;
bool ALERT_STATUS = TRUE; 
bool MORE_LESS_COMPARISION = TRUE;
	
float SHT31_Temp=0,SHT31_Humid=0;
uint8_t SHT31_I2C_Data_Buffer[6]={0,0,0,0,0,0};
char Char_buff[10] = {0};
//static uint32_t CYCLE_LENGTH, PREV_TIME = 0;


PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(USART_PRINT, (uint8_t)ch);

	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART_PRINT, USART_FLAG_TC) == RESET)
	{
		//
	}
	return ch;
}

GETCHAR_PROTOTYPE
{
	int c;

	while(USART_GetFlagStatus(USART_PRINT, USART_FLAG_RXNE) == RESET);

	c = USART_ReceiveData(USART_PRINT);

	return fputc(c, f);
}

void TestContrast(uint8_t Contrast)
{
	LCDInit(Contrast);
}


//Configuaration USART for CH340G
void USART1_Configuration(uint32_t Baudrate)
{
	//NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef	GPIO_InitStructure;
	USART_InitTypeDef	USART_InitStructure;
	
	/* Enable GPIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	/* Enable USART2 Clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	
	//PA9 - TX : PA10 - RX
	/* Configure USART2 Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART2 Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USART configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
	
	USART_InitStructure.USART_BaudRate = Baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

	/* Configure USART2 */
	USART_Init(USART1, &USART_InitStructure);
	
	USART_ClearFlag(USART1, USART_FLAG_RXNE); 
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	/* Enable USART2 */
	USART_Cmd(USART1, ENABLE);
	
	/* Enable USART2 global interrupt */
//	NVIC_EnableIRQ(USART2_IRQn);
}

void USART2_Configuration(uint32_t Baudrate)
{
	//NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef	GPIO_InitStructure;
	USART_InitTypeDef	USART_InitStructure;
	
	/* Enable GPIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	/* Enable USART2 Clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	
	//PA2 - TX : PA3 - RX
	/* Configure USART2 Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure USART2 Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USART configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
	USART_InitStructure.USART_BaudRate = Baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

	/* Configure USART2 */
	USART_Init(USART2, &USART_InitStructure);
	
	USART_ClearFlag(USART2, USART_FLAG_RXNE); 
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

	/* Enable USART2 */
	USART_Cmd(USART2, ENABLE);
	
	/* Enable USART2 global interrupt */
//	NVIC_EnableIRQ(USART2_IRQn);
}

void USART3_Configuration(uint32_t Baudrate){
		//NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef	GPIO_InitStructure;
	USART_InitTypeDef	USART_InitStructure;
	
	/* Enable GPIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	/* Enable USART3 Clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	
	//PB10 - TX : PB11 - RX
	/* Configure USART3 Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure USART3 Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* USART configured as follow:
        - BaudRate = 9600 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
	USART_InitStructure.USART_BaudRate = Baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

	/* Configure USART3 */
	USART_Init(USART3, &USART_InitStructure);
	
	USART_ClearFlag(USART3, USART_FLAG_RXNE); 
//	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	/* Enable USART3 */
	USART_Cmd(USART3, ENABLE);
	
	/* Enable USART3 global interrupt */
//	NVIC_EnableIRQ(USART3_IRQn);
}

/**
* @brief 
* sends SHT31_I2C_Data_Buffer out without dma using polling 
* 
* @note 
* @param SHT31_I2C_Data_Buffer SHT31_I2C_Data_Buffer to send data from 
* @param size the size of the SHT31_I2C_Data_Buffer, must not be greater than the SHT31_I2C_Data_Buffer!
* @retval none
*/
void sendSHT31_I2C_Data_Buffer(uint8_t* SHT31_I2C_Data_Buffer, int size)
{
	int i=0;
	// todo convert size to ascii 
	for(i=0; i<size; i++)
	{
		// wait for it to go out
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
		USART_SendData(USART2, SHT31_I2C_Data_Buffer[i]);
	}
}

void LCD_Interface(void){
	char buf_SHT31_Temp[20] =  "Temp :";
	char buf_SHT31_Humid[20] = "Humid:";
  char buf_ALERT_TEMP[20] = "WHEN T";
	char buf_ALERT_STATUS[4] = "A:";
		
	LCDClear();
	gotoXY(13,0);
	LCDString("BLE_SHT31");
	
	sprintf(Char_buff, "%4.2fC", SHT31_Temp);
	strcat(buf_SHT31_Temp,Char_buff); 
	gotoXY(0, 1);
	LCDString(buf_SHT31_Temp);
	
	sprintf(Char_buff, "%4.2f%%", SHT31_Humid);
	strcat(buf_SHT31_Humid,Char_buff); 
	gotoXY(0, 2);
	LCDString(buf_SHT31_Humid);
	
	if(MORE_LESS_COMPARISION == TRUE){
		strcat(buf_ALERT_TEMP, ">");
	}
	else strcat(buf_ALERT_TEMP, "<");
	
	sprintf(Char_buff, "%4.1f?", TEMP_ALERT);
	strcat(buf_ALERT_TEMP,Char_buff); 
	gotoXY(0, 3);
	LCDString(buf_ALERT_TEMP);	
	
	sprintf(Char_buff, "%d", ALERT_STATUS);
	strcat(buf_ALERT_STATUS,Char_buff); 
	gotoXY(0, 4);
	LCDString(buf_ALERT_STATUS);
}

void SHT31_TempThreshHold(void){
	static uint8_t alert_state = 0;
	if(ALERT_STATUS == TRUE){
		if(MORE_LESS_COMPARISION == TRUE){	
			if(SHT31_Temp >= TEMP_ALERT){	
				if(alert_state == 0){
					gotoXY(20, 5);
					LCDString("<ALERT>");
					GPIO_ResetBits(BUZZ_PORT, BUZZ);
					GPIO_ResetBits(GPIOA, LED_2|LED_3);
					alert_state = 1;
				}
				else if(alert_state == 1){
					gotoXY(20, 5);
					LCDString("_______");
					GPIO_SetBits(BUZZ_PORT, BUZZ);
					GPIO_SetBits(GPIOA, LED_2|LED_3);
					alert_state = 0;
				}
			}
			else if(SHT31_Temp < TEMP_ALERT){
				GPIO_SetBits(BUZZ_PORT, BUZZ);
				GPIO_SetBits(GPIOA, LED_2|LED_3);
			}
		}
		
		else if(MORE_LESS_COMPARISION == FALSE) {
			if(SHT31_Temp < TEMP_ALERT){				
				if(alert_state == 0){
					gotoXY(20, 5);
					LCDString("<ALERT>");
					GPIO_ResetBits(BUZZ_PORT, BUZZ);
					GPIO_ResetBits(GPIOA, LED_2|LED_3);
					alert_state = 1;
				}
				else if(alert_state == 1){
					gotoXY(20, 5);
					LCDString("_______");
					GPIO_SetBits(BUZZ_PORT, BUZZ);
					GPIO_SetBits(GPIOA, LED_2|LED_3);
					alert_state = 0;
				}
			}
			else if(SHT31_Temp > TEMP_ALERT){
				GPIO_SetBits(BUZZ_PORT, BUZZ);
				GPIO_SetBits(GPIOA, LED_2|LED_3);
			}
		}
	}	
	else{
		GPIO_SetBits(BUZZ_PORT, BUZZ);
		GPIO_SetBits(GPIOA, LED_2|LED_3);
	}
}


void SHT31_GetData(uint8_t* COMMAND){
	IIC_Write(SHT31_ADDR<<1,COMMAND[0],COMMAND[1]);
	delay_ms(300);
	IIC_ReadMulti(SHT31_ADDR<<1,0x00,sizeof(SHT31_I2C_Data_Buffer),SHT31_I2C_Data_Buffer);
	
	SHT31_Temp=(float) (((SHT31_I2C_Data_Buffer[0]<<8)|SHT31_I2C_Data_Buffer[1])*175)/65535-45;
	SHT31_Humid=(float) (((SHT31_I2C_Data_Buffer[3]<<8)|SHT31_I2C_Data_Buffer[4])*100)/65535;
}



int main(void)
{
	uint16_t c;
	
//System Clock Initialize
	SystemInit(); // Cau hinh Clock he thong 72Mhz
	SysTick_Config(SystemCoreClock / 1000);
	delay_init(72); // Khoi tao thu vien user_delay
	
//	Interrupt Group Config
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
//Led and Buzz Initialize
	Init_GPIO(LED_CLOCK_A, LED_PORT_A, LED_2|LED_3, OUTPUT);
	Init_GPIO(LED_CLOCK_B, LED_PORT_B, LED_1, OUTPUT);
	Init_GPIO(BUZZ_CLOCK, BUZZ_PORT, BUZZ, OUTPUT);
	Init_GPIO(BT_CLOCK_A, BT_PORT_A, BT_UP|BT_CENTER|BT_DOWN, INPUT);
	Init_BT_ExInterrupt();
	
//	Init_GPIO(MB_BT_CLOCK_A, MB_BT_PORT, MB_BT_CENTER|MB_BT_LEFT|MB_BT_RIGHT, INPUT);
//	Init_GPIO(MB_BUZZ_CLK, MB_BUZZ_PORT, MB_BUZZ, OUTPUT);
	
	// Khoi tao LCD
	LCDInit(0x20);// Tuy mach thi contrast se khac nhau
	
//USART Config
	USART2_Configuration(9600);
	USART1_Configuration(9600);
	USART3_Configuration(9600);
	//printf("Start\n");
	
//Timer2 Configuration
	Init_Timer_2();

//	SHT31_configuration
	IIC_Init();
	
	delay_ms(1000);
	
	while(1)
	{

		SHT31_GetData((uint8_t*)HIGH_REPEATABILITY_CLOCK_STRETCHING_ENABLE);
		LCD_Interface();
		SHT31_TempThreshHold();
	
		if(SHT31_Humid>80) USART_SendData(USART1, 0x01);
		else if(SHT31_Humid<80) USART_SendData(USART1, 0x00);
		
		
	
//Do an
//		while(USART_GetFlagStatus(USART_PRINT, USART_FLAG_RXNE) == SET){
//			c = USART_ReceiveData(USART1); 
//			printf("%d", c);
//			if(c == '1') GPIO_ResetBits(LED_PORT_B, LED_1);
//			if(c == '0') GPIO_SetBits(LED_PORT_B, LED_1);
//		}
//		printf("\r\n%4.2f", SHT31_Temp);

//Nhung
//		while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET){
//			c = USART_ReceiveData(USART2);
//			USART_SendData(USART3, c);
//			if(c == 0x01) GPIO_ResetBits(LED_PORT_B, LED_1);
//			if(c == 0x00) GPIO_SetBits(LED_PORT_B, LED_1);
//			break;
//		}
//		
		delay_ms(300);
	}
}
