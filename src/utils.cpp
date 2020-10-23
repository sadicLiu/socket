#include <iostream>
#include <string.h>
#include "utils.h"


// 去掉参数中的tab，回车，换行
void strip_args(char *s)
{
    size_t i;
    size_t len = strlen(s);
    size_t offset = 0;
    for (i = 0; i < len; ++i)
    {
        char c = s[i];
        if (c == '\t' || c == '\n' || c == '\r' || c == 0x0d || c == 0x0a) ++offset;
        else s[i - offset] = c;
    }
    s[len - offset] = '\0';
}