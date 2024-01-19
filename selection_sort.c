#include <stddef.h>
#include <stdio.h>

// Sorts least to greatest
void selection_sort(double array[], size_t capacity);

void selection_sort(double array[], size_t capacity)
{
	size_t index_largest = capacity - 1;
	double swap = 0;

	// ASSERT: capacity is real capacity

	// Base case
	if (capacity <= 1) { return; }

	for (size_t itr = 0; itr < capacity; ++itr) {
		if (array[itr] > array[index_largest]) {
			index_largest = itr;
		}
	}

	if (index_largest != (capacity - 1)) {
		swap = array[capacity - 1];
		array[capacity - 1] = array[index_largest];
		array[index_largest] = swap;
	}

	return selection_sort(array, capacity - 1);
} // END void selection_sort()

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

	selection_sort(test, 10);

	for (size_t itr = 0; itr < 10; ++itr) {
		printf("%f\n", test[itr]);
	}

	return 0;
}
