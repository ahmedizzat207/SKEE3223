#include "stm32f4xx.h"

void delay_ms(int ms)
{
    for (int i = 0; i < ms * 1000; i++){
    }
}

int main(void)
{
    uint8_t button;

    RCC->AHB1ENR |= 0x05;
    GPIOC->MODER |= (0x01 << 13 * 2);
    GPIOA->MODER |= (0x01 << 3 * 2);
    GPIOA->OTYPER &= ~(0x01 << 3);  // Change to push-pull
    GPIOA->PUPDR |= 0x01;
    while(1)
    {   
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