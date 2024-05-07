#include "stdlib.h"
#include "float.h"
#include "numutils.h"
#include "math.h"

size_t num_digits(size_t start) {
	size_t count = 0;
	size_t n = start;
	while(n != 0) {
		n /= 10;
		++count;
	}
	return count + (start < 0 ? 1 : 0);
}

int count_decimals(const double val) {
    int result = 0;
    double epsilon = DBL_EPSILON;
    double exponent = 1.0;

    while(fabs(val * exponent - trunc(val * exponent)) > epsilon)
    {
        ++result;

        epsilon *= 10;
        exponent *= 10;
    }

    return result;
}

int max(int num1, int num2) {
	return num1 > num2 ? num1 : num2;
}

int min(int num1, int num2) {
	return num1 < num2 ? num1 : num2;
}