#include "stm32f0xx.h"
#include "stdio.h"
#include "RS_485.h"
#include "timer.h"
#include "spi.h"
#define countof(a) (sizeof(a) / sizeof(*(a)))//���������ڵĳ�Ա����
//����봮���жϽ��գ��벻Ҫע�����º궨��
#define EN_USART2_RX 	1			//0,������;1,����.
uint32_t  fac_us=0;	
extern uint8_t SPI_TX_BUF[3];
extern uint8_t SPI_RX_BUF[3];

/************************************************
�������� �� Delay
��    �� �� �����ʱ������ʱ��
��    �� �� temp
�� �� ֵ �� ��
��    �� �� Mico
*************************************************/
void Delay(uint32_t temp)
{
	for(; temp!= 0; temp--);
}

/************************************************
�������� �� delay_init
��    �� �� ��ʱ��ʼ��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� Mico
*************************************************/
void delay_init()
{

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8
	fac_us=SystemCoreClock/8000000;				//Ϊϵͳʱ�ӵ�1/8  

}								    
/************************************************
�������� �� delay_us
��    �� �� ʵ��΢�뼶����ʱ
��    �� �� nus
�� �� ֵ �� ��
��    �� �� Mico
*************************************************/

void delay_us(uint32_t nus)
{		
	uint32_t temp;	    	 
	SysTick->LOAD=nus*fac_us; 					//ʱ�����	  		 
	SysTick->VAL=0x00;        					//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0X00;      					 //��ռ�����	 
}

int main(void)
{
	GPIOA_Config();
	USART_Config();
	delay_init();
	SystemInit();
	SPI_GPIO_Configuration();
	MYDMA_485_Config();//485 DMA��ʼ��
	MYDMA_TX_Config(DMA1_Channel3,(uint32_t)(&(SPI1->DR)),(uint32_t)SPI_TX_BUF,3);//DMA1ͨ��3,����ΪSPI,�洢��ΪSPI_TX_BUF,��3.
	MYDMA_RX_Confog(DMA1_Channel2,(uint32_t)(&(SPI1->DR)),(uint32_t)SPI_RX_BUF,3);
  //�����ʱ��鿴�ṹ��get_rcc_clock�п��Կ������������ʱ��
	RCC_ClocksTypeDef get_rcc_clock; 
	RCC_GetClocksFreq(&get_rcc_clock);

    while(1)
    {

    }
}
