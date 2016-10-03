//
//  CommandBatch.h
//  MyShell
//
//  Created by 赵超 on 02/10/2016.
//  Copyright © 2016 赵超. All rights reserved.
//

#ifndef CommandBatch_h
#define CommandBatch_h
#include "Utils.h"

typedef struct CommandBatch {
    int size;
    char **commands;
} CommandBatch;

CommandBatch generateBatch(char *command);
void printCommandBatch(CommandBatch batch);
void printBatchInterval(CommandBatch batch, int begin, int end);

#endif /* CommandBatch_h */
