#include "stm32f4xx.h"

uint16_t ADC_Cval;
uint8_t UART_MSG;

void delay_ms(int ms)
{
    for (int i = 0; i < ms * 1000; i++){
    }
}

uint16_t ADC_READ(void)
{
    ADC1->CR2 |= (1 << 30);
    while (!(ADC1->SR & 0x02))
    {
        /* code */
    }
    return ADC1->DR;
}

uint8_t UART_READ(void)
{
    while (!(USART1->SR & 0x0020))
    {
    }
    return USART1->DR;
}

void UART_SEND(uint8_t UART_MSG)
{
    while (!(USART1->SR & 0x0080)){
    
    }
    USART1->DR = UART_MSG;
}

int main(void)
{
    uint8_t button;

    RCC->AHB1ENR |= 0x07;
    RCC->APB2ENR |= (0X01 << 8) | (0x01 << 4);
    GPIOC->MODER |= (0x01 << 13 * 2);
    GPIOA->MODER |= (0x01 << 3 * 2);
    GPIOB->MODER |= (0x03 << 2);
    ADC1->CR2 |= 0x01;
    ADC1->SMPR1 |= (0x07 << 3 * 9);
    ADC1->SQR1 &= ~(0x0F << 20);
    ADC1->SQR3 |= (uint32_t) 9;
    GPIOA->OTYPER &= ~(0x01 << 3);  // Change to push-pull
    GPIOA->PUPDR |= 0x01;
    GPIOA->MODER |= (0x02 << 9 * 2) | (0x02 << 10 * 2);
    GPIOA->AFR[1] |= (0x07 << 1 * 4) | (0x07 << 2 * 4);
    USART1->BRR = 0x683;
    USART1->CR1 |= 0x0C;
    USART1->CR2 |= 0x00;
    USART1->CR1 |= 0x2000;

    while(1)
    {   
        ADC_Cval = ADC_READ();
        UART_MSG = UART_READ();
        UART_SEND(UART_MSG);
        button = GPIOA->IDR &= 0x01;
        if (button == 0) {
            GPIOC->BSRR = (0X01 << 13);
            GPIOA->BSRR = (0x01 << 19);
        }
        else {
            GPIOC->ODR ^= (0x01 << 13);
            GPIOA->ODR ^= (0x01 << 3);
            delay_ms(500);
        }

    }
}