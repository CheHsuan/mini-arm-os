#define USART_SR_RXNE ((uint16_t) 0x0020)
#define USART_FLAG_TXE  ((uint16_t) 0x0080)

void print_str(const char *str);
void print_char(const char c);
void print_int(int num);
void commandCheck(const char *command);
int fibonacci(int num);
void shell();
