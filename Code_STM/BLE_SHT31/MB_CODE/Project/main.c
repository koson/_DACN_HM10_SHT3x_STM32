#include "main.h"
#include "stm32f10x_it.h"

#define USART_PRINT	USART2

uint16_t DATA_SENT_FREQUENCY = 1000;
uint16_t LCD_UPDATE_FREQUENCY = 100;
uint16_t THRESHHOLD_FREQUENCY = 300;

float SHT31_Temp=0,SHT31_Humid=0;
uint8_t SHT31_I2C_Data_Buffer[6]={0,0,0,0,0,0};

float TEMP_ALERT = 32.50;
bool ALERT_STATUS = TRUE; 
bool MORE_LESS_COMPARISION = TRUE;

char Char_buff[10] = {0};
static uint32_t CYCLE_LENGTH, PREV_TIME, PREV_TIME_LCD, PREV_THRESHHOLD_TIME;
static char freq_setup = 0;

char byte_received = 0;
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
	uint8_t ch = 0;
	
	USART_ClearFlag(USART_PRINT, USART_FLAG_ORE);
	while(USART_GetFlagStatus(USART_PRINT, USART_FLAG_RXNE) == RESET);

	ch = USART_ReceiveData(USART_PRINT);

	return ch;
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
	char buf_SHT31_Temp[20] =  "T:";
	char buf_SHT31_Humid[20] = "H:";
  char buf_ALERT_TEMP[20] = "T";
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
	gotoXY(64, 3);
	LCDString(buf_ALERT_STATUS);
	
	sprintf(Char_buff, "%1.1f", DATA_SENT_FREQUENCY/1000.0);
	gotoXY(64, 2);
	LCDString(Char_buff);
}

void SHT31_TempThreshHold(void){
	static uint8_t alert_state = 0;
	if(ALERT_STATUS == TRUE){
		if(MORE_LESS_COMPARISION == TRUE){	
			if(SHT31_Temp >= TEMP_ALERT){	
				if(alert_state == 0){
					gotoXY(20, 5);
					LCDString("<ALERT>");
					GPIO_ResetBits(MB_BUZZ_PORT, MB_BUZZ);
					alert_state = 1;
				}
				else if(alert_state == 1){
					gotoXY(20, 5);
					LCDString("_______");
					GPIO_SetBits(MB_BUZZ_PORT, MB_BUZZ);
					alert_state = 0;
				}
			}
			else if(SHT31_Temp < TEMP_ALERT){
				GPIO_SetBits(MB_BUZZ_PORT, MB_BUZZ);
			}
		}
		
		else if(MORE_LESS_COMPARISION == FALSE) {
			if(SHT31_Temp < TEMP_ALERT){				
				if(alert_state == 0){
					gotoXY(20, 5);
					LCDString("<ALERT>");
					GPIO_ResetBits(MB_BUZZ_PORT, MB_BUZZ);
					alert_state = 1;
				}
				else if(alert_state == 1){
					gotoXY(20, 5);
					LCDString("_______");
					GPIO_SetBits(MB_BUZZ_PORT, MB_BUZZ);
					alert_state = 0;
				}
			}
			else if(SHT31_Temp > TEMP_ALERT){
				GPIO_SetBits(MB_BUZZ_PORT, MB_BUZZ);
			}
		}
	}	
	else{
		GPIO_SetBits(MB_BUZZ_PORT, MB_BUZZ);
	}
}


void SHT31_GetData(uint8_t* COMMAND){
	IIC_Write(SHT31_ADDR<<1,COMMAND[0],COMMAND[1]);
	delay_ms(50);
	IIC_ReadMulti(SHT31_ADDR<<1,0x00,sizeof(SHT31_I2C_Data_Buffer),SHT31_I2C_Data_Buffer);
	
	SHT31_Temp=(float) (((SHT31_I2C_Data_Buffer[0]<<8)|SHT31_I2C_Data_Buffer[1])*175)/65535-45;
	SHT31_Humid=(float) (((SHT31_I2C_Data_Buffer[3]<<8)|SHT31_I2C_Data_Buffer[4])*100)/65535;
}

void testBUZZ(void){
	GPIO_SetBits(MB_BUZZ_PORT, MB_BUZZ);
	delay_ms(500);
	GPIO_ResetBits(MB_BUZZ_PORT, MB_BUZZ);
	delay_ms(500);
}

void testBUTTON(void){
	if(GPIO_ReadInputDataBit(MB_BT_PORT, MB_BT_LEFT) == 0){
		gotoXY(0,0);
		LCDString("BT_LEFT Pushed");
	}
	else if(GPIO_ReadInputDataBit(MB_BT_PORT, MB_BT_LEFT) == 1){
		gotoXY(0,0);
		LCDString("BT_LEFT not pushed");
	}
	if(GPIO_ReadInputDataBit(MB_BT_PORT, MB_BT_CENTER) == 0){
		gotoXY(0,2);
		LCDString("BT_CENTER Pushed");
	}
	else if(GPIO_ReadInputDataBit(MB_BT_PORT, MB_BT_CENTER) == 1){
		gotoXY(0,2);
		LCDString("BT_CENTER not pushed");
	}
	if(GPIO_ReadInputDataBit(MB_BT_PORT, MB_BT_RIGHT) == 0){
		gotoXY(0,4);
		LCDString("BT_RIGHT Pushed");
	}
	else if(GPIO_ReadInputDataBit(MB_BT_PORT, MB_BT_RIGHT) == 1){
		gotoXY(0,4);
		LCDString("BT_RIGHT not pushed");
	}
}

