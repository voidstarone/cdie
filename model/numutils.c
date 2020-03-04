#include "numutils.h"

int num_digits(int start) {
	int count = 0;
	int n = start;
	while(n != 0) {
		n /= 10;
		++count;
	}
	return count + (start < 0 ? 1 : 0);
}

int max(int num1, int num2) {
	return num1 > num2 ? num1 : num2;
}

int min(int num1, int num2) {
	return num1 < num2 ? num1 : num2;
}