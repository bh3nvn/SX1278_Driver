/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: (1) "AS IS" WITH NO WARRANTY; AND 
 * (2)TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, SEMTECH SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 * 
 * Copyright (C) SEMTECH S.A.
 */
/*! 
 * \file       sx1278-Hal.c
 * \brief      SX1278 Hardware Abstraction Layer
 *
 * \version    2.0.B2 
 * \date       Nov 21 2012
 * \author     Miguel Luis
 *
 * Last modified by Marc0 Xu on Jan 07 2018
 */
//#include <stdint.h>
//#include <stdbool.h> 

#include "platform.h"

#if defined( USE_SX1278_RADIO )

#include "sx1278-Hal.h"

#define GPIO_Low_Power_Mode     GPIO_Mode_Out_PP_Low_Fast

/*!
 * SX1278 RESET I/O definitions
 */


/*!
 * SX1278 SPI NSS I/O definitions
 */

#define NSS_IOPORT              GPIOB
#define NSS_PIN                 GPIO_Pin_4

#define SPI_NSS_LOW()            GPIO_WriteBit(NSS_IOPORT , NSS_PIN, RESET)
#define SPI_NSS_HIGH()           GPIO_WriteBit(NSS_IOPORT , NSS_PIN, SET)

#define DUMMY_BYTE   0
/*!
 * SX1278 DIO pins  I/O definitions
 */

#define DIO0_IOPORT            GPIOC
#define DIO0_PIN               GPIO_Pin_5


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Configure system clock to run at 16MHz
  * @param  None
  * @retval None
  */
static void CLK_Config(void)
{
   /* System Clock is HSI/1 */
  CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
  
  /* Enable TIM4 CLK */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);
}

/**
  * @brief  Configure TIM4 peripheral   1ms
  * @param  None
  * @retval None
  */
static void TIM4_Config(void)
{
  /* TIM4 configuration:
   - TIM4CLK is set to 16 MHz, the TIM4 Prescaler is equal to 128 so the TIM1 counter
   clock used is 16 MHz / 128 = 125000Hz
  - With 125 000 Hz we can generate time base:
      max time base is 66 ms if TIM4_PERIOD = 255 --> (255 + 1) / 7812.5 = 2.048 ms
      min time base is 0.5ms if TIM4_PERIOD = 1   --> (  1 + 1) / 7812.5 = 0.016 ms
  - In this example we need to generate a time base equal to 1 ms
   so TIM4_PERIOD = (0.001 * 125000 - 1) = 124 */

  /* Time base configuration */
  TIM4_TimeBaseInit(TIM4_Prescaler_128, 124);
  TIM4_SetCounter(0x00);
  TIM4_ARRPreloadConfig(ENABLE);
  /* Clear TIM4 update flag */
  TIM4_ClearFlag(TIM4_FLAG_Update);
  /* Enable update interrupt */
  TIM4_ITConfig(TIM4_IT_Update, ENABLE);
  /* Enable TIM4 */
  TIM4_Cmd(ENABLE);
}

/**
  * @brief  Configure GPIOS 
  * @param  None
  * @retval None
  */
