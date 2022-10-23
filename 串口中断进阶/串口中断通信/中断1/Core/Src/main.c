
#include "main.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>

void SystemClock_Config(void);

char c;//ָ�� 0:ֹͣ  1:��ʼ
char message[]="hello Windows\n";//�����Ϣ
char tips[]="CommandError\n";//��ʾ1
char tips1[]="Start.....\n";//��ʾ2
char tips2[]="Stop......\n";//��ʾ3
int flag=0;//��־ 0:ֹͣ���� 1.��ʼ����


int main(void)
{
	HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART1_UART_Init();
	
	//���ý����ж�
	HAL_UART_Receive_IT(&huart1, (uint8_t *)&c, 1);

	
	//��flagΪ1ʱ,ÿ�뷢��һ����Ϣ
	//��flagΪ0ʱ,ֹͣ
  while (1)
  {
		if(flag==1){
			//������Ϣ
			HAL_UART_Transmit(&huart1, (uint8_t *)&message, strlen(message),0xFFFF); 
			
			//��ʱ
			HAL_Delay(1000);
		}
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	
	//�������ָ��Ϊ0ʱ,������ʾ���ı�flag
	if(c=='s'){
		flag=0;
		HAL_UART_Transmit(&huart1, (uint8_t *)&tips2, strlen(tips2),0xFFFF); 
	}
	
	//�������ָ��Ϊ1ʱ,������ʾ���ı�flag
	else if(c=='t'){
		flag=1;
		HAL_UART_Transmit(&huart1, (uint8_t *)&tips1, strlen(tips1),0xFFFF); 
	}
	
	//�����벻����ָ��ʱ,������ʾ���ı�flag
	else {
		flag=0;
		HAL_UART_Transmit(&huart1, (uint8_t *)&tips, strlen(tips),0xFFFF); 
	}

	//���������ж�
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



