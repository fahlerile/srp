#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "utils/fileUtils.h"

#define MAX_LINE_LENGTH_IN_TESTFILE 100
#define TESTFILE_NAME "res/fileUtilsTest_testfile.txt"

void testReadLine(FILE* fp)
{
    char lineToCompareTo[MAX_LINE_LENGTH_IN_TESTFILE];
    char* line = NULL;
    size_t n = 0;
    long pos = ftell(fp);
    while (readLine(&line, &n, fp) != EOF)
    {
        fseek(fp, pos, SEEK_SET);
        assert(fgets(lineToCompareTo, MAX_LINE_LENGTH_IN_TESTFILE, fp) != NULL);
        assert(strcmp(line, lineToCompareTo) == 0);
        pos = ftell(fp);
    }
}

int main()
{
    FILE* fp = fopen(TESTFILE_NAME, "r");
    assert(fp != NULL);
    testReadLine(fp);
    fclose(fp);
}
