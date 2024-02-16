/**
  ******************************************************************************
  * @file    SSP/Loopback/main.c 
  * @author  WIZnet
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
#include "main.h"
#include "w7500x_ssp.h"

/* Private function prototypes -----------------------------------------------*/
void delay_ms(__IO uint32_t nCount);
static void UART_Config(void);
void SSP0_Master_Init(void);
void SSP1_Slave_Init(void);                                  
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main Function
  */
int main()
{

    uint16_t masterTxData = 0x4321; 
    uint16_t slaveRxData = 0; 

    /* Set Systme init */
    SystemInit();

    UART_Config();

    printf("W7500x Standard Peripheral Library version : %d.%d.%d\r\n", __W7500X_STDPERIPH_VERSION_MAIN, __W7500X_STDPERIPH_VERSION_SUB1, __W7500X_STDPERIPH_VERSION_SUB2);
	
    printf("SourceClock : %d\r\n", (int) GetSourceClock());    
    printf("SystemClock : %d\r\n", (int) GetSystemClock());

    SSP0_Master_Init();
    SSP1_Slave_Init();
    
    SSP_SendData(SSP0, masterTxData);

    while(SSP_GetFlagStatus(SSP1, SSP_FLAG_RNE) != SET); 
    
    slaveRxData = SSP_ReceiveData(SSP1);

    printf("masterTxData: %#04x\r\n", masterTxData);
    printf("slaveRxData: %#04x\r\n", slaveRxData);

    if(slaveRxData == masterTxData) {
        printf("\nSSP Passed!!\r\n");
    } else {
        printf("\nSSP Failed!!\r\n");
    }

    printf("END SSP\r\n");  
}


/**
  * @brief  Delay Function
  */
void delay_ms(__IO uint32_t nCount)
{
    volatile uint32_t delay = nCount * 2500;  // approximate loops per ms at 24 MHz, Debug config
    for(; delay != 0; delay--)
        __NOP();
}

void SSP0_Master_Init(void) {
    SSP_InitTypeDef SSP_InitStructure;
    
    /* SSP0 Init -- SSP Master */ 
    SSP_StructInit(&SSP_InitStructure);
    SSP_InitStructure.SSP_FrameFormat  = SSP_FrameFormat_MO; // Motorora SPI mode
    SSP_InitStructure.SSP_DataSize = SSP_DataSize_16b;
    SSP_Init(SSP0,&SSP_InitStructure);
    SSP_Cmd(SSP0, ENABLE);

}

void SSP1_Slave_Init(void) {
    SSP_InitTypeDef SSP_InitStructure;
    
    /* SSP1 Init -- SSP Slave */
    SSP_StructInit(&SSP_InitStructure);
    SSP_InitStructure.SSP_DataSize = SSP_DataSize_16b;
    SSP_InitStructure.SSP_Mode = SSP_Mode_Slave; // SSP1 = Slave
    SSP_Init(SSP1,&SSP_InitStructure);
    SSP_Cmd(SSP1, ENABLE);
}

static void UART_Config(void)
{
    UART_InitTypeDef UART_InitStructure;

    UART_StructInit(&UART_InitStructure);
	
#if defined (USE_WIZWIKI_W7500_EVAL)
    UART_Init(UART1, &UART_InitStructure);
    UART_Cmd(UART1, ENABLE);
#else
    S_UART_Init(115200);
    S_UART_Cmd(ENABLE);
#endif
}



#ifdef  USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif

/**
 * @}
 */

/**
 * @}
 */

/******************** (C) COPYRIGHT WIZnet *****END OF FILE********************/
