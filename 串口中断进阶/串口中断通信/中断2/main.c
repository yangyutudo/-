#include "stm32f10x.h"
#include "bsp_usart.h"

// 接收缓冲，最大100个字节
uint8_t USART_RX_BUF[100];
// 接收状态标记位
uint16_t USART_RX_FLAG=0;

//串口中断服务函数
void DEBUG_USART_IRQHandler(void)
{
	uint8_t temp;
	//接收中断
	if(USART_GetFlagStatus(DEBUG_USARTx, USART_IT_RXNE) != RESET)
	{
		// 读取接收的数据
		temp = USART_ReceiveData(DEBUG_USARTx);
		//接收未完成
		if((USART_RX_FLAG & 0x8000)==0)
		{
			//接收到了0x0d
			if(USART_RX_FLAG & 0x4000)
			{
				// 接收错误,重新开始
				if(temp != 0x0a) USART_RX_FLAG=0;
				// 接收完成
				else USART_RX_FLAG |= 0x8000;
			}
			// 还未接收到0x0d
			else
			{
				if(temp == 0x0d)
				{
					USART_RX_FLAG |= 0x4000;
				}
				else
				{
					USART_RX_BUF[USART_RX_FLAG & 0x3FFF]=temp;
					USART_RX_FLAG++;
					//接收数据错误，重新开始接收
					if(USART_RX_FLAG > 99) USART_RX_FLAG=0;
				}
			}
		}
	}
}
int main(void)
{
	uint8_t len=0;
	uint8_t i=0;
	// USART初始化
	USART_Config();

	while(1)
	{
		if(USART_RX_FLAG & 0x8000)
		{
			// 获取接收到的数据长度
			len = USART_RX_FLAG & 0x3FFF;
			Usart_Sendstr(DEBUG_USARTx, "你发送的消息：\n");
			for(i=0; i<len;i++)
			{
				// 向串口发送数据
				USART_SendData(DEBUG_USARTx, USART_RX_BUF[i]);
				//等待发送结束
				while(USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TC)!=SET);
			}
			Usart_Sendstr(DEBUG_USARTx, "\n\n");
			if(strcmp((char *)USART_RX_BUF,"stop stm32!")==0)
			{
				Usart_Sendstr(DEBUG_USARTx, "stm32停止！");
				break;
			}
			USART_RX_FLAG=0;
			memset(USART_RX_BUF,0,sizeof(USART_RX_BUF));
		}
	
		else
		{
			Usart_Sendstr(DEBUG_USARTx, "hello windows!\n");
			delay_ms(1000);
		}
	}
	


}
