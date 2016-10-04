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
    int singleQuote = 0, doubleQuote = 0;
    for (; i < size; ++i) {
        char ch = getchar();
        switch (ch) {
            case EOF:
                exit(0);
            case '\'':
                ++singleQuote;
                break;
            case '\"':
                ++doubleQuote;
                break;
            case '\n':
                if (singleQuote % 2 == 0 && doubleQuote % 2 == 0) {
                    buffer[i] = 0;
                    return i;
                } else {
                    buffer[i] = '\n';
                    printf("> ");
                    break;
                }
            default:
                buffer[i] = ch;
                break;
        }
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

char **doubleCapacity(char **batch, int *capacity) {
    int newCap = *capacity * 2;
    char **newBatch = (char **)malloc(sizeof(char*) * newCap);
    memcpy(newBatch, batch, sizeof(char*) * (*capacity));
    free(batch);
    debugPrintf(utilsDebug, "double batch capacity from %d to %d\n", *capacity, newCap);
    *capacity = newCap;
    return newBatch;
}

void stringGoupAppend(char ***group, int *capacity, int *size, char *content) {
    if (*size == *capacity) {
        *group = doubleCapacity(*group, capacity);
    }
    (*group)[(*size)++] = content;
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

void errorPrompt() {
    fprintf(stderr, "Error: ");
    switch (errno) {
        case 0:
            fprintf(stderr, "Wrong Call to Error Prompt!\n");
            break;
        case EACCES:
            fprintf(stderr, "Permission Denied!\n");
            break;
        case ELOOP:
            fprintf(stderr, "Symbloc Loops!\n");
            break;
        case ENOENT:
            fprintf(stderr, "No Such File or Directory!\n");
            break;
        case EFAULT:
            fprintf(stderr, "Bad Address!\n");
        default:
            fprintf(stderr, "Something Wrong! Error Number: %d\n", errno);
            break;
    }
}
