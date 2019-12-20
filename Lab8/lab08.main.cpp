// ALGO2 IS1 212A LAB08
// Michal Tymejczyk
// tm44537@zut.edu.pl

#include<stdio.h>
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <string>
#include <algorithm>

using namespace std;
#define var auto
#define Max   pow(10, 7)

double GetRandom(int min = INT_MIN, int max = INT_MAX) {
	vector<int> range{ { min, max } };
	auto RandNumber = mt19937();
	RandNumber.seed(random_device()());
	uniform_real_distribution<double> dist_(range.front(), range.back());
	return dist_(RandNumber);
}

template<class type>
type* InsertionSort(type* arr, int length) {
	type key = 0;
	size_t i, j;
	for (i = 0; i < length; i++)
	{
		key = arr[i];
		j = i - 1;

		while (j >= 0 && arr[j] > key)
		{
			arr[j + 1] = arr[j];
			j = j - 1;
		}
		arr[j + 1] = key;
	}

	return arr;
}

template<class type>
string ToString(type* arr, int length) {
	string s = "";
	for (size_t i = 0; i < length; i++)
		s += to_string(arr[i]) + " ";

	return s;
}

template<class type>
struct Tablica {
	type* values;
	unsigned long length;
	unsigned long max;
public:
	Tablica() :
		Tablica(1) {};

	Tablica(int size) {
		try {
			values = new type[size];
			length = 0;
			max = size;
		}
		catch (exception e) {
			printf("Error allocating memory!!!");
			exit(1);
		}

		fill(values, values + max, 0);
	}

	void Add(type value) {
		if (max < length + 1) {
			max *= 2;
			var temp = new type[max];

			Swap(values, temp, length);

			delete[] values;

			values = new type[max];

			Swap(temp, values, length);
		}
		values[length++] = value;
	}

	type& Index(int index) {
		return values[index];
	}

	void Free() {
		delete[] values;

		values = new type[1];
		length = 0;
		max = 1;
	}

	string ToString() {
		if (length == 0)
			return "Empty.";

		string ret = "";
		for (size_t i = 0; i < length; i++)
			if (values[i] > -1)
				ret = ret + std::to_string(values[i]) + " ";


		return ret;
	}

	type Maximum() {
		type max = values[0];
		for (size_t i = 1; i < length; i++)
			if (values[i] > max)
				max = values[i];

		return max;
	}

	type Minimum() {
		type min = values[0];
		for (int i = 1; i < length; i++)
			if (values[i] < min)
				min = values[i];

		return min;
	}
private:
	void Swap(type*& from, type*& to, int times) {
		for (int i = 0; i < times; i++)
			to[i] = from[i];
	}
};

template<class type>
struct Heap {
	Tablica<type> tablica;

	void HeapUp(int size) {
		int parent = size == 0 ? -1 : (size - 1) / 2;
		// swap if parent < newElem
		if (size && tablica.Index(parent) < tablica.Index(size)) {
			// printf("swap %i %i \n", tablica.Index(size), tablica.Index(parent));
			swap(tablica.Index(size), tablica.Index(parent));
			HeapUp(parent);
		}
	}

	void HeapDown(int length, int size) {
		int left = (2 * size + 1);
		int right = (2 * size + 2);

		int greater = size;

		if (left < length && tablica.Index(left) > tablica.Index(size))
			greater = left;

		if (right < length && tablica.Index(right) > tablica.Index(greater))
			greater = right;

		if (greater != size) {
			swap(tablica.Index(size), tablica.Index(greater));
			HeapDown(length, greater);
		}
	}

	void Add(type value) {
		tablica.Add(value);
		HeapUp(tablica.length - 1);
	}

	type& Index(int x) {
		return tablica.Index(x);
	}

	void DeleteRoot() {
		this->GetRoot() = tablica.Index(tablica.length);
		HeapDown(this->Size(), 0);
		tablica.Index(tablica.length) = 0;
	}

	type& GetRoot() {
		return tablica.Index(0);
	}

	int Size() {
		return tablica.length;
	}

	string ToString() {
		return tablica.ToString();
	}

	void Free() {
		tablica.Free();
	}

	type Maximum() {
		return tablica.Maximum();
	}

	type Minimum() {
		return tablica.Minimum();
	}

	void SwapAndHeapDown(int zero, int index) {
		swap(this->Index(zero), this->Index(index));
		HeapDown(index, zero);
	}
};

// Heapsort
template<class type>
void HSort(Heap<type>*& heap) {
	for (int i = heap->Size() - 1; i >= 0; i--)
		heap->SwapAndHeapDown(0, i);
}

