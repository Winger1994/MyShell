//
//  Utils.h
//  MyShell
//
//  Created by 赵超 on 03/10/2016.
//  Copyright © 2016 赵超. All rights reserved.
//

#ifndef Utils_h
#define Utils_h
#include "stdio.h"
#include "stdarg.h"

typedef int bool;

int getString(char *buffer, int size);
bool isStringEqual(char *a, char *b);
void debugPrintf(int level, const char *fmt, ...);

#endif /* Utils_h */
