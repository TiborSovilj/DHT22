#include <stdio.h>
#include <stdint.h>
#include "../Inc/stm32f4xx.h"
#include "../Inc/syncTimes.h"

/** functions **/ 
void sendString(char* buffer);
void RCC_init();
void USART_init();
void Timer_init();
void send3Seconds();
void collectData();

/** pointers - constants **/
USART_TypeDef* usart = USART1;
TIM_TypeDef* timer = TIM6;
GPIO_TypeDef* gpiod = GPIOD;
GPIO_TypeDef* gpioa = GPIOA;

/** vars **/
double humidity = 0.0; 
double temperature = 0.0;
char buffer[100];
int state = 0;
int data = 0;  
uint8_t dataArray[5] = {0,0,0,0,0};


int main()
{
	RCC_init();
	SET_BIT(gpiod->MODER, 0x1);
	USART_init();
	Timer_init();

	while(1){
		SET_BIT(gpiod->MODER, 0x1);
		CLEAR_BIT(gpiod->ODR, 0x1);	
		SET_BIT(timer->CR1, 0x1);                //CEN - counter enable 1

		while((READ_BIT(timer->SR, 0x1) & 1) == 0);
		SET_BIT(gpiod->ODR, 0x1);
		CLEAR_BIT(gpiod->MODER, 0x1);
    	  
		CLEAR_BIT(timer->CR1, 0x1);
		CLEAR_BIT(timer->SR, 0x1);
    	   
		while((READ_BIT(gpiod->IDR, 0x1) & 1) == 1);
		while((READ_BIT(gpiod->IDR, 0x1) & 1) == 0);
		while((READ_BIT(gpiod->IDR, 0x1) & 1) == 1);
      
		collectData();
    
		sprintf(buffer,"Temp: %.2f Hum: %.2f \n",temperature,humidity);  
		sendString(buffer);
		
		SET_BIT(gpiod->MODER, 0x1);
		SET_BIT(gpiod->ODR, 0x1);
		
		send3Seconds();
	}
}


void collectData(){
	for (int i = 0; i < 5; i++){
		for (int j = 0; j < 8; j++){
			while((READ_BIT(gpiod->IDR, 0x1) & 1) == 0);
			CLEAR_BIT(timer->CNT, 0xFFFF);
			SET_BIT(timer->CR1, 0x1);	
			while((READ_BIT(gpiod->IDR, 0x1) & 1) == 1);
			if((READ_BIT(timer->CNT, 0xFFFF) & 0xFFFF) < 31)
				dataArray[i] = dataArray[i] << 1;
			else
				dataArray[i] = (dataArray[i] << 1) | 1;
			CLEAR_BIT(timer->CR1, 0x1);
		}
	}
        printf ("%x\n", (dataArray[0]+dataArray[1]+dataArray[2]+dataArray[3]) & 0xFF);
        printf ("%x\n", dataArray[4]);

	

	if ((dataArray[2] & 0x80) == 0x80){
		int16_t t;
		t = (int16_t)(dataArray[2] & 0x7F);
		t = t << 8;
		t = t | dataArray[3];
		temperature = (double) t;
		temperature = temperature/10 * (-1);
	}
	else{
		temperature = (double) ((dataArray[2] << 8) | dataArray[3])/10;
	}
	humidity = (double) ((dataArray[0] << 8) | dataArray[1]) / 10;
        
}       


void send3Seconds(){
	for(int i=0; i < 3000; i++){          //posiljanje vsake 3s
		CLEAR_BIT(timer->SR, 0x1);
		SET_BIT(timer->CR1, 0x1);
		while((READ_BIT(timer->SR,0x1) & 1) == 0);
		CLEAR_BIT(timer->SR, 0x1);
		CLEAR_BIT(timer->CR1, 0x1);
	}
}

void sendString(char* buffer){
	int i = 0;
	while(buffer[i]!='\0'){
		WRITE_REG(usart->DR, buffer[i]);
		while((READ_BIT(usart->SR, 0x80) & 0x80) == 0);
		i++;
	}
}


void RCC_init(){
	RCC_TypeDef* rcc = RCC;
	SET_BIT(rcc->AHB1ENR, RCC_AHB1ENR_GPIODEN);		// dht22
	SET_BIT(rcc->APB1ENR, RCC_APB1ENR_TIM6EN);		// timer
	SET_BIT(rcc->AHB1ENR, RCC_AHB1ENR_GPIOAEN);		// USART_GPIO
	SET_BIT(rcc->APB2ENR, RCC_APB2ENR_USART1EN);	        // USART
}

void USART_init(){
	SET_BIT(gpioa->MODER, 0x00280000);
	SET_BIT(gpioa->AFR[1], 0x00000770);		
	SET_BIT(usart->CR1, 0x3008);
	WRITE_REG(usart->BRR, 0x683); 		// 8B => 115200
}
void Timer_init(){
	SET_BIT(timer->PSC, 0x000F);	        // 15 
	CLEAR_BIT(timer->ARR, 0x0000FFFF);
	SET_BIT(timer->ARR, 0x000003E7);	// 1000-1 = 999(10) = 3E7(16)
	SET_BIT(timer->EGR, 0x0001);
	CLEAR_BIT(timer->SR, 0x0001);
}