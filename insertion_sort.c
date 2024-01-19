#include <stdio.h>
#include <stddef.h>

void insertion_sort(double array[], size_t capacity);

void insertion_sort(double array[], size_t capacity)
{
	double last = 0;
	size_t itr = capacity - 2;

	// Base case
	if (capacity <= 1) { return; }

	insertion_sort(array, capacity - 1);

	last = array[capacity - 1];

	for (
		;
		itr >= 0 && array[itr] > last;
		--itr
	) {
		array[itr + 1] = array[itr];
	}

	array[itr + 1] = last;
} // END void insertion_sort()

int main() {
	double test[10];
	test[0] = 12.2;
	test[1] = 21.7;
	test[2] = 5;
	test[3] = -12.5;
	test[4] = 1.12;
	test[5] = 10.1;
	test[6] = 3.6;
	test[7] = -11.2;
	test[8] = -2.5;
	test[9] = 0.45;

	for (size_t itr = 0; itr < 10; ++itr) {
		printf("%f\n", test[itr]);
	}

	puts("Sorted:\n");

	insertion_sort(test, 10);

	for (size_t itr = 0; itr < 10; ++itr) {
		printf("%f\n", test[itr]);
	}

	return 0;
}