static void GPIO_Config(void)
{
  GPIO_Init(GPIOA, GPIO_Pin_0, GPIO_Low_Power_Mode);
  GPIO_Init(GPIOA, GPIO_Pin_1, GPIO_Low_Power_Mode);
  GPIO_Init(GPIOA, GPIO_Pin_2, GPIO_Low_Power_Mode);
  GPIO_Init(GPIOA, GPIO_Pin_3, GPIO_Low_Power_Mode);
  GPIO_Init(GPIOA, GPIO_Pin_4, GPIO_Low_Power_Mode);
  GPIO_Init(GPIOA, GPIO_Pin_5, GPIO_Low_Power_Mode);
  GPIO_Init(GPIOA, GPIO_Pin_6, GPIO_Low_Power_Mode);

  
  GPIO_Init(GPIOB, GPIO_Pin_0, GPIO_Low_Power_Mode);
  GPIO_Init(GPIOB, GPIO_Pin_1, GPIO_Low_Power_Mode);
  GPIO_Init(GPIOB, GPIO_Pin_2, GPIO_Low_Power_Mode);
  GPIO_Init(GPIOB, GPIO_Pin_3, GPIO_Low_Power_Mode);
  GPIO_Init(GPIOB, GPIO_Pin_4, GPIO_Low_Power_Mode);
  GPIO_Init(GPIOB, GPIO_Pin_5, GPIO_Low_Power_Mode);
  GPIO_Init(GPIOB, GPIO_Pin_6, GPIO_Low_Power_Mode);
  GPIO_Init(GPIOB, GPIO_Pin_7, GPIO_Low_Power_Mode);
 
  
  GPIO_Init(GPIOC, GPIO_Pin_0, GPIO_Low_Power_Mode);
  GPIO_Init(GPIOC, GPIO_Pin_1, GPIO_Low_Power_Mode);
  GPIO_Init(GPIOC, GPIO_Pin_2, GPIO_Low_Power_Mode);
  GPIO_Init(GPIOC, GPIO_Pin_3, GPIO_Low_Power_Mode);
  GPIO_Init(GPIOC, GPIO_Pin_4, GPIO_Low_Power_Mode);
  GPIO_Init(GPIOC, GPIO_Pin_5, GPIO_Low_Power_Mode);
  GPIO_Init(GPIOC, GPIO_Pin_6, GPIO_Low_Power_Mode);
  
  GPIO_Init(GPIOD, GPIO_Pin_1, GPIO_Low_Power_Mode);
  GPIO_Init(GPIOD, GPIO_Pin_2, GPIO_Low_Power_Mode);
  GPIO_Init(GPIOD, GPIO_Pin_3, GPIO_Low_Power_Mode);
  GPIO_Init(GPIOD, GPIO_Pin_4, GPIO_Low_Power_Mode);
  GPIO_Init(GPIOD, GPIO_Pin_5, GPIO_Low_Power_Mode);
  GPIO_Init(GPIOD, GPIO_Pin_6, GPIO_Low_Power_Mode);
  GPIO_Init(GPIOD, GPIO_Pin_7, GPIO_Low_Power_Mode);
}

void BoardInit( void )
{
  CLK_Config();
  
  GPIO_Config();
  
  Init_SPI();

  TIM4_Config();

  /* Enable general Interrupt*/
  enableInterrupts();
}

void SX1278InitIo( void )
{
  GPIO_Init(DIO0_IOPORT,DIO0_PIN,GPIO_Mode_In_FL_No_IT);
  //EXTI_SetPinSensitivity(EXTI_Pin_5, EXTI_Trigger_Rising);
}

void SX1278SetReset( uint8_t state )
{
 
}

/**
  * @brief  Initializes the peripherals used by the SPI PCAP01 driver.
  * @param  None
  * @retval None
  */
void Init_SPI(void)
{
   /* Pcap01_SPI Periph clock enable */
   CLK_PeripheralClockConfig(CLK_Peripheral_SPI1, ENABLE);
   //SPI_CLOCK:PB5, SPI_MOSI: PB6, SPI_MISO: PB7  
   GPIO_Init(GPIOB, GPIO_Pin_5, GPIO_Mode_Out_PP_High_Fast);  
   GPIO_Init(GPIOB, GPIO_Pin_6, GPIO_Mode_Out_PP_High_Fast);  
   GPIO_Init(GPIOB, GPIO_Pin_7, GPIO_Mode_In_PU_No_IT);  
    /* CS */  
   GPIO_Init(GPIOB , GPIO_Pin_4, GPIO_Mode_Out_PP_High_Fast);  
   
   SPI_NSS_HIGH();   
     
   SPI_Init(SPI1, SPI_FirstBit_MSB, SPI_BaudRatePrescaler_2, SPI_Mode_Master,  
            SPI_CPOL_Low, SPI_CPHA_1Edge,   
            SPI_Direction_2Lines_FullDuplex, SPI_NSS_Soft, 0x07);  
      
   SPI_Cmd(SPI1, ENABLE);  
}

