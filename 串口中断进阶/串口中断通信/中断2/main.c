#include "stm32f10x.h"
#include "bsp_usart.h"

// ���ջ��壬���100���ֽ�
uint8_t USART_RX_BUF[100];
// ����״̬���λ
uint16_t USART_RX_FLAG=0;

//�����жϷ�����
void DEBUG_USART_IRQHandler(void)
{
	uint8_t temp;
	//�����ж�
	if(USART_GetFlagStatus(DEBUG_USARTx, USART_IT_RXNE) != RESET)
	{
		// ��ȡ���յ�����
		temp = USART_ReceiveData(DEBUG_USARTx);
		//����δ���
		if((USART_RX_FLAG & 0x8000)==0)
		{
			//���յ���0x0d
			if(USART_RX_FLAG & 0x4000)
			{
				// ���մ���,���¿�ʼ
				if(temp != 0x0a) USART_RX_FLAG=0;
				// �������
				else USART_RX_FLAG |= 0x8000;
			}
			// ��δ���յ�0x0d
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
					//�������ݴ������¿�ʼ����
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
	// USART��ʼ��
	USART_Config();

	while(1)
	{
		if(USART_RX_FLAG & 0x8000)
		{
			// ��ȡ���յ������ݳ���
			len = USART_RX_FLAG & 0x3FFF;
			Usart_Sendstr(DEBUG_USARTx, "�㷢�͵���Ϣ��\n");
			for(i=0; i<len;i++)
			{
				// �򴮿ڷ�������
				USART_SendData(DEBUG_USARTx, USART_RX_BUF[i]);
				//�ȴ����ͽ���
				while(USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TC)!=SET);
			}
			Usart_Sendstr(DEBUG_USARTx, "\n\n");
			if(strcmp((char *)USART_RX_BUF,"stop stm32!")==0)
			{
				Usart_Sendstr(DEBUG_USARTx, "stm32ֹͣ��");
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
