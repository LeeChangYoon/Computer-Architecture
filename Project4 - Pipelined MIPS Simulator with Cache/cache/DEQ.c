#include "cpu.h"

/* pop the index at the end of the deque */
void Deque_Cycle(int* Deque, int size) {
	int temp = Deque[size - 1];
	for (int i = size - 1; i > 0; i--) Deque[i] = Deque[i - 1];
	Deque[0] = temp;
}