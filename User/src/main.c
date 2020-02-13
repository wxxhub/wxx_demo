#include <stdarg.h>
#include "stm32f10x.h"
#include "usart.h"
#include "led.h"
#include "esp8266.h"
#include "sys_tick.h"
#include "ov7725.h"

const uint32_t CAMERA_WIDTH = 320;
const uint32_t CAMERA_HEIGHT = 240;
const uint32_t CAMERA_SIZE = CAMERA_WIDTH * CAMERA_HEIGHT;
const uint8_t end_code = 0XFF;
extern u8 ov_sta;

// void cameraRefresh() {
// 	if (ov_sta) {
// 		USART_printf(USART1, "cameraRefresh\n");
// 		OV7725_RRST = 0;				//��ʼ��λ��ָ�� 
// 		OV7725_RCK_L;
// 		OV7725_RCK_H;
// 		OV7725_RRST = 1;				//��λ��ָ����� 
// 		OV7725_RCK_L;
// 		OV7725_RCK_H;  
// 		int i;
// 		uint32_t color;

// 		i = 0;
// 		for (i = 0; i < CAMERA_SIZE; i++) {
// 			color = 0;
// 			OV7725_RCK_L;
// 			color = GPIOC->IDR&0XFF;	//������
// 			OV7725_RCK_H; 
// 			color <<= 8;  
			
// 			OV7725_RCK_L;
// 			color |= GPIOC->IDR&0XFF;	//������
// 			OV7725_RCK_H;
// 			USART_printf(USART1, "%d\n", color);
// 			//printf("%d\n", color);
// 		}
// 		USART_printf(USART1, "######\n");
// 		ov_sta=0;					//����֡�жϱ��
// 	}
// }

uint8_t rgb2gray(uint16_t color) {
	uint16_t c1 = (color & 0X001F) << 3;
	uint16_t c2 = (color & 0X07E0) >> 3;
	uint16_t c3 = (color & 0XF800) >> 8;
	uint16_t result = (c1 + c2 + c3) / 3;
	if (result == 0XFF) return 0XFE;
	
	return result;
}

void sendCode(uint8_t code) {
	USART_SendData(USART1, code);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE ) == RESET);
}

void cameraRefresh() {
	if (ov_sta) {
		OV7725_RRST = 0;				//��ʼ��λ��ָ�� 
		OV7725_RCK_L;
		OV7725_RCK_H;
		OV7725_RRST = 1;				//��λ��ָ����� 
		OV7725_RCK_L;
		OV7725_RCK_H;  
		int i, j;
		uint16_t color;
		
		uint8_t test = 255;
		for (i = 0; i < CAMERA_SIZE; i++) {
			color = 0;
			OV7725_RCK_L;
			color = GPIOC->IDR&0XFF;
			OV7725_RCK_H; 
			color <<= 8;  
			
			OV7725_RCK_L;
			color |= GPIOC->IDR&0XFF;
			OV7725_RCK_H;
			
			//sendCode(rgb2gray(color));
			wifiSendOneData(rgb2gray(color));
		}
		//sendCode(end_code);
		wifiSendOneData(end_code);
		ov_sta=0;					//����֡�жϱ��
	}
}

void ov7725Test() {
	// init ov7725
	u8 lightmode=0,saturation=2,brightness=2,contrast=2;
	u8 effect=0;
	
	while (OV7725_Init() != 0) {
		USART_printf(USART1, "vo7725 init failed...\n");
		delay_ms(100);
	}
	
	USART_printf(USART1, "OV7725_Init success1!");
	
	EXTI8_Init();

	OV7725_Light_Mode(lightmode);
	OV7725_Color_Saturation(saturation);
	OV7725_Brightness(brightness);
	OV7725_Contrast(contrast);
 	OV7725_Special_Effects(effect);	
	OV7725_Window_Set(CAMERA_WIDTH, CAMERA_HEIGHT, 0);
	OV7725_CS = 0;
	ov_sta = 0;
	USART_printf(USART1, "wtest cameraRefresh\n");
	while (1) {
		cameraRefresh();
		delay_ms(1);
		LED2_REV;
		LED3_REV;
	}
}

int main() {
	// init
	initLED();
	initSysTick();

	initUsart1(9600);
	USART_printf(USART1, "start test...");
	
	initESP8266();
	
	delay_ms(500);
	LED2_ON;
	LED3_OFF;
	
	ov7725Test();
	//test8266();
	return 0;
}
