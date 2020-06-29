/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
 * @defgroup tw_sensor_example main.c
 * @{
 * @ingroup nrf_twi_example
 * @brief TWI Sensor Example main file.
 *
 * This file contains the source code for a sample application using TWI.
 *
 */

#include <stdio.h>
#include "boards.h"
#include "app_util_platform.h"
#include "app_uart.h"
#include "app_error.h"
#include "nrf_drv_twi.h"
#include "nrf_delay.h"

/*SHT31 Address*/
#define SHT31_ADDR 0x44

#define MYBOARD_RX_PIN_NUMBER 25
#define MYBOARD_TX_PIN_NUMBER 23

#define MYBOARD_SCL_PIN 7
#define MYBOARD_SDA_PIN 5

/*UART buffer size. */
#define UART_TX_BUF_SIZE 256
#define UART_RX_BUF_SIZE 1

/* Define version of GCC. */
#define GCC_VERSION (__GNUC__ * 10000 \
                     + __GNUC_MINOR__ * 100 \
                     + __GNUC_PATCHLEVEL__)

#ifdef __GNUC_PATCHLEVEL__
#if GCC_VERSION < 50505
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-braces"           // Hack to GCC 4.9.3 bug. Can be deleted after switch on using GCC 5.0.0
#endif
#endif

#ifdef __GNUC_PATCHLEVEL__
#if GCC_VERSION < 50505
#pragma GCC diagnostic pop
#endif
#endif

/* SHT31 Command for data receiving mode */
const uint8_t SINGLE_SHOT_LOW_REPEAT_CLOCK_STRETCH[2] = {0x2C, 0x10};
const uint8_t SINGLE_SHOT_HIGH_REPEAT_CLOCK_STRETCH[2] = {0x2C, 0x06};
const uint8_t PERIODIC_1_MPS_LOW_REPEAT[2] = {0x21, 0x2D};
const uint8_t PERIODIC_1_MPS_HIGH_REPEAT[2] = {0x21, 0x30};


typedef struct  
{
	uint8_t h_temp;
	uint8_t l_temp;
	uint8_t CRC_temp;
	uint8_t h_humid;
	uint8_t l_humid;
	uint8_t CRC_humid;

} sample_t;


/* Indicates if reading operation from accelerometer has ended. */
static volatile bool m_xfer_done = true;
/* Indicates if setting mode operation has ended. */
static volatile bool m_set_mode_done = false;
/* TWI instance. */
static const nrf_drv_twi_t twi_sht31_nrf51822 = NRF_DRV_TWI_INSTANCE(0);

/* Store I2c Received Data */
#define I2C_RX_BUF_SIZE 6
volatile static uint8_t I2C_RECEIVE_BUF[I2C_RX_BUF_SIZE];


/**
 * @brief UART events handler.
 */
static void uart_events_handler(app_uart_evt_t * p_event)
{
    switch (p_event->evt_type)
    {
        case APP_UART_COMMUNICATION_ERROR:
            APP_ERROR_HANDLER(p_event->data.error_communication);
            break;

        case APP_UART_FIFO_ERROR:
            APP_ERROR_HANDLER(p_event->data.error_code);
            break;

        default:
            break;
    }
}

/**
 * @brief UART initialization.
 */
static void uart_config(void)
{
    uint32_t                     err_code;
    const app_uart_comm_params_t comm_params =
    {
        MYBOARD_RX_PIN_NUMBER,
        MYBOARD_TX_PIN_NUMBER,
        RTS_PIN_NUMBER,
        CTS_PIN_NUMBER,
        APP_UART_FLOW_CONTROL_DISABLED,
        false,
        UART_BAUDRATE_BAUDRATE_Baud9600
    };

    APP_UART_FIFO_INIT(&comm_params,
                       UART_RX_BUF_SIZE,
                       UART_TX_BUF_SIZE,
                       uart_events_handler,
                       APP_IRQ_PRIORITY_LOW,
                       err_code);

    APP_ERROR_CHECK(err_code);
}




float SHT31_tempCal(){
	uint16_t raw = ((uint16_t)I2C_RECEIVE_BUF[0]<<8) + I2C_RECEIVE_BUF[1];	
	return -45 + (float)raw*175/65535;
	
}

float SHT31_humidCal(){
	uint16_t raw = ((uint16_t)I2C_RECEIVE_BUF[3]<<8) + I2C_RECEIVE_BUF[4];	
	return 100*(float)raw/65535;
}

void print_value(){
//	for(int i = 0; i <6; i ++){
//		printf("0x%x\n",I2C_RECEIVE_BUF[i]);
//	}
//	app_uart_put((uint32_t)I2C_RECEIVE_BUF[0]);
//	app_uart_put((uint32_t)I2C_RECEIVE_BUF[1]);
	float temp = SHT31_tempCal();
	float humid = SHT31_humidCal();
//	app_uart_put(temp);
	static int count = 0;
	count++;
	printf("No.%d: Temp = %.2f , Humid = %.2f%%\r\n", count, temp, humid);
//	  printf("%.2f", temp);
	
}

/**
 * @brief TWI events handler.
 */
void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{   
    ret_code_t err_code;
    //static sample_t m_sample;
    
    switch(p_event->type)
    {
        case NRF_DRV_TWI_EVT_DONE:
            if ((p_event->type == NRF_DRV_TWI_EVT_DONE) &&
                (p_event->xfer_desc.type == NRF_DRV_TWI_XFER_TX))
            {
                if(m_set_mode_done != true)
                {
                    m_set_mode_done  = true;
                    return;
                }
                m_xfer_done = false;
                /* Read 6 bytes from the specified address. */
                err_code = nrf_drv_twi_rx(&twi_sht31_nrf51822, SHT31_ADDR, (uint8_t *)I2C_RECEIVE_BUF, sizeof(I2C_RECEIVE_BUF));
                APP_ERROR_CHECK(err_code);
            }
            else //Receive complete
            {
                print_value();
                m_xfer_done = true;
            }
            break;
        default:
            break;        
    }   
}

/**
 * @brief UART initialization.
 */
void twi_init (void)
{
    ret_code_t err_code;
    
    const nrf_drv_twi_config_t twi_sht_31_config = {
       .scl                = MYBOARD_SCL_PIN,
       .sda                = MYBOARD_SDA_PIN,
       .frequency          = NRF_TWI_FREQ_100K,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH
    };
    
    err_code = nrf_drv_twi_init(&twi_sht31_nrf51822, &twi_sht_31_config, twi_handler, NULL);
    APP_ERROR_CHECK(err_code);
    
    nrf_drv_twi_enable(&twi_sht31_nrf51822);
}

/**
 * @brief Function for main application entry.
 */
int main(void)
{
    uart_config();
		// int a = __GNUC__, c = __GNUC_PATCHLEVEL__;
//		printf("\n\rUART-Config\r\n");
//    twi_init();
//		printf("\n\rTWI-Config\r\n");
//    printf("\n\rStart!\r\n");
    ret_code_t err_code;
    
    while(true)
    {
        nrf_delay_ms(1000);
				printf("\n\rWorking\n\r");
        /* Start transaction with a slave with the specified address. */
//				err_code = nrf_drv_twi_tx(&twi_sht31_nrf51822, SHT31_ADDR, SINGLE_SHOT_LOW_REPEAT_CLOCK_STRETCH, sizeof(SINGLE_SHOT_LOW_REPEAT_CLOCK_STRETCH), false);		
//        do
//        {
//            __WFE();
//        }while(m_xfer_done == false);
//        APP_ERROR_CHECK(err_code);
//        m_xfer_done = false;
    }
}

/** @} */
