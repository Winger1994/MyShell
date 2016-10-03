//
//  Utils.c
//  MyShell
//
//  Created by 赵超 on 03/10/2016.
//  Copyright © 2016 赵超. All rights reserved.
//

#include "Utils.h"

const int utilsDebug = 0;

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

bool isBelong(char c, const char *group) {
    int i = 0;
    while (group[i] != 0) {
        if (c == group[i++]) {
            debugPrintf(utilsDebug, "%c belongs to %s\n", c, group);
            return true;
        }
    }
    return false;
}

char *nextToken(char *command, const char *delims, int *pos) {
    while (command[*pos] != 0 && isBelong(command[*pos], delims))
        ++(*pos);
    if (command[*pos] == 0)
        return NULL;
    int begin = *pos;
    while (command[*pos] != 0 && !isBelong(command[*pos], delims))
        ++(*pos);
    if (command[*pos] != 0)
        command[(*pos)++] = 0;
    return &command[begin];
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
