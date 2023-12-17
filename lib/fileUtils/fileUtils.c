#include <stdio.h>
#include "memoryUtils/memoryUtils.h"
#include "fileUtils.h"

/**
 * @brief Read the whole line from a filestream INCLUDING '\n'
 * Also see `getline` from GNU standard
 * @param line a pointer to string (`char` array) which was `malloc`'ed for `length` characters. If points to `NULL`, the buffer will be allocated inside the function (don't forget to `free` it!) and the value pointed to by `length` is ignored.
 * @param length a pointer to number of characters the `line` was allocated to hold. The value pointed to is ignored if `line` is `NULL`.
 * @param fp file descriptor
 * @return int `0` if success, `EOF` if error
 */
int readLine(char** line, size_t* length, FILE* fp)
{
    size_t neededLength = distanceToNextOccurenceOfCharInFile('\n', fp);
    neededLength++;  // null byte exists

    // reallocate the line buffer if it is not long enough
    if (*line == NULL)
        goto allocate;
    else if (*length < neededLength)
    {
        xfree(*line);
allocate:
        *line = xmalloc(neededLength * sizeof(char));
        *length = neededLength;
    }

    return (fgets(*line, *length, fp) == NULL) ? EOF : 0;
}

/**
 * @brief Find the distance from the current position in file to the next occurence of a specific character
 * May be interpreted as "length of a string buffer to be able to read from current character up to including this character WITHOUT null byte (add +1 if you need null byte!)"
 * @param ch char to look for
 * @param fp file pointer
 */
size_t distanceToNextOccurenceOfCharInFile(char ch, FILE* fp)
{
    size_t distance = 0;
    long startPos = ftell(fp);
    char c;
    while ((c = fgetc(fp)) != EOF)
    {
        distance++;
        if (c == ch)
            break;
    }

    fseek(fp, startPos, SEEK_SET);
    return distance;
}

