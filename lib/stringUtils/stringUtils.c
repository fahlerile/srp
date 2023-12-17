#include <string.h>
#include "utils/utils.h"
#include "stringUtils.h"

static void splitStringFreeCallback(void* p_str)
{
    xfree(*(char**) p_str);
}

DynamicArray* splitString(const char* str, char* delim)
{
    DynamicArray* arr = newDynamicArray(1, sizeof(char*), splitStringFreeCallback);
    
    size_t nDelims = strlen(delim);
    size_t lengthCounter = 0;
    size_t startOfNextToken = 0;

    for (size_t i = 0, n = strlen(str); i < n; i++)
    {
        bool isDelim = false;
        for (size_t j = 0; j < nDelims; j++)
        {
            if (str[i] == delim[j])
            {
                isDelim = true;
                break;
            }
        }
        if (!isDelim || i + 1 == n)
            lengthCounter++;
        if (isDelim || i + 1 == n)
        {
            char* token = xmalloc((lengthCounter + 1) * sizeof(char));
            strncpy(token, str + startOfNextToken, lengthCounter);
            token[lengthCounter] = '\0';
            addToDynamicArray(arr, &token);

            lengthCounter = 0;
            startOfNextToken = i + 1;
        }
    }
    return arr;
}

