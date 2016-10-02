//
//  CommandBatch.c
//  MyShell
//
//  Created by 赵超 on 02/10/2016.
//  Copyright © 2016 赵超. All rights reserved.
//

#include "CommandBatch.h"

int batchDebug = 0;

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
    char *token = strtok(command, delims);
    while (token != NULL) {
        if (size == capacity) {
            batch = doubleCapacity(batch, &capacity);
        }
        batch[size++] = token;
        token = strtok(NULL, delims);
    }
    CommandBatch res;
    res.size = size;
    res.commands = batch;
    return res;
}

void printCommandBatch(CommandBatch batch) {
    int i = 0;
    for (; i < batch.size; ++i) {
        printf("%s\n", batch.commands[i]);
    }
}
