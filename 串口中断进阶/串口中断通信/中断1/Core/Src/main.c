
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>

void SystemClock_Config(void);

char c;//指令 0:停止  1:开始
char message[]="hello Windows\n";//输出信息
char tips[]="CommandError\n";//提示1
char tips1[]="Start.....\n";//提示2
char tips2[]="Stop......\n";//提示3
int flag=0;//标志 0:停止发送 1.开始发送


int main(void)
{
	HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART1_UART_Init();
	
	//设置接受中断
	HAL_UART_Receive_IT(&huart1, (uint8_t *)&c, 1);

	
	//当flag为1时,每秒发送一次信息
	//当flag为0时,停止
  while (1)
  {
		if(flag==1){
			//发送信息
			HAL_UART_Transmit(&huart1, (uint8_t *)&message, strlen(message),0xFFFF); 
			
			//延时
			HAL_Delay(1000);
		}
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	
	//当输入的指令为0时,发送提示并改变flag
	if(c=='s'){
		flag=0;
		HAL_UART_Transmit(&huart1, (uint8_t *)&tips2, strlen(tips2),0xFFFF); 
	}
	
	//当输入的指令为1时,发送提示并改变flag
	else if(c=='t'){
		flag=1;
		HAL_UART_Transmit(&huart1, (uint8_t *)&tips1, strlen(tips1),0xFFFF); 
	}
	
	//当输入不存在指令时,发送提示并改变flag
	else {
		flag=0;
		HAL_UART_Transmit(&huart1, (uint8_t *)&tips, strlen(tips),0xFFFF); 
	}

	//重新设置中断
		HAL_UART_Receive_IT(&huart1, (uint8_t *)&c, 1);  
}
/* USER CODE END 4 */
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}



