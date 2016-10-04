//
//  main.c
//  MyShell
//
//  Created by 赵超 on 02/10/2016.
//  Copyright © 2016 赵超. All rights reserved.
//

#include "Calls.h"
#include "signal.h"

const int BUFFERSIZE = 1024;
const char *prompt = "ve482sh $ ";
const int mainDebug = 0;

int pid;

void changeDir(const char *path) {
    int ret = chdir(path);
    if (ret != 0)
        errorPrompt();
}

void signalHandler(int signo) {
    debugPrintf(mainDebug, "Signal #%d Caught!\n", signo);
    if (pid > 0) {
        switch (signo) {
            case SIGINT:
                debugPrintf(mainDebug, "Redirect INT Signal to child: %d", pid);
                kill(pid, SIGINT);
                break;
            default:
                debugPrintf(mainDebug, "Signal Not Redict\n");
                break;
        }
    } else {
        debugPrintf(mainDebug, "pid: %d not Valid!\n", pid);
        printf("\n%s", prompt);
    }
}

char *addEssentialSpaces(char *inputBuffer);

int main(int argc, const char * argv[]) {
    signal(SIGINT, signalHandler);
    char *inputBuffer = (char*) malloc(sizeof(char) * BUFFERSIZE);
    printf("%s", prompt);
    getString(inputBuffer, BUFFERSIZE);
    int status;
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
                signal(SIGINT, SIG_DFL);
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

