//
//  main.c
//  MyShell
//
//  Created by 赵超 on 02/10/2016.
//  Copyright © 2016 赵超. All rights reserved.
//

#include "unistd.h"
#include "stdlib.h"
#include "CommandBatch.h"
#include "Utils.h"

const int BUFFERSIZE = 1024;
const char *prompt = "ve482sh $ ";
const int mainDebug = 0;

int simpleCall(CommandBatch batch) {
    int ret;
    char *parameters[batch.size + 1];
    memcpy(parameters, batch.commands, sizeof(char*) * batch.size);
    parameters[batch.size] = NULL;
    ret = execvp(batch.commands[0], parameters);
    debugPrintf(mainDebug, "[child] ret: %d\n", ret);
    return ret;
}

int main(int argc, const char * argv[]) {
    char *inputBuffer = (char*) malloc(sizeof(char) * BUFFERSIZE);
    printf("%s", prompt);
    getString(inputBuffer, BUFFERSIZE);
    int pid, block;
    while (!isStringEqual("exit", inputBuffer)) {
        CommandBatch batch = generateBatch(inputBuffer);
        pid = fork();
        switch (pid) {
            case -1:
                printf("FORK ERROR!\n");
                return 1;
            case 0: // child process
                return simpleCall(batch);
            default:
                while (wait(&block) < 0);
                debugPrintf(mainDebug, "child pid: %d terminated\n", pid);
                break;
        }
        printf("%s", prompt);
        getString(inputBuffer, BUFFERSIZE);
    }
    return 0;
}
