#import "numutils.h"

int num_digits(int start) {
	int count = 0;
	int n = start;
	while(n != 0) {
		n /= 10;
		++count;
	}
	return count + (start < 0 ? 1 : 0);
}