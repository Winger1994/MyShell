//
//  CommandBatch.c
//  MyShell
//
//  Created by 赵超 on 02/10/2016.
//  Copyright © 2016 赵超. All rights reserved.
//

#include "CommandBatch.h"

const int batchDebug = 0;

CommandBatch generateBatch(char *command) {
    char delims[] = {' ', '\t'};
    int capacity = 10;
    int size = 0;
    char **batch = (char **)malloc(sizeof(char*) * capacity);
    int pos = 0;
    char *token = nextToken(command, delims, &pos);
    if (token != NULL) debugPrintf(batchDebug, "next token is %s\n", token);
    while (token != NULL) {
        batchAppend(&batch, &capacity, &size, token);
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
    printBatchInterval(batch, 0, batch.size - 1);
}

void printBatchInterval(CommandBatch batch, int begin, int end) {
    while (begin <= end) {
        printf("%s ", batch.commands[begin++]);
    }
    printf("\n");
}
