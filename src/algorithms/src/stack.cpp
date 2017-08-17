#include <stdlib.h>
#include <stdio.h>


template <class T>
class Stack {
	static int const deafult_capacity = 5;

	T *arr;
	int capacity;
	int size;

	void resize() {
		capacity += (int) (2.0 / 3 * capacity); 
		arr = (T*) realloc(arr, capacity * sizeof(T));
	}

	public:
	Stack() {
		capacity = deafult_capacity;
		size = 0;
		arr = (T*) malloc(capacity * sizeof(T));
	}
	~Stack() {
		free(arr);
	}

	void push(T const & new_elem) {
		if (size == capacity) {
			resize();
		}

		arr[size++] = new_elem;
	}
	T pop() {
		return arr[size--];
	}
	int get_size() {
		return size;
	}
	void print() {
		printf("stack info:\n");
		printf("object address: %p\n", this);
		printf("container address: %p\n", arr);
		printf("capacity: %d\n", capacity);
		printf("size: %d\n", size);

		printf("data: ");
		if (is_empty()) {
			printf("<empty>\n");
		} else {
			for (int i = 0; i < size; ++i) {
				printf("%d ", arr[i]);
			}
			printf("\n");
		}
		printf("\n");
	}
};


int main() {
	Stack<int> s1;

	s1.print();

	for (int i = 0; i < 13; ++i) {
		s1.push(i);
	}

	s1.print();

	s1.pop();
	s1.pop();
	s1.pop();

	s1.print();

	return 0;
}
