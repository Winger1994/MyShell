//
//  Utils.c
//  MyShell
//
//  Created by 赵超 on 03/10/2016.
//  Copyright © 2016 赵超. All rights reserved.
//

#include "Utils.h"

const bool true = 1;
const bool false = 0;

int getString(char *buffer, int size) {
    int i = 0;
    for (; i < size; ++i) {
        char tmp = getchar();
        if (tmp == '\n') {
            buffer[i] = 0;
            return i;
        }
        buffer[i] = tmp;
    }
    return size;
}

bool isStringEqual(char *a, char *b) {
    while (*a == *b) {
        if (*a == 0)
            return true;
        ++a; ++b;
    }
    return false;
}

void debugPrintf(int level, const char *fmt, ...) {
    if (level) {
        printf("[debug] ");
        va_list argptr;
        va_start(argptr, fmt);
        vprintf(fmt, argptr); // not call to printf!
        va_end(argptr);
    }
}
