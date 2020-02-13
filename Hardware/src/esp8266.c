#include <string.h>  

#include "esp8266.h"
#include "led.h"
#include "sys_tick.h"

static void configESP8266GPIO();

void initESP8266() {
    initUsart3(115200);
    ESP8266_RST_HIGH_LEVEL(); 	//复位引脚拉高
    ESP8266_CH_ENABLE();	//片选失效
	
	delay_ms(500);
	
	while(!setSATandAP()) {
		USART_printf(USART1, "setSATandAP failed!\n");
		delay_ms(500);
	}
	
	reset8266();
	
	while(!wifiConnect("Xiaomi_9E40", "123123123")) {
		USART_printf(USART1, "connect wifi failed!\n");
		delay_ms(500);
	}
	
	while(!getIP()) {
		USART_printf(USART1, "getIP failed!\n");
		delay_ms(500);
	}
	
	// 启动模块多连接
	while(!enableMultipleID()) {
		USART_printf(USART1, "enableMultipleID failed!\n");
		delay_ms(500);
	}
	
	while(!enableUnvarnishSend()) {
		USART_printf(USART1, "enableUnvarnishSend failed!\n");
		delay_ms(500);
	}
	
	while(!connectServer("192.168.31.129", 6000)) {
		USART_printf(USART1, "connect server failed!\n");
		delay_ms(500);
	}
	
	wifiCMD("AT+CIPSEND" , 0, 0, 500);
}

void wifiSendData(char *str) {
	while (*str != '\0') {
		USART3->DR = *str;
		while((USART3->SR&0X40) == 0);
		str++;
	}
}

void wifiSendOneData(uint8_t data) {
	USART3->DR = data;
	while((USART3->SR&0X40) == 0);
}

void wifiSendDataBylen(char *str, int len) {
	int i = 0;
	for(; i < len; i++) {
		USART3->DR = *str;
		while((USART3->SR&0X40) == 0);
		str++;
	}
}

void test8266(){
	const int data_len = 1;
	char data[data_len];
	int i = 0;
	for (; i < data_len; i++) {
		data[i] = i;
	}
	while (1) {
		//delay_ms(500);
		LED2_REV;
		LED3_REV;
    //USART_printf(USART3, "\r\nAT\r\n");
		wifiSendDataBylen(data, data_len);
		//USART_printf(USART1, "send data\n");
		//delay_ms(1);
	}
}

void reset8266() {
	wifiCMD("AT+RST", 0, 0, 500);
}

bool setSATandAP() {
	return wifiCMD("AT+CWMODE=1" , "no change", "OK", 1000);
}

bool wifiConnect(char *ssid, char *pwd) {
	char cmd[120];
	sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"", ssid, pwd);
	return wifiCMD(cmd, "OK", 0, 3000);
}

bool enableMultipleID(){
	return wifiCMD("AT+CIPMUX=0", "OK", 0, 100);
}

bool connectServer(char *ip, uint16_t port) {
	char cmd[120];
	sprintf(cmd, "AT+CIPSTART=\"TCP\",\"%s\",%d", ip, port);
	return wifiCMD(cmd, "OK", 0, 3000);
}

bool enableUnvarnishSend() {
	//if(!wifiCMD("AT+CIPMODE=1", "OK", 0, 500))
		//return false;
	
	//return wifiCMD("AT+CIPSEND", "OK", ">", 500);
	return wifiCMD("AT+CIPMODE=1", "OK", 0, 500);
}

bool getIP() {
	return wifiCMD("AT+CIFSR", 0, 0, 500);
}

bool sendData(char *data) {
	// wifiSendData(data)
}

bool wifiCMD(char *cmd, char *reply1, char *reply2, uint16_t waittime) {
	RX3_Data.UsartRxLen = 0;
	RX3_Data.UsartRxFinishFlag = 0;
	
	char cmd_[128];
	sprintf(cmd_, "%s\r\n", cmd);
	wifiSendData(cmd_);
	if ((reply1 == 0) && (reply2 == 0))                      //不需要接收数据
		return true;
	
	delay_ms(waittime);
	//while(RX3_Data.UsartRxFinishFlag != 1) delay_ms(1);
	RX3_Data.UsartRXBUF[RX3_Data.UsartRxLen] = '\0';
	USART_printf(USART1, "%s\n", RX3_Data.UsartRXBUF);
	RX3_Data.UsartRxFinishFlag = 0;
	if ((reply1 != 0) && (reply2 != 0))
		return (bool)strstr(RX3_Data.UsartRXBUF, reply1) || (bool)strstr(RX3_Data.UsartRXBUF, reply2);
	else if (reply1 != 0)
		return (bool)strstr(RX3_Data.UsartRXBUF, reply1);
	else 
		return (bool)strstr(RX3_Data.UsartRXBUF, reply2);
}

/**
  * @brief  初始化ESP8266用到的GPIO引脚
  * @param  无
  * @retval 无
  */
static void ESP8266_GPIO_Config( void )
{
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;


	/* 配置 CH_PD 引脚*/
	ESP8266_CH_PD_APBxClock_FUN(ESP8266_CH_PD_CLK, ENABLE); 
											   
	GPIO_InitStructure.GPIO_Pin = ESP8266_CH_PD_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

	GPIO_Init(ESP8266_CH_PD_PORT, &GPIO_InitStructure);	 

	/* 配置 RST 引脚*/
	ESP8266_RST_APBxClock_FUN(ESP8266_RST_CLK, ENABLE); 									   
	GPIO_InitStructure.GPIO_Pin = ESP8266_RST_PIN;	
	GPIO_Init(ESP8266_RST_PORT, &GPIO_InitStructure);	 


}