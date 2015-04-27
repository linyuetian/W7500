/**
  ******************************************************************************
  * @file    Uart/Polling/main.c 
  * @author  IOP Team
  * @version V1.0.0
  * @date    01-May-2015
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, WIZnet SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2015 WIZnet Co.,Ltd.</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "W7500x.h"
/* Private typedef -----------------------------------------------------------*/
typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define TxBufferSize    (countof(TxBuffer))
#define countof(a)  (sizeof(a) / sizeof(*(a)))
	
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
UART_InitTypeDef UART_InitStructure;
uint8_t TxBuffer[] = "Buffer Send from UART0 to UART1";
uint8_t RxBuffer[TxBufferSize];
__IO uint8_t TxCounter = 0, RxCounter = 0;
volatile TestStatus TransferStatus = FAILED;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/**
  * @brief   Main program
  * @param  None
  * @retval None
  *
*/


int main()
{
    uint32_t i;

	 /*System clock configuration*/
	SystemInit();
    /* UART0 and UART1 configuration*/
    UART_StructInit(&UART_InitStructure);
    /* Configure UART0 */
    UART_Init(UART0,&UART_InitStructure);
    /* Configure UART1 */
    UART_Init(UART1,&UART_InitStructure);

    while(TxCounter < TxBufferSize)
    {
        /* Send one byte from UART0 to UART1 */
        UART_SendData(UART0,TxBuffer[TxCounter++]);

        /* Loop until UART0 TX FIFO Register is empty */
        while(UART_GetFlagStatus(UART0,UART_FLAG_TXFE) == RESET)
        {
        } 

        /* Loop until the UART1 Receive FIFO Register is not empty */
        while(UART_GetFlagStatus(UART1,UART_FLAG_RXFE) == SET)
        {
        }
        
        /* Store the received byte in RxBuffer */
        RxBuffer[RxCounter++] = (UART_ReceiveData(UART1) & 0xFF);
    }

    /* Check the received data with the send ones */
    TransferStatus  = Buffercmp(TxBuffer, RxBuffer, TxBufferSize);
    /* TransferStatus = PASSED, if the data transmitted from USARTy and  
       received by USARTz are the same */
    /* TransferStatus = FAILED, if the data transmitted from USARTy and 
       received by USARTz are different */

    while(1)
    {
    } 
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
    while(BufferLength--)
    {
        if(*pBuffer1 != *pBuffer2)
        {
            return FAILED;
        }
    
        pBuffer1++;
        pBuffer2++;
    }

    return PASSED;  
}
