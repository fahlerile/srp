// Software Rendering Pipeline (SRP) library
// Licensed under GNU GPLv3

/** @file
 *  Declarations of wrappers to `malloc`, `free` and `realloc` */

#pragma once

void* srpMalloc(unsigned long size);
void srpFree(void* p);
void* srpRealloc(void* p, unsigned long size);

