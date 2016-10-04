//
//  CommandBatch.c
//  MyShell
//
//  Created by 赵超 on 02/10/2016.
//  Copyright © 2016 赵超. All rights reserved.
//

#include "CommandBatch.h"

const int batchDebug = 0;

void lastFilenameInputPatch(char **batch, int *capacity, int *size) {
    debugPrintf(batchDebug, "last file name input capacity: %d size: %d\n", *capacity, *size);
    if (*size > 0 && isKeyWordsMatch(batch[*size - 1])) {
        printf("> ");
        int bufferCap = 20;
        char *buffer = (char*) malloc(sizeof(char) * bufferCap);
        getString(buffer, &bufferCap, 0);
        int pos = 0;
        char *filename = nextToken(buffer, " \t\n", &pos);
        debugPrintf(batchDebug, "filename: %s\n", filename);
        free(buffer);
        batchAppend(&batch, capacity, size, filename);
    }
}

CommandBatch generateBatch(char *command) {
    char *delims = " \t\n";
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
    lastFilenameInputPatch(batch, &capacity, &size);
    CommandBatch res;
    res.size = size;
    res.commands = batch;
    if (batchDebug) printCommandBatch(res);
    return res;
}

void freeBatch(CommandBatch batch) {
    int i = 0;
    for (; i < batch.size; ++i) {
        free(batch.commands[i]);
    }
    free(batch.commands);
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
