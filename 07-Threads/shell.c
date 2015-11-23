#include <stdint.h>
#include "threads.h"
#include "shell.h"
#include "str.h"
#include "reg.h"

extern int fibonacci(int x);

int TestAndSet(volatile int *lockPtr)
{
    int oldValue;
    oldValue = *lockPtr;
    *lockPtr = LOCKED;
    return oldValue;
}

void fib(void *threaddata)
{
    threadInfo *data = (threadInfo *)threaddata;
    int result = fibonacci(12);
    while(TestAndSet(data->mutexlock) == 1);
    print_str("The fibonacci sequence at ");
    print_int(12);
    print_str(" is: ");
    print_int(result);
    print_str("\n");
    data->mutexlock = 0;
}

void print_str(const char *str)
{
    while (*str) {
        while (!(*(USART2_SR) & USART_FLAG_TXE));
        *(USART2_DR) = (*str & 0xFF);
        str++;
    }
}

void print_char(const char character)
{
    while (!(*(USART2_SR) & USART_FLAG_TXE));
    *(USART2_DR) = (character & 0xFF);
}

void print_int(int num)
{
    char intArray[10];
    int i;
    for(i = 9; i >= 0 ; i--) {
        intArray[i] = num % 10 + 48;
        num = num / 10;
        if(num == 0)
            break;
    }
    print_str(&intArray[i]);
}

char get_char()
{
    while (!(*(USART2_SR) & USART_SR_RXNE));
    return *(USART2_DR);
}

void commandCheck(const char *command, volatile int *mutexlock)
{
    threadInfo commandInfo;
    if(strcmp(command, "fibonacci") == 0) {
        strcpy(commandInfo.name,"fibonacci");
        commandInfo.priority = 5;
        commandInfo.mutexlock = mutexlock;
        if(thread_create(fib, (void *) &commandInfo) == -1)
            print_str("fibonacci thread creation failed\r\n");
    }
}

void shell()
{
    char c;
    char command[MAX_COMMAND_LENGTH];
    char history[MAX_COMMAND_LENGTH];
    int index = 0;
    char *prompt = "guest@mini-arm-os$\0";
    volatile int mutexlock = 0;

    print_str(prompt);

    while(1) {
        c = get_char();
        switch(c) {
            // enter
            case 13:
                command[index] = '\0';
                strcpy(history, command);
                index = 0;
                print_str("\n");
                commandCheck(command,&mutexlock);
                while(TestAndSet(&mutexlock) == 1);
                print_str(prompt);
                mutexlock = 0;
                break;
            // backspace
            case 127:
                if(index > 0) {
                    command[index-1] = ' ';
                    command[index] = '\0';
                    print_str("\r");
                    print_str(prompt);
                    print_str(command);
                    index--;
                    command[index] = '\0';
                    print_str("\r");
                    print_str(prompt);
                    print_str(command);
                }
                break;
            default:
                print_char(c);
                command[index++] = c;
        }
    }
}
