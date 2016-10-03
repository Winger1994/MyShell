//
//  CommandBatch.c
//  MyShell
//
//  Created by 赵超 on 02/10/2016.
//  Copyright © 2016 赵超. All rights reserved.
//

#include "CommandBatch.h"

const int batchDebug = 0;

char **doubleCapacity(char **batch, int *capacity) {
    int newCap = *capacity * 2;
    char **newBatch = (char **)malloc(sizeof(char*) * newCap);
    memcpy(newBatch, batch, sizeof(char*) * (*capacity));
    free(batch);
    debugPrintf(batchDebug, "double batch capacity from %d to %d\n", *capacity, newCap);
    *capacity = newCap;
    return newBatch;
}

CommandBatch generateBatch(char *command) {
    char delims[] = {' ', '\t', '\n'};
    int capacity = 10;
    int size = 0;
    char **batch = (char **)malloc(sizeof(char*) * capacity);
    int pos = 0;
    char *token = nextToken(command, delims, &pos);
    if (token != NULL) debugPrintf(batchDebug, "next token is %s\n", token);
    while (token != NULL) {
        if (size == capacity) {
            batch = doubleCapacity(batch, &capacity);
        }
        batch[size++] = token;
        token = nextToken(command, delims, &pos);
        if (token != NULL) debugPrintf(batchDebug, "next token is %s\n", token);
    }
    CommandBatch res;
    res.size = size;
    res.commands = batch;
    if (batchDebug) printCommandBatch(res);
    return res;
}

void printCommandBatch(CommandBatch batch) {
    int i = 0;
    for (; i < batch.size; ++i) {
        printf("%s\n", batch.commands[i]);
    }
}
