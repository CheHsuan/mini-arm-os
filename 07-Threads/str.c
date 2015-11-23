int strcmp(const char *a,const char *b)
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

void strcpy(char *a, const char *b)
{
    while(*b != '\0') {
        *a = *b;
        a++;
        b++;
    }
    *a = '\0';
}
