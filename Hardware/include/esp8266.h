#ifndef _ESP8266_H_
#define _ESP8266_H_

#include "stm32f10x.h"
#include "usart.h"
#include "stdbool.h"

#define ESP8266_CH_PD_APBxClock_FUN  RCC_APB2PeriphClockCmd
#define ESP8266_CH_PD_CLK            RCC_APB2Periph_GPIOE  
#define ESP8266_CH_PD_PORT           GPIOE
#define ESP8266_CH_PD_PIN            GPIO_Pin_14

#define ESP8266_RST_APBxClock_FUN    RCC_APB2PeriphClockCmd
#define ESP8266_RST_CLK              RCC_APB2Periph_GPIOE
#define ESP8266_RST_PORT             GPIOE
#define ESP8266_RST_PIN              GPIO_Pin_15

#define ESP8266_CH_ENABLE()    GPIO_SetBits ( ESP8266_CH_PD_PORT, ESP8266_CH_PD_PIN )
#define ESP8266_CH_DISABLE()   GPIO_ResetBits ( ESP8266_CH_PD_PORT, ESP8266_CH_PD_PIN )

#define ESP8266_RST_HIGH_LEVEL() GPIO_SetBits ( ESP8266_RST_PORT, ESP8266_RST_PIN )
#define ESP8266_RST_LOW_LEVEL()  GPIO_ResetBits ( ESP8266_RST_PORT, ESP8266_RST_PIN )

void initESP8266();
void wifiSendData(char *str);
bool setSATandAP();
bool wifiCMD(char *cmd, char *reply1, char *reply2, uint16_t waittime);
bool wifiConnect(char *ssid, char *pwd);
bool enableMultipleID();  // 
bool connectServer(char *ip, uint16_t port);
bool enableUnvarnishSend(); // 
bool getIP();
bool sendData(char *data);
void reset8266();
void test8266();

#endif /* _ESP8266_H_ */
