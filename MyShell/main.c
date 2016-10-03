//
//  main.c
//  MyShell
//
//  Created by 赵超 on 02/10/2016.
//  Copyright © 2016 赵超. All rights reserved.
//

#include "unistd.h"
#include "fcntl.h"
#include "CommandBatch.h"
#include "Utils.h"

const int BUFFERSIZE = 1024;
const int INITPARAMETERNUM = 10;
const char *prompt = "ve482sh $ ";
const int mainDebug = 0;

int simpleCall(CommandBatch batch, int begin, int end) {
    int ret;
    char *parameters[end - begin + 2];
    memcpy(parameters, &batch.commands[begin], sizeof(char*) * (end - begin + 1));
    parameters[batch.size] = NULL;
    ret = execvp(batch.commands[0], parameters);
    debugPrintf(mainDebug, "[child] ret: %d\n", ret);
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
        char *nextToken = i < end ? batch.commands[i + 1] : NULL;
        if (isStringEqual(token, "<")) {
            int fd_in = open(nextToken, O_RDONLY);
            dup2(fd_in, 0);
            close(fd_in);
        } else if (isStringEqual(token, ">") || isStringEqual(token, "1>")) {
            int fd_out = open(nextToken, O_CREAT | O_RDWR | O_TRUNC, fileOpenOptions);
            dup2(fd_out, 1);
            close(fd_out);
        } else if (isStringEqual(token, ">>")) {
            int fd_out = open(nextToken, O_CREAT | O_RDWR | O_APPEND, fileOpenOptions);
            dup2(fd_out, 1);
            close(fd_out);
        } else if (isStringEqual(token, "2>")) {
            int fd_out = open(nextToken, O_CREAT | O_RDWR | O_TRUNC, fileOpenOptions);
            dup2(fd_out, 2);
            close(fd_out);
        } else if (isStringEqual(token, "&>")) {
            int fd_out = open(nextToken, O_CREAT | O_RDWR | O_TRUNC, fileOpenOptions);
            dup2(fd_out, 1);
            dup2(fd_out, 2);
            close(fd_out);
        } else {
            stringGoupAppend(&parameters, &capacity, &size, token);
            --i;
        }
        i += 2;
    }
    stringGoupAppend(&parameters, &capacity, &size, NULL);
    int ret = execvp(batch.commands[begin], parameters);
    debugPrintf(mainDebug, "[child] ret: %d\n", ret);
    if (ret != 0)
        errorPrompt();
    return ret;
}

int pipeCall(CommandBatch batch, int begin, int end) {
    if (begin > end) {
        fprintf(stderr, "Parse Error!\n");
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
    if (mainDebug) {
        printf("before pipe: ");
        printBatchInterval(batch, begin, pipePos - 1);
        printf("after pipe: ");
        printBatchInterval(batch, pipePos + 1, end);
    }
    int pipeArr[2];
    if (pipe(pipeArr)) {
        fprintf (stderr, "Pipe failed.\n");
        return -1;
    }
    int pid = fork();
    switch (pid) {
        case -1:
            fprintf(stderr, "FORK ERROR!\n");
            return 1;
        case 0: // child process write
            close(pipeArr[0]);
            dup2(pipeArr[1], 1);
            close(pipeArr[1]);
            return fileRedirectCall(batch, begin, pipePos - 1);
        default: // parent process read
            close(pipeArr[1]);
            dup2(pipeArr[0], 0);
            close(pipeArr[0]);
            return pipeCall(batch, pipePos + 1, end);
    }
}

void changeDir(const char *path) {
    int ret = chdir(path);
    if (ret != 0)
        errorPrompt();
}

char *addEssentialSpaces(char *inputBuffer);

int main(int argc, const char * argv[]) {
    char *inputBuffer = (char*) malloc(sizeof(char) * BUFFERSIZE);
    printf("%s", prompt);
    getString(inputBuffer, BUFFERSIZE);
    int pid, status;
    while (!isStringEqual("exit", inputBuffer)) {
        // inputBuffer = addEssentialSpaces(inputBuffer);
        debugPrintf(mainDebug, "buffer: %s\n", inputBuffer);
        CommandBatch batch = generateBatch(inputBuffer);
        if (batch.size == 0)
            goto next;
        if (isStringEqual(batch.commands[0], "cd")) {
            changeDir(batch.commands[1]);
            goto next;
        }
        pid = fork();
        switch (pid) {
            case -1:
                fprintf(stderr, "FORK ERROR!\n");
                return 1;
            case 0: // child process
                // return fileRedirectCall(batch, 0, batch.size - 1);
                return pipeCall(batch, 0, batch.size - 1);
            default: // parent process
                while (wait(&status) < 0);
                debugPrintf(mainDebug, "child pid: %d terminated\n", pid);
                break;
        }
        free(batch.commands);
    next:
        printf("%s", prompt);
        getString(inputBuffer, BUFFERSIZE);
    }
    return 0;
}

char *addEssentialSpaces(char *inputBuffer) {
    char *buffer = malloc(sizeof(char) * BUFFERSIZE);
    char *oldBuffer = inputBuffer;
    char *newBuffer = buffer;
    int count;
    while (*inputBuffer != 0) {
        switch (*inputBuffer) {
            case '|':
                *(buffer++) = ' ';
                *(buffer++) = '|';
                *(buffer++) = ' ';
                break;
            case '<':
                *(buffer++) = ' ';
                *(buffer++) = '<';
                *(buffer++) = ' ';
                break;
            case '>':
                count = 0;
                do {
                    if (count % 2 == 0) {
                        *(buffer++) = ' ';
                    }
                    *(buffer++) = '>';
                    ++inputBuffer;
                    ++count;
                } while (*inputBuffer != 0 && *inputBuffer == '>');
                *(buffer++) = ' ';
                *(buffer++) = *inputBuffer;
                if (*inputBuffer == 0)
                    return newBuffer;
                break;
            default:
                *(buffer++) = *inputBuffer;
        }
        ++inputBuffer;
    }
    free(oldBuffer);
    *buffer = 0;
    return newBuffer;
}

