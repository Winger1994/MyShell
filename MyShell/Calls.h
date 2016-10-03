//
//  Calls.h
//  MyShell
//
//  Created by 赵超 on 03/10/2016.
//  Copyright © 2016 赵超. All rights reserved.
//

#ifndef Calls_h
#define Calls_h

#include "Utils.h"
#include "CommandBatch.h"
#include "unistd.h"
#include "fcntl.h"

int simpleCall(CommandBatch batch, int begin, int end);
int fileRedirectCall(CommandBatch batch, int begin, int end);
int pipeCall(CommandBatch batch, int begin, int end);


#endif /* Calls_h */
