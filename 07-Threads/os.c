#include <stddef.h>
#include <stdint.h>
#include "reg.h"
#include "threads.h"
#include "shell.h"
#include "str.h"

/* USART TXE Flag
 * This flag is cleared when data is written to USARTx_DR and
 * set when that data is transferred to the TDR
 */
#define USART_FLAG_TXE	((uint16_t) 0x0080)

void usart_init(void)
{
    *(RCC_APB2ENR) |= (uint32_t) (0x00000001 | 0x00000004);
    *(RCC_APB1ENR) |= (uint32_t) (0x00020000);

    /* USART2 Configuration, Rx->PA3, Tx->PA2 */
    *(GPIOA_CRL) = 0x00004B00;
    *(GPIOA_CRH) = 0x44444444;
    *(GPIOA_ODR) = 0x00000000;
    *(GPIOA_BSRR) = 0x00000000;
    *(GPIOA_BRR) = 0x00000000;

    *(USART2_CR1) = 0x0000000C;
    *(USART2_CR2) = 0x00000000;
    *(USART2_CR3) = 0x00000000;
    *(USART2_CR1) |= 0x2000;
}

/* 72MHz */
#define CPU_CLOCK_HZ 72000000

/* 100 ms per tick. */
#define TICK_RATE_HZ 10

int main(void)
{
    threadInfo shellinfo;
    strcpy(shellinfo.name, "shell");
    shellinfo.priority = 10;

    usart_init();

    if (thread_create(shell, (void *) &shellinfo) == -1)
        print_str("Shell creation failed\r\n");

    /* SysTick configuration */
    *SYSTICK_LOAD = (CPU_CLOCK_HZ / TICK_RATE_HZ) - 1UL;
    *SYSTICK_VAL = 0;
    *SYSTICK_CTRL = 0x07;

    thread_start();

    return 0;
}
