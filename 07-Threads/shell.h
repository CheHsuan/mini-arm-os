#define USART_SR_RXNE ((uint16_t) 0x0020)
#define USART_FLAG_TXE  ((uint16_t) 0x0080)

#define LOCKED 1
#define MAX_COMMAND_LENGTH 255

void fib();
void print_str(const char *str);
void print_char(const char c);
void print_int(int num);
void commandCheck(const char *command,volatile int *a);
int fibonacci(int num);
void shell();
