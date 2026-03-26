#include "timer.h"
#include "stm32f3xx.h"

Status_t Tim1InitPWM(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

	TIM1->PSC = 19;
	TIM1->ARR = 99;

	TIM1->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1PE;
	TIM1->CCMR1 |= TIM_CCMR1_OC2M_0 |TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2PE;
	TIM1->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3PE;
	TIM1->CCMR2 |= TIM_CCMR2_OC4M_0 |TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4PE;

	TIM1->CCER |= TIM_CCER_CC1E;
	TIM1->CCER |= TIM_CCER_CC2E;
	TIM1->CCER |= TIM_CCER_CC3E;
	TIM1->CCER |= TIM_CCER_CC4E;

	TIM1->CR1 |= TIM_CR1_ARPE;

	TIM1->BDTR |= TIM_BDTR_MOE;

	TIM1->EGR |= TIM_EGR_UG;

	TIM1->CCR1 = 0;
	TIM1->CCR2 = 0;
	TIM1->CCR3 = 0;
	TIM1->CCR4 = 0;

	return STATUS_OK;
}

Status_t Tim1Start(void)
{
	TIM1->CR1 |= TIM_CR1_CEN;

	return STATUS_OK;
}

Status_t Tim1HighRightTranssistor(uint16_t duty)
{
	TIM1->CCR1 = duty;

	return STATUS_OK;
}

Status_t Tim1LowRightTranssistor(uint16_t duty)
{
	TIM1->CCR2 = duty;

	return STATUS_OK;
}

Status_t Tim1HighLeftTranssistor(uint16_t duty)
{
	TIM1->CCR3 = duty;

	return STATUS_OK;
}

Status_t Tim1LowLeftTranssistor(uint16_t duty)
{
	TIM1->CCR4 = duty;

	return STATUS_OK;
}

Status_t Tim2InitPWM(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	TIM2->PSC = 19;
	TIM2->ARR = 99;

	TIM2->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1PE;
	TIM2->CCMR1 |= TIM_CCMR1_OC2M_0 |TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2PE;
	TIM2->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3PE;
	TIM2->CCMR2 |= TIM_CCMR2_OC4M_0 |TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4PE;

	TIM2->CR1 |= TIM_CR1_ARPE;

	TIM2->CCER |= TIM_CCER_CC1E;
	TIM2->CCER |= TIM_CCER_CC2E;
	TIM2->CCER |= TIM_CCER_CC3E;
	TIM2->CCER |= TIM_CCER_CC4E;

	TIM2->EGR |= TIM_EGR_UG;

	TIM2->CCR1 = 0;
	TIM2->CCR2 = 0;
	TIM2->CCR3 = 0;
	TIM2->CCR4 = 0;

	return STATUS_OK;
}

Status_t Tim2Start(void)
{
	TIM2->CR1 |= TIM_CR1_CEN;

	return STATUS_OK;
}

Status_t Tim2HighRightTranssistor(uint16_t duty)
{
	TIM2->CCR1 = duty;

	return STATUS_OK;
}

Status_t Tim2LowRightTranssistor(uint16_t duty)
{
	TIM2->CCR2 = duty;

	return STATUS_OK;
}

Status_t Tim2HighLeftTranssistor(uint16_t duty)
{
	TIM2->CCR3 = duty;

	return STATUS_OK;
}

Status_t Tim2LowLeftTranssistor(uint16_t duty)
{
	TIM2->CCR4 = duty;

	return STATUS_OK;
}

Status_t Tim3InitEncoderMode(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	TIM3->SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;
	TIM3->CCMR1 |= TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0;
	TIM3->CR1 |= TIM_CR1_CEN;

	return STATUS_OK;
}

Status_t Tim3Start(void)
{
	TIM3->CR1 |= TIM_CR1_CEN;

	return STATUS_OK;
}

int16_t Tim3GetValue(void)
{
	int16_t cnt = (int16_t)TIM3->CNT;
	TIM3->CNT = 0;

	return cnt;
}

Status_t Tim4InitEncoderMode(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

	TIM4->SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;
	TIM4->CCMR1 |= TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC2S_0;
	TIM4->CR1 |= TIM_CR1_CEN;

	return STATUS_OK;
}

Status_t Tim4Start(void)
{
	TIM4->CR1 |= TIM_CR1_CEN;

	return STATUS_OK;
}

int16_t Tim4GetValue(void)
{
	int16_t cnt = (int16_t)TIM4->CNT;
	TIM4->CNT = 0;

	return cnt;
}

Status_t Tim6Init(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

	TIM6->PSC = 99;
	TIM6->ARR = 49151;
	TIM6->SR = 0;

	//TIM6->EGR |=TIM_EGR_UG;event dla adc do poprawy

	return STATUS_OK;
}

Status_t Tim6Start(void)
{
	TIM6->CR1 |= TIM_CR1_CEN;

	return STATUS_OK;
}

Status_t Tim7Init(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;

	TIM7->PSC = 59;
	TIM7->ARR = 49151;
	TIM7->SR = 0;

	TIM7->DIER |= TIM_DIER_UIE;

	NVIC_SetPriority(TIM7_IRQn, 2);
	NVIC_EnableIRQ(TIM7_IRQn);

	return STATUS_OK;
}

Status_t Tim7Start(void)
{
	TIM7->CR1 |= TIM_CR1_CEN;

	return STATUS_OK;
}