/**********************************************************
* @brief  Sends a byte through the SPI interface and 
					return the byte received from the SPI bus.
* @param  byte: byte to send.
* @retval The value of the received byte.
**********************************************************/
static uint8_t SPI1_SendByte(uint8_t byte)
{
  /* Loop while DR register in not emplty */
  while (SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE) == RESET);
  
  /* Send byte through the SPI peripheral */
  SPI_SendData(SPI1, byte);

  /* Wait to receive a byte */
  while (SPI_GetFlagStatus(SPI1, SPI_FLAG_RXNE) == RESET);
  /* Return the byte read from the SPI bus */
  return SPI_ReceiveData(SPI1);
}
/**********************************************************
**Name:     SPIWrite
**Function: SPI Write CMD
**Input:    uint8_t address & uint8_t data
**Output:   None
**********************************************************/
void SPIWrite(uint8_t adr, uint8_t WrPara)  
{	
  SPI_NSS_LOW();						
  SPI1_SendByte(adr|0x80);		 
  SPI1_SendByte(WrPara);                
  SPI_NSS_HIGH();
}
/**********************************************************
**Name:     SPIRead
**Function: SPI Read CMD
**Input:    adr -> address for read
**Output:   None
**********************************************************/
uint8_t SPIRead(uint8_t adr)
{
  uint8_t tmp; 
  SPI_NSS_LOW();
  SPI1_SendByte(adr);                          
  tmp = SPI1_SendByte(DUMMY_BYTE);
  SPI_NSS_HIGH();  
  return(tmp);
}
/**********************************************************
**Name:     SX1278ReadBuffer
**Function: SPI burst read mode
**Input:    adr-----address for read
**          ptr-----data buffer point for read
**          length--how many bytes for read
**Output:   None
**********************************************************/

void SX1278ReadBuffer(uint8_t adr, uint8_t *ptr, uint8_t length)
{
  uint8_t i;
  
  SPI_NSS_LOW();
  SPI1_SendByte(adr); 
  for(i=0;i<length;i++)
  ptr[i] = SPI1_SendByte(DUMMY_BYTE);
  SPI_NSS_HIGH(); 
}
/**********************************************************
**Name:     SX1278WriteBuffer
**Function: SPI burst write mode
**Input:    adr-----address for write
**          ptr-----data buffer point for write
**          length--how many bytes for write
**Output:   none
**********************************************************/      
void SX1278WriteBuffer(uint8_t adr, uint8_t *ptr, uint8_t length)
{ 
  uint8_t i;

  SPI_NSS_LOW();       
  SPI1_SendByte(adr|0x80);
  for(i=0;i<length;i++)
  SPI1_SendByte(ptr[i]);
  SPI_NSS_HIGH(); 
}

void SX1278Write( uint8_t addr, uint8_t data )
{
    SX1278WriteBuffer( addr, &data, 1 );
}

void SX1278Read( uint8_t addr, uint8_t *data )
{
    SX1278ReadBuffer( addr, data, 1 );
}

void SX1278WriteFifo( uint8_t *buffer, uint8_t size )
{
    SX1278WriteBuffer( 0, buffer, size );
}

void SX1278ReadFifo( uint8_t *buffer, uint8_t size )
{
    SX1278ReadBuffer( 0, buffer, size );
}

inline uint8_t SX1278ReadDio0( void )
{
    return GPIO_ReadInputDataBit( DIO0_IOPORT, DIO0_PIN );
}

#endif // USE_SX1278_RADIO
