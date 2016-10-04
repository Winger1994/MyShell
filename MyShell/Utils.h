//
//  Utils.h
//  MyShell
//
//  Created by 赵超 on 03/10/2016.
//  Copyright © 2016 赵超. All rights reserved.
//

#ifndef Utils_h
#define Utils_h
#include "stdio.h"
#include "stdarg.h"
#include "string.h"
#include "stdlib.h"
#include "errno.h"

typedef int bool;

int getString(char *buffer, int *capacity, int allowEmpty);
bool isStringBelong(char *string, const char **group, int size);
bool isStringEqual(const char *a, const char *b);

int isKeyWordsMatch(char *string);
int isKeyWordsMatchGetPos(char *string, int *pos);

void doubelStringCapacity(char **string, int *capacity);
void stringAppend(char **string, int *capacity, int *size, char content);

char *nextToken(char *command, const char *delims, int *pos);
void doubleBatchCapacity(char ***batch, int *capacity);
void batchAppend(char ***batch, int *capacity, int *size, char *content);

void debugPrintf(int level, const char *fmt, ...);
void errorPrompt();

#endif /* Utils_h */
