#ifndef __MYIIC_H
#define __MYIIC_H 			   

#include "main.h"
#include "user_delay.h"
//////////////////////////////////////////////////////////////////////////////////	 


////////////////////////////////////////////////////////////////////////////////// 
//IO

#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  

//8,9 - mach ngoai
//6,7 - mach do an

//#define SDA_IN()  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=8<<4;}	//PB9IN
//#define SDA_OUT() {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=3<<4;} //PB9OUT

//#define IIC_SCL    PBout(8) //SCL
//#define IIC_SDA    PBout(9) //SDA	 
//#define READ_SDA   PBin(9)  //SDA IN

#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=8<<28;}	//PB7IN
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=3<<28;} //PB7OUT

#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(7) //SDA	 
#define READ_SDA   PBin(7)  //SDA IN

//#define IIC_SCL			GPIO_Pin_6   
//#define IIC_SDA  		GPIO_Pin_7
//#define READ_SDA   	GPIO_Pin_7

//IIC
void IIC_Init(void);                			 
void IIC_Start(void);				
void IIC_Stop(void);	  			
void IIC_Send_Byte(uint8_t txd);		
uint8_t IIC_Read_Byte(unsigned char ack);
uint8_t IIC_Wait_Ack(void); 				
void IIC_Ack(void);					
void IIC_NAck(void);	
///////////////////////////////
void IIC_Cmd_Write(uint8_t add ,uint8_t reg,uint8_t data);
uint8_t IIC_Write(uint8_t addr,uint8_t reg,uint8_t data);
uint8_t Read_IIC(uint8_t addr, uint8_t reg);
uint8_t IIC_ReadMulti(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);
uint8_t IIC_WriteMulti(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf);

#endif

