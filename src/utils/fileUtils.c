#include <stdio.h>
#include "memoryUtils.h"
#include "fileUtils.h"

/**
 * @brief A function to read the whole line from a filestream. Also see `getline`
 *
 * @param line a pointer to string (`char` array) which was `malloc`'ed for `length` characters. If points to `NULL`, the buffer will be allocated inside the function (don't forget to `free` it!) and the value pointed to by `length` is ignored.
 * @param length a pointer to number of characters the `line` was allocated to hold. The value pointed to is ignored if `line` is `NULL`.
 * @param fp file descriptor
 * @return int `0` if success, `EOF` if error
 */
int readLine(char** line, size_t* length, FILE* fp)
{
    // find the closest \n character
    // determine the needed length for a line
    size_t neededLength = 0;
    long startPos = ftell(fp);
    char c;
    while ((c = fgetc(fp)) != EOF)
    {
        neededLength++;
        if (c == '\n')
            break;
    }
    neededLength++;  // one more (for null byte)
    fseek(fp, startPos, SEEK_SET);

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