// Counting sort - only int!!!
void CSort(Heap<int>*& heap) {
	int max = heap->Maximum();
	int size = heap->Size();
	var outputArray = new Tablica<int>(size + 1);
	var numbers = new Tablica<int>(max + 1);

	for (int i = 0; i < size; i++)
		++numbers->Index(heap->Index(i));

	for (int i = 1; i <= max; ++i)
		numbers->Index(i) += numbers->Index(i - 1);

	for (int i = size - 1; i >= 0; i--)
		outputArray->Index(--(numbers->Index(heap->Index(i)))) = heap->Index(i);

	for (int i = 0; i < size; ++i)
		heap->Index(i) = outputArray->Index(i);
}

// Bucket sort
template<class type>
void BSort(type* arr, int length) {
	type min = arr[0];
	type max = arr[0];

	for (size_t i = 0; i < length; i++)
		if (arr[i] < min) min = arr[i];
		else if (arr[i] > max) max = arr[i];

	type width = (max - min) / length;

	if (width == 0) 
		width = 10; // bo może wyjść dzielenie przez zero

	type** buckets = nullptr;

	buckets = new type * [length + 1];

	for (size_t i = 0; i <= length; i++)
		buckets[i] = new type[length + 1];

	for (size_t i = 0; i <= length; i++)
		for (size_t j = 0; j <= length; j++)
			buckets[i][j] = 0;

	for (size_t i = 0; i < length; i++)
		for (size_t j = 0; j < length; j++)
			if (buckets[(int)((arr[i] - min) / width)][j] == 0) {
				buckets[(int)((arr[i] - min) / width)][j] = arr[i];
				break;
			}

	for (size_t i = 0; i < length; i++) {
		buckets[i] = InsertionSort(buckets[i], length);
		cout << "Bucket:" << ToString(buckets[i], length) << endl;
	}

	for (size_t k = 0, i = 0; i <= length; i++)
		for (size_t j = 0; j <= length; j++)
			if (buckets[i][j] != 0)
				arr[k++] = buckets[i][j];

	for (size_t k = 0, i = 0; i <= length; i++)
		delete[] buckets[i];

	delete[] buckets;
}

int main() {
	clock_t start;
	clock_t end;

	Heap<int>* heap = nullptr;
	Heap<double>* dheap = nullptr;

	for (unsigned long size = 10; size <= Max; size = size * 10)
	{

		cout << "\n ***SIZE " << size << "***\n" << endl;
		cout << "\n ****SORTING INTS**** \n" << endl;

		cout << "HEAP SORT" << endl;
		heap = new Heap<int>();

		for (int i = 0; i < size; i++)
			heap->Add((int)(GetRandom(0, size)));

		start = clock();
		//cout << "Before: " << heap->ToString() << endl;
		HSort(heap);
		//cout << "After: " << heap->ToString() << endl;
		end = clock();
		printf("Time: %0.3fs \n", (float)(end - start) / CLOCKS_PER_SEC);

		heap->Free();
		delete heap;

		cout << "COUNTING SORT" << endl;
		heap = new Heap<int>();
		for (int i = 0; i < size; i++)
			heap->Add((int)(GetRandom(0, size)));

		start = clock();
		//cout << "Before: " << heap->ToString() << endl;
		CSort(heap);
		//cout << "After: " << heap->ToString() << endl;
		end = clock();
		printf("Time: %0.3fs \n", (float)(end - start) / CLOCKS_PER_SEC);

		heap->Free();
		delete heap;

		cout << "BUCKET SORT" << endl;
		int* arr = new int[size];
		for (int i = 0; i < size; i++)
			arr[i] = (int)GetRandom(0, size);

		start = clock();
		cout << "Before: " << ToString(arr, size) << endl;
		BSort(arr, size);
		cout << "After: " << ToString(arr, size) << endl;
		end = clock();
		printf("Time: %0.3fs \n", (float)(end - start) / CLOCKS_PER_SEC);

		delete[] arr;

		////////////////////////////////////

		cout << "\n ****SORTING DOUBLES**** \n" << endl;

		cout << "HEAP SORT" << endl;
		dheap = new Heap<double>();

		for (int i = 0; i < size; i++)
			dheap->Add(GetRandom(0, 1));

		start = clock();
		//cout << "Before: " << dheap->ToString() << endl;
		HSort(dheap);
		//cout << "After: " << dheap->ToString() << endl;
		end = clock();
		printf("Time: %0.3fs \n", (float)(end - start) / CLOCKS_PER_SEC);

		dheap->Free();
		delete dheap;

		cout << "BUCKET SORT" << endl;
		double* darr = new double[size];

		for (int i = 0; i < size; i++)
			darr[i] = (double)GetRandom(0, 1);

		start = clock();
		cout << "Before: " << ToString(darr, size) << endl;
		BSort(darr, size);
		cout << "After: " << ToString(darr, size) << endl;
		end = clock();
		printf("Time: %0.3fs \n", (float)(end - start) / CLOCKS_PER_SEC);

		delete[] darr;
	}
	return 0;
}
