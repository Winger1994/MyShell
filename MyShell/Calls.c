//
//  Calls.c
//  MyShell
//
//  Created by 赵超 on 03/10/2016.
//  Copyright © 2016 赵超. All rights reserved.
//

#include "Calls.h"

const int callsDebug = 0;
const int INITPARAMETERNUM = 10;

int simpleCall(CommandBatch batch, int begin, int end) {
    int ret;
    char *parameters[end - begin + 2];
    memcpy(parameters, &batch.commands[begin], sizeof(char*) * (end - begin + 1));
    parameters[batch.size] = NULL;
    ret = execvp(batch.commands[begin], parameters);
    debugPrintf(callsDebug, "child ret: %d\n", ret);
    if (ret != 0)
        errorPrompt();
    return ret;
}

int fileRedirectCall(CommandBatch batch, int begin, int end) {
    int capacity = INITPARAMETERNUM;
    int size = 0;
    char **parameters = (char **)malloc(sizeof(char*) * capacity);
    int fileOpenOptions = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int i = begin;
    while (i <= end) {
        char *token = batch.commands[i];
        // the order is | , < , > , 1> , >> , 2> , &>
        int pos;
        if (!isKeyWordsMatchGetPos(token, &pos)) {
            batchAppend(&parameters, &capacity, &size, token);
            ++i;
            continue;
        }
        debugPrintf(callsDebug, "get key word pos: %d\n", pos);
        char *nextToken = i < end ? batch.commands[i + 1] : NULL;
        if (nextToken == NULL || isKeyWordsMatch(nextToken)) {
            fprintf(stderr, "[Parse Error] File Redirect FILENAME NOT GIVEN!\n");
            return -1;
        }
        int fd_in, fd_out;
        switch (pos) {
            case 0:
                fprintf(stderr, "[Function Error] isKeyWordsMatchGetPos error!\n");
                break;
            case 1:
                fd_in = open(nextToken, O_RDONLY);
                dup2(fd_in, 0);
                close(fd_in);
                break;
            case 2:
            case 3:
                fd_out = open(nextToken, O_CREAT | O_RDWR | O_TRUNC, fileOpenOptions);
                dup2(fd_out, 1);
                close(fd_out);
                break;
            case 4:
                fd_out = open(nextToken, O_CREAT | O_RDWR | O_APPEND, fileOpenOptions);
                dup2(fd_out, 1);
                close(fd_out);
                break;
            case 5:
                fd_out = open(nextToken, O_CREAT | O_RDWR | O_TRUNC, fileOpenOptions);
                dup2(fd_out, 2);
                close(fd_out);
                break;
            case 6:
                fd_out = open(nextToken, O_CREAT | O_RDWR | O_TRUNC, fileOpenOptions);
                dup2(fd_out, 1);
                dup2(fd_out, 2);
                close(fd_out);
                break;
            default:
                fprintf(stderr, "[Funtion Error] isKeyWordsMatch Wrong!\n");
                return -1;
        }
        i += 2;
    }
    batchAppend(&parameters, &capacity, &size, NULL);
    int ret = execvp(parameters[0], parameters);
    debugPrintf(callsDebug, "child ret: %d\n", ret);
    if (ret != 0)
        errorPrompt();
    return ret;
}

int pipeCall(CommandBatch batch, int begin, int end) {
    if (begin > end) {
        fprintf(stderr, "[Parse Error] During Pipe Parse!\n");
        return -1;
    }
    int pipePos = begin;
    while (pipePos <= end) {
        if (isStringEqual(batch.commands[pipePos], "|")) {
            break;
        }
        ++pipePos;
    }
    if (pipePos > end) {
        return fileRedirectCall(batch, begin, end);
    }
    if (callsDebug) {
        printf("before pipe: ");
        printBatchInterval(batch, begin, pipePos - 1);
        printf("after pipe: ");
        printBatchInterval(batch, pipePos + 1, end);
    }
    int pipeArr[2];
    if (pipe(pipeArr)) {
        fprintf (stderr, "[Pipe Error] Pipe failed.\n");
        return -1;
    }
    int pid = fork();
    switch (pid) {
        case -1:
            fprintf(stderr, "[Fork Error] Pipe Fork Failed!\n");
            return 1;
        case 0: // child process write
            close(pipeArr[0]);
            close(1);
            dup2(pipeArr[1], 1);
            close(pipeArr[1]);
            return fileRedirectCall(batch, begin, pipePos - 1);
        default: // parent process read
            close(pipeArr[1]);
            close(0);
            dup2(pipeArr[0], 0);
            close(pipeArr[0]);
            return pipeCall(batch, pipePos + 1, end);
    }
}
