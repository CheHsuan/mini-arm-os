#include <stdint.h>
#include "shell.h"
#include "reg.h"

int fibonacci(int x)
{
    if(x == 0) return 0;
    if(x == 1) return 1;
    return fibonacci(x-1) + fibonacci(x-2);
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

static int strcmp(const char *a,const char *b)
{
    while(*a != '\0' && *b != '\0') {
        if(*a > *b)
            return 1;
        else if(*a < *b)
            return -1;
        a++;
        b++;
    }
    if(*a == '\0' && *b == '\0')
        return 0;
    else if(*a == '\0' && *b != '\0')
        return -1;
    else
        return 1;
}

void commandCheck(const char *command)
{
    if(strcmp(command, "fibonacci") == 0) {
        print_str("The fibonacci sequence at ");
        print_int(12);
        print_str(" is: ");
        int i = fibonacci(12);
        print_int(i);
        print_str("\n");
    }

}

void shell()
{
    char c;
    char command[100];
    int index = 0;
    char prompt[19] = "guest@mini-arm-os$\0";
    print_str(prompt);
    while(1) {
        while (!(*(USART2_SR) & USART_SR_RXNE));
        c = *(USART2_DR);
        if(c == 13) {
            command[index] = '\0';
            index = 0;
            print_str("\n");
            commandCheck(command);
            print_str(prompt);
        } else if(c == 8 || c == 127) {
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
        } else {
            print_char(c);
            command[index++] = c;
        }
    }
}
