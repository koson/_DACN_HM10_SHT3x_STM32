  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H



/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "User_Function.h" 


#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
	#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */
	
void USART2_Configuration(uint32_t Baudrate);	
void USART1_Configuration(uint32_t Baudrate);
void USART3_Configuration(uint32_t Baudrate);
	
void sendSHT31_I2C_Data_Buffer(uint8_t* SHT31_I2C_Data_Buffer, int size);
void LCD_Interface(void);
void SHT31_TempThreshHold(void);
void SHT31_GetData(uint8_t* COMMAND);

	


#endif /* __MAIN_H */
