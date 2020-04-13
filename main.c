#include "stm32f0xx.h"
#include "stdio.h"
#include "RS_485.h"
#include "timer.h"
#include "spi.h"
#define countof(a) (sizeof(a) / sizeof(*(a)))//计算数组内的成员个数
//如果想串口中断接收，请不要注释以下宏定义
#define EN_USART2_RX 	1			//0,不接收;1,接收.
uint32_t  fac_us=0;	
extern uint8_t SPI_TX_BUF[3];
extern uint8_t SPI_RX_BUF[3];

/************************************************
函数名称 ： Delay
功    能 ： 软件延时（粗延时）
参    数 ： temp
返 回 值 ： 无
作    者 ： Mico
*************************************************/
void Delay(uint32_t temp)
{
	for(; temp!= 0; temp--);
}

/************************************************
函数名称 ： delay_init
功    能 ： 延时初始化
参    数 ： 无
返 回 值 ： 无
作    者 ： Mico
*************************************************/
void delay_init()
{

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
	fac_us=SystemCoreClock/8000000;				//为系统时钟的1/8  

}								    
/************************************************
函数名称 ： delay_us
功    能 ： 实现微秒级别延时
参    数 ： nus
返 回 值 ： 无
作    者 ： Mico
*************************************************/

void delay_us(uint32_t nus)
{		
	uint32_t temp;	    	 
	SysTick->LOAD=nus*fac_us; 					//时间加载	  		 
	SysTick->VAL=0x00;        					//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL =0X00;      					 //清空计数器	 
}

int main(void)
{
	GPIOA_Config();
	USART_Config();
	delay_init();
	SystemInit();
	SPI_GPIO_Configuration();
	MYDMA_485_Config();//485 DMA初始化
	MYDMA_TX_Config(DMA1_Channel3,(uint32_t)(&(SPI1->DR)),(uint32_t)SPI_TX_BUF,3);//DMA1通道3,外设为SPI,存储器为SPI_TX_BUF,长3.
	MYDMA_RX_Confog(DMA1_Channel2,(uint32_t)(&(SPI1->DR)),(uint32_t)SPI_RX_BUF,3);
  //仿真的时候查看结构体get_rcc_clock中可以看见各个外设的时钟
	RCC_ClocksTypeDef get_rcc_clock; 
	RCC_GetClocksFreq(&get_rcc_clock);

    while(1)
    {

    }
}
