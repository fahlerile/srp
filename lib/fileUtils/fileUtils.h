#pragma once
#include <stdio.h>

int readLine(char** line, size_t* length, FILE* fp);
size_t distanceToNextOccurenceOfCharInFile(char ch, FILE* fp);

