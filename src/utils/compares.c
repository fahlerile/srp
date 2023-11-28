#include <math.h>
#include "compares.h"

bool roughlyEqualD(double a, double b)
{
    return fabs(a - b) < TOLERANCE;
}
