#include "usart.h"

#include <stdarg.h>

static char *itoa(int value, char *string, int radix);
static void usart1NvicInit();
static void usart2NvicInit();
static void usart3NvicInit();

struct RX_Data RX1_Data = {0};
struct RX_Data RX2_Data = {0};
struct RX_Data RX3_Data = {0};


void initUsart1(int baudrate) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
 	USART_DeInit(USART1);  //复位串口1

	  //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//使用推免输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10 

    /* USARTx configured as follow:
        - BaudRate = 9600 baud  波特率
        - Word Length = 8 Bits  数据长度
        - One Stop Bit          停止位
        - No parity             校验方式
        - Hardware flow control disabled (RTS and CTS signals) 硬件控制流
        - Receive and transmit enabled                         使能发送和接收
  */

	usart1NvicInit();
		
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   

    USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART1, ENABLE);                    //使能串口 
}

void initUsart2(int baudrate) {

}

void initUsart3(int baudrate) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能USART3，GPIOB时钟
    USART_DeInit(USART3);  //复位串口3

	  //USART3_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//使用推免输出
    GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化PB10
   
    //USART3_RX	  PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PB11 

    /* USARTx configured as follow:
        - BaudRate = 9600 baud  波特率
        - Word Length = 8 Bits  数据长度
        - One Stop Bit          停止位
        - No parity             校验方式
        - Hardware flow control disabled (RTS and CTS signals) 硬件控制流
        - Receive and transmit enabled                         使能发送和接收
  */

	usart3NvicInit();
		
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   

    USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART3, ENABLE);                    //使能串口 
}

void USART1_IRQHandler(void) {
}

void USART3_IRQHandler(void) {
	uint8_t rx_ch;

	// 有数据     
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) { 
		rx_ch = USART_ReceiveData(USART3);
		RX3_Data.UsartRXBUF[RX3_Data.UsartRxLen++] = rx_ch;
	}
	
	//数据帧接收完
	if (USART_GetITStatus(USART3, USART_IT_IDLE) == SET ) {
		RX3_Data.UsartRxFinishFlag = 1;
		rx_ch = USART_ReceiveData(USART3);
	}
	
	USART_ClearFlag(USART3, USART_FLAG_RXNE);
}

static void usart1NvicInit() {
    NVIC_InitTypeDef NVIC_InitStructure;

    //Usart1 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}

static void usart2NvicInit() {
    
}

static void usart3NvicInit() {
    NVIC_InitTypeDef NVIC_InitStructure;

    //Usart1 NVIC 配置

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}

/*
 * 函数名:itoa
 * 将整形数据转换成字符串
 * 输入:-radix =10 表示10进制
 *      -value 要转换的整形数
 *      -buf 转换后的字符串
 *      -radix = 10
 */
static char * itoa(int value, char *string, int radix)
{
	int     i, d;
	int     flag = 0;
	char    *ptr = string;

	/* This implementation only works for decimal numbers. */
	if (radix != 10)
	{
		*ptr = 0;
		return string;
	}

	if (!value)
	{
		*ptr++ = 0x30;
		*ptr = 0;
		return string;
	}

	/* if this is a negative value insert the minus sign. */
	if (value < 0)
	{
		*ptr++ = '-';

		/* Make the value positive. */
		value *= -1;
		
	}

	for (i = 10000; i > 0; i /= 10)
	{
		d = value / i;

		if (d || flag)
		{
			*ptr++ = (char)(d + 0x30);
			value -= (d * i);
			flag = 1;
		}
	}

	/* Null terminate the string. */
	*ptr = 0;

	return string;

} /* NCL_Itoa */

/*
 * 函数名：USART_printf
 * 格式化输出，类似于c库中的printf。
 * 参数:-USARTx 串口通道（USART1，USART2，USART3）
 *		-Data   内容指针
 *		-...   其它参数
 *
 * 示例:	USART_printf(USART1, "\r\n this is a demo \r\n" );
 *          USART_printf(USART1, "\r\n %d \r\n", i );
 *          USART_printf(USART1, "\r\n %s \r\n", j );
 */
void USART_printf(USART_TypeDef * USARTx, char * Data, ...)
{
	const char *s;
	int d;   
	char buf[16];

	
	va_list ap;
	va_start(ap, Data);

	while ( *Data != 0)     
	{				                          
		if ( *Data == 0x5c )  //'\'
		{									  
			switch ( *++Data)
			{
				case 'r':							        
					USART_SendData(USARTx, 0x0d);
					Data ++;
					break;

				case 'n':							         
					USART_SendData(USARTx, 0x0a);	
					Data ++;
					break;

				default:
					Data ++;
					break;
			}			 
		}
		
		else if (* Data =='%')
		{							
			switch (*++Data)
			{				
				case 's':									
					s = va_arg(ap, const char *);
					for ( ; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
					}
					Data++;
					break;

				case 'd':			
					d = va_arg(ap, int);
					itoa(d, buf, 10);
					
					for (s = buf; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
					}
					Data++;
					break;
				
				default:
					Data++;
					break;
					
			}		 
		}
		else USART_SendData(USARTx, *Data++);
		
		while (USART_GetFlagStatus ( USARTx, USART_FLAG_TXE ) == RESET);
		
	}
}

/*

do{\
                              i=0;\
                              while(*(str+i)!='\0')\
                              { WifiUart->DR=*(str+i); while((WifiUart->SR&0X40)==0);i++;}\
                         }while(0)    

*/