void testBLE(void){
	printf("%4.2f", 29.92);
}

char GetDataFromPhone(void){
	char ReceivedDataFromPhone[20] = "Last Data: ";
	char c;
	if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET){
		while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET){
			c = USART_ReceiveData(USART2); 
			byte_received = c;	
		}
	}
	sprintf(Char_buff, "%c", byte_received);
	strcat(ReceivedDataFromPhone, Char_buff); 
	gotoXY(0,4);
	LCDString(ReceivedDataFromPhone);
	return c;
	
//	else
//	{
//		gotoXY(0,4);
//		LCDString("R_Data: None");
//		return 0;
//	}
}

void CalculateProcessingTime(void){
	CYCLE_LENGTH = millis()- PREV_TIME;
	CYCLE_LENGTH = CYCLE_LENGTH*2.7322;
}

void ReconfigByUser(void){
	switch(freq_setup){
		case '1': 
			DATA_SENT_FREQUENCY = 200;	//1s
			break;
		case '2':
			DATA_SENT_FREQUENCY = 500;	//1.5s
			break;
		case '3': 
			DATA_SENT_FREQUENCY = 1000;	
			break;
		case '4':
			DATA_SENT_FREQUENCY = 1500;
			break;
		case '5': 
			DATA_SENT_FREQUENCY = 2000;	
			break;
		case '6':
			DATA_SENT_FREQUENCY = 5000;
			break;
		case '7': 
			DATA_SENT_FREQUENCY = 10000;
			break;
		case '8':
			DATA_SENT_FREQUENCY = 15000;
			break;
		case '9':
			DATA_SENT_FREQUENCY = 30000;
			break;
		case '+':	//increase temp alert
			TEMP_ALERT += 0.1;
			if(TEMP_ALERT>40) TEMP_ALERT = 40;
			break;
		case '-':	//decrease temp alert
			TEMP_ALERT -= 0.1;
			if(TEMP_ALERT<20) TEMP_ALERT = 20;
			break;
		case 'c':	//change temp compare mode
			MORE_LESS_COMPARISION = !MORE_LESS_COMPARISION;
			break;
		case 'a': //toggle alert status
			ALERT_STATUS = !ALERT_STATUS;
		default:
			break;
	}
}

int main(void)
{
//System Clock Initialize
	SystemInit(); // Cau hinh Clock he thong 72Mhz
	delay_init(24); // Khoi tao thu vien user_delay
	
//	Interrupt Group Config
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
//Button and BUZZ Initialize
	Init_GPIO(MB_BT_CLOCK_A, MB_BT_PORT, MB_BT_CENTER|MB_BT_LEFT|MB_BT_RIGHT, INPUT);
	Init_GPIO(MB_BUZZ_CLK, MB_BUZZ_PORT, MB_BUZZ, OUTPUT);
	Init_BT_ExInterrupt();
	
	// Khoi tao LCD
	LCDInit(0x3F); // Tuy mach thi contrast se khac nhau 
//USART Config
	USART2_Configuration(9600);
//	USART1_Configuration(9600);
//	USART3_Configuration(9600);
	
	
//Timer2 Configuration
	Init_Timer_2();
	Init_Timer_3();
//	SHT31_configuration
	IIC_Init();
	delay_ms(1000);
	PREV_TIME = millis();
	PREV_TIME_LCD = millis();
	PREV_THRESHHOLD_TIME = millis();
	while(1)
	{
		
		if(((millis()- PREV_TIME)*2.7322)>DATA_SENT_FREQUENCY){
			PREV_TIME = millis();
			SHT31_GetData((uint8_t*)HIGH_REPEATABILITY_CLOCK_STRETCHING_ENABLE);		
			CalculateProcessingTime();
			PREV_TIME = millis();
		}	
		
		if(((millis()- PREV_TIME_LCD)*2.7322)>LCD_UPDATE_FREQUENCY){
			LCD_Interface();	
			/* check temp */			
			
			/* print processing time */
			sprintf(Char_buff, "%d", CYCLE_LENGTH);
			gotoXY(64,1);
			LCDString(Char_buff);
			/* update data to android phone */
			if((SHT31_CRCCheck(SHT31_I2C_Data_Buffer)==1)&&(SHT31_CRCCheck(SHT31_I2C_Data_Buffer+3)==1))
			{					
				printf("%2.2f %2.2f %2.2f   ", SHT31_Temp, SHT31_Humid, TEMP_ALERT);		
				printf("%2.2f %d %d", DATA_SENT_FREQUENCY/1000.0, ALERT_STATUS, MORE_LESS_COMPARISION);
			}
			/* scan available data received from phone */
			freq_setup = GetDataFromPhone();
			/* re-config device based on received data from phone */
			ReconfigByUser();
			PREV_TIME_LCD = millis();
		}	
	
		if(((millis()- PREV_THRESHHOLD_TIME)*2.7322)>THRESHHOLD_FREQUENCY){
			SHT31_TempThreshHold();
			PREV_THRESHHOLD_TIME = millis();
		}
		
		/* GO TO SLEEP WAIT FOR EVENT */
		__WFE;
	/* TEST CODE BEGIN */
	/* TEST CODE END */
	}
}
