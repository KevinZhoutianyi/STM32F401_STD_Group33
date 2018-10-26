#include "sys.h"
#include "usart.h"	
#include "led.h"

#if 1
#pragma import(__use_no_semihosting)                             
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;        
void _sys_exit(int x) 
{ 
	x = x; 
} 

int fputc(int ch, FILE *f)
{ 	
	while((USART2->SR&0X40)==0); 
	USART2->DR = (u8) ch;      
	return ch;
}
#endif
 
#if EN_USART1_RX   
 	
u8 USART_RX_BUF[USART_REC_LEN];   
//接收状态
//bit15，	completer
//bit14，receive x0d
//bit13~0，	recieve effective vale
u16 USART_RX_STA=0;       //status	


void uart_init(u32 bound,int USARTx){

  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	if(USARTx==1)
	{
	
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

		GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
		

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
		GPIO_Init(GPIOA,&GPIO_InitStructure); 

		USART_InitStructure.USART_BaudRate = bound;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
		USART_Init(USART1, &USART_InitStructure); 
		
		USART_Cmd(USART1, ENABLE); 
		
		//USART_ClearFlag(USART1, USART_FLAG_TC);
		
	#if EN_USART1_RX	
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

		//Usart1 NVIC 配置
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
		NVIC_Init(&NVIC_InitStructure);	

	#endif
	}
	else if(USARTx==2)
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	 

		GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART1); 
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART1); 
		

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOA,&GPIO_InitStructure); 

	
		USART_InitStructure.USART_BaudRate = bound;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
		USART_Init(USART2, &USART_InitStructure); 
		
		USART_Cmd(USART2, ENABLE); 
		

		
	#if EN_USART1_RX	
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
		NVIC_Init(&NVIC_InitStructure);

	#endif

	}
	
}


void USART1_IRQHandler(void)                	
{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
	{
		
		if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_5)==1)
			GPIO_ResetBits(GPIOA,GPIO_Pin_5);
		else if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_5)==0)
			GPIO_SetBits(GPIOA,GPIO_Pin_5);
		
		Res =USART_ReceiveData(USART1);
		USART_SendData(USART1,Res);
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
  } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 

void USART2_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		
		if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_5)==1)
			GPIO_ResetBits(GPIOA,GPIO_Pin_5);
		else if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_5)==0)
			GPIO_SetBits(GPIOA,GPIO_Pin_5);
		
		Res =USART_ReceiveData(USART2);//(USART1->DR);	//读取接收到的数据
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART_RX_STA&0x4000)//接收到了0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
			}
			else //还没收到0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		}   		 
  } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 
#endif	

 



