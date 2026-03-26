#include "serial.h"
#include "stm32f3xx.h"

#define AHB2_CLOCK			29491200
#define USART2_BAUD_RATE 	115200

#define GPIO_AFRH_AFSER10_AF7 	(0x7UL << (8U))
#define GPIO_AFRH_AFSER11_AF7	(0x7UL << (12U))

Status_t Usart3DMAInit(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	RCC->APB1ENR = RCC_APB1ENR_USART3EN;
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;

	//Set PB10 and PB11 as alternative function
	GPIOB->MODER |= GPIO_MODER_MODER10_1;
	GPIOB->MODER |= GPIO_MODER_MODER11_1;

	//Set alternative function AF7 for PA2 and PA3
	GPIOB->AFR[1] |= GPIO_AFRH_AFSER10_AF7;
	GPIOB->AFR[1] |= GPIO_AFRH_AFSER11_AF7;

	//PB11 open-drein
	GPIOB->OTYPER |= GPIO_OTYPER_OT_11;

	//USART baud rate 115200
	USART3->BRR = AHB2_CLOCK/USART2_BAUD_RATE;

	USART3->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;

	//IDEL interrupt
	USART3->CR1 |= USART_CR1_IDLEIE;

	NVIC_SetPriority(USART3_IRQn, 1);
	NVIC_EnableIRQ(USART3_IRQn);

	//USART enable
	USART3->CR1 |= USART_CR1_UE  | USART_CR1_TE | USART_CR1_RE;

	return STATUS_OK;
}

Status_t Usart3TransmitDMA(uint32_t addr_tx)
{
	DMA1_Channel2->CCR = 0;
	DMA1_Channel2->CPAR = (uint32_t)&USART3->TDR;
	DMA1_Channel2->CMAR = addr_tx;
	DMA1_Channel2->CNDTR = 5;

	DMA1_Channel2->CCR |= DMA_CCR_PL_1 | DMA_CCR_DIR | DMA_CCR_MINC | DMA_CCR_EN;

	return STATUS_OK;
}

Status_t Usart3ReceiverDMA(uint32_t addr_rx)
{
	DMA1_Channel3->CCR = 0;
	DMA1_Channel3->CPAR = (uint32_t)&USART3->RDR;
	DMA1_Channel3->CMAR = addr_rx;
	DMA1_Channel3->CNDTR = 5;

	DMA1_Channel3->CCR |= DMA_CCR_PL_1 | DMA_CCR_TEIE | DMA_CCR_MINC | DMA_CCR_EN;

	return STATUS_OK;
}
