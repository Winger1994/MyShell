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

bool isCharBelong(char c, const char *group) {
    int i = 0;
    while (group[i] != 0) {
        if (c == group[i++]) {
            debugPrintf(utilsDebug, "%c belongs to %s\n", c, group);
            return true;
        }
    }
    return false;
}

int getString(char *buffer, int *capacity, int allowEmpty) {
    int size = 0;
    int validSize = 0;
    int singleQuote = 0, doubleQuote = 0;
    while (true) {
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
                    if (validSize != 0 || allowEmpty) {
                        buffer[size] = 0;
                        return size;
                    }
                }
                printf("> ");
                break;
            default:
                break;
        }
        stringAppend(&buffer, capacity, &size, ch);
        if (!isCharBelong(ch, " \t\n"))
            ++validSize;
    }
}

bool isStringEqual(const char *a, const char *b) {
    while (*a == *b) {
        if (*a == 0)
            return true;
        ++a; ++b;
    }
    return false;
}

bool isStringMatch(const char *string, const char *pattern) {
    while (*pattern != 0) {
        if (*string != *pattern)
            return false;
        ++string; ++pattern;
    }
    return true;
}

bool isStringBelong(char *string, const char **group, int size) {
    int i = 0;
    for (; i < size; ++i) {
        if (isStringEqual(string, group[i]))
            return true;
    }
    return false;
}

int isKeyWordsMatch(char *string) {
    const int dosNum = 4;
    const char *unoKeyWords = "|<>";
    char *dosKeyWords[] = {">>", "1>", "2>", "&>"};
    if (isCharBelong(*string, unoKeyWords))
        return 1;
    int i = 0;
    for(; i < dosNum; ++i) {
        if (isStringMatch(string, dosKeyWords[i]))
            return 2;
    }
    return 0;
}

// TODO: This function is seriously flawed
void doubelStringCapacity(char **string, int *capacity) {
    int newCap = *capacity * 2;
    char *newStr = (char *)malloc(sizeof(char) * newCap);
    memcpy(newStr, *string, sizeof(char) * (*capacity));
    free(*string);
    debugPrintf(utilsDebug, "double string capacity from %d to %d\n", *capacity, newCap);
    *capacity = newCap;
    *string = newStr;
}

void stringAppend(char **string, int *capacity, int *size, char content) {
    if (*size == *capacity) {
        doubelStringCapacity(string, capacity);
    }
    (*string)[(*size)++] = content;
}


// TODO: Need use stack to correctly match quotes
char *nextToken(char *command, const char *delims, int *pos) {
    int capacity = 10, size = 0;
    char *res = (char*)malloc(capacity);
    while (command[*pos] != 0 && isCharBelong(command[*pos], delims))
        ++(*pos);
    if (command[*pos] == 0)
        return NULL;
    bool inSingleQuote = false;
    bool inDoubleQuote = false;
    while (command[*pos] != 0) {
        if (!inSingleQuote && !inDoubleQuote) {
            if (isCharBelong(command[*pos], delims))
                break;
            int keyLen = isKeyWordsMatch(command + *pos);
        }
        switch (command[*pos]) {
            case '\'':
                inSingleQuote = !inSingleQuote;
                break;
            case '\"':
                inDoubleQuote = !inDoubleQuote;
                break;
            default:
                stringAppend(&res, &capacity, &size, command[*pos]);
                break;
        }
        ++(*pos);
    }
    return res;
}

void doubleBatchCapacity(char ***batch, int *capacity) {
    int newCap = *capacity * 2;
    char **newBatch = (char **)malloc(sizeof(char*) * newCap);
    memcpy(newBatch, *batch, sizeof(char*) * (*capacity));
    free(*batch);
    debugPrintf(utilsDebug, "double batch capacity from %d to %d\n", *capacity, newCap);
    *capacity = newCap;
    *batch = newBatch;
}

void batchAppend(char ***batch, int *capacity, int *size, char *content) {
    if (*size == *capacity) {
        doubleBatchCapacity(batch, capacity);
    }
    (*batch)[(*size)++] = content;
}

void debugPrintf(int level, const char *fmt, ...) {
    if (level) {
        printf("[DEBUG] ");
        va_list argptr;
        va_start(argptr, fmt);
        vprintf(fmt, argptr); // not call to printf!
        va_end(argptr);
    }
}

void errorPrompt() {
    if (errno)
        fprintf(stderr, "[Error]: ");
    switch (errno) {
        case 0:
            fprintf(stderr, "[Function Error] Wrong Call to Error Prompt!\n");
            break;
        case EACCES:
            fprintf(stderr, "Permission Denied!\n");
            break;
        case ELOOP:
            fprintf(stderr, "Symbloc Loops!\n");
            break;
        case ENOENT:
            fprintf(stderr, "File not Found!\n");
            break;
        case EFAULT:
            fprintf(stderr, "Bad Address!\n");
        default:
            fprintf(stderr, "Something Wrong! Error Number: %d\n", errno);
            break;
    }
}
