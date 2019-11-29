// ALGO2 IS1 212A LAB06
// Michal Tymejczyk
// tm44537@zut.edu.pl

#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <string>

using namespace std;
#define ulong unsigned long
#define Max   pow(10, 7)

int GetRandom(int min = INT_MIN, int max = INT_MAX) {
	vector<int> range{ { min, max } };
	auto RandNumber = mt19937();
	RandNumber.seed(random_device()());
	uniform_real_distribution<double> dist_(range.front(), range.back());
	return static_cast<int>(dist_(RandNumber));
}

struct Tablica {
	int* values;
	ulong length;
	ulong max;
public:
	Tablica() {
		values = new int[1];
		length = 0;
		max = 1;
	}

	void Add(int value) {
		if (max < length + 1) {
			max *= 2; // new maximum
			int* temp = new int[max];

			move(values, temp, length);

			delete[] values;

			values = new int[max];

			move(temp, values, length);
		}
		values[length++] = value; // add value to next position
	}

	int& Index(int index) {
		return values[index];
	}

	void Free() {
		delete[] values;

		values = new int[1];
		length = 0;
		max = 1;
	}

	string ToString() {
		string ret = "";
		for (int i = 0; i < length; i++) {
			if (values[i] > 0) {
				ret += std::to_string(values[i]);
				ret += " ";
			}
		}

		return ret == "" ? "Empty." : ret;
	}

private:
	void move(int*& from, int*& to, int times) {
		for (int i = 0; i < times; i++)
			to[i] = from[i];
	}
};

struct Heap {
	Tablica tablica;

	void HeapUp(int size) {
		int parent = size == 0 ? -1 : (size - 1) / 2;
		// swap if parent < newElem
		if (size && tablica.Index(parent) < tablica.Index(size)) {
			// printf("swap %i %i \n", tablica.Index(size), tablica.Index(parent));
			swap(tablica.Index(size), tablica.Index(parent));
			HeapUp(parent);
		}
	}

	void HeapDown(int size) {
		int left = (2 * size + 1);
		int right = (2 * size + 2);

		int greater = size;
		int length = tablica.length;

		if (left < length && tablica.Index(left) > tablica.Index(size))
			greater = left;
		else
		if (right < length && tablica.Index(right) > tablica.Index(greater))
			greater = right;

		if (greater != size) {
			swap(tablica.Index(size), tablica.Index(greater));
			HeapDown(greater);
		}
	}

	void Add(int i) {
		// printf("Added %i \n", i);
		tablica.Add(i);
		HeapUp(tablica.length - 1);
	}

	void DeleteRoot() {
		this->GetRoot() = tablica.Index(tablica.length);
		HeapDown(0);
		tablica.Index(tablica.length) = 0;
		// printf("Deleted. \n");
	}

	int& GetRoot() {
		return tablica.Index(0);
	}

	string ToString() {
		string ret = "";
		ret = tablica.ToString();
		return ret;
	}

	void Free() {
		tablica.Free();
	}
};


int main() {
	clock_t start;
	clock_t end;

	Heap* heap = new Heap();

	for (ulong n = 100; n <= Max; n *= 10) // od 10^1 do 10^7
	{
		start = clock();
		for (int i = 0; i < n; i++)
			heap->Add(GetRandom(1, Max));
		end = clock();

		cout << "Length: " << heap->tablica.length << endl;
		cout << "Heap: " << heap->ToString() << endl;
		printf("Add time: %0.3fs \n\n", (float)(end - start) / CLOCKS_PER_SEC);

		start = clock();
		for (int i = 0; i <= n; i++) {
			heap->DeleteRoot();
		}
		end = clock();

		cout << "Heap: " << heap->ToString() << endl;
		printf("Delete time: %0.3fs \n\n", (float)(end - start) / CLOCKS_PER_SEC);

		heap->Free();
	}

	delete heap;
	return 0;
}

