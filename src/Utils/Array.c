#include "Array.h"
#include "Utils/Utils.h"

double sumOfArrayDouble(double* ptr, size_t n)
{
    double sum = 0;
    for (size_t i = 0; i < n; i++)
        sum += ptr[i];
    return sum;
}
