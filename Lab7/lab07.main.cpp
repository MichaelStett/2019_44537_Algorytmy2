// ALGO2 IS1 212A LAB07
// Michal Tymejczyk
// tm44537@zut.edu.pl

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <vector>
#include <random>

using namespace std;

#define var auto
#define bottom 20000
#define top	   40000
#define ulong unsigned long
#define mod %

double GetRandom(int min = INT_MIN, int max = INT_MAX) {
	vector<int> range{ { min, max } };
	auto RandNumber = mt19937();
	RandNumber.seed(random_device()());
	uniform_real_distribution<double> dist_(range.front(), range.back());
	return dist_(RandNumber);
}
// Liniowa
struct Tablica {
	int* values;
	ulong length;
	ulong max;

	Tablica() {
		max = 1000000;
		values = new int[max];
		length = 0;

		for (int i = 0; i < max; i++)
			values[i] = 0; // is empty
	}

	bool Find(int key) {
		int index = Hash(key); // find position
		while (values[index] != 0 && values[index] != key) {
			index += 1;
		}

		if (values[index] == 0) {
			return false;
		}

		return true;
	}

	void Print(int from, int to) {
		for (int i = from; i < to; i++)
			printf("%i: %i \n", i, values[i]);

		printf("\n\n");
	}

	void AddMany(int X) {
		for (int i = 0; i < X; i++)
		{
			int key = 0;
			do {
				key = static_cast<int>(GetRandom(bottom, top));
			} while (Find(key));

			Add(key);
			// printf("%i %i \n", i, key);
		}
	}

	void Add(int key) { // key already hashed
		if (max < length + 1) {
			max *= 2; // new maximum
			int* temp = new int[max];

			move(values, temp, length);

			delete[] values;

			values = new int[max];

			move(temp, values, length);
		}

		// Adresowanie liniowe
		int index = Hash(key);
		while (values[index] != 0 && values[index] != key) {
			index += 1;
		}

		values[index] = key;
		length++;
		// printf("Add: %i \n", key);
	}

	int Hash(int key) {
		return (((key mod 1000) + (int)(pow(2, (key mod 10))) + 1) mod 997);
	}

	int HashPrim(int key) {
		return ((3 * key) mod 19) + 1;
	}

	void Delete(int key) {
		if (!Find(key)) {
			printf("No key: %i \n", key);
			return;
		}

		int index = Hash(key); 
		while (values[index] != 0 && values[index] != key) {
			index += 1;
		}

		values[index] = -1;
		length--;
	}
	void Free() {
		delete[] values;

		max = 1000000;
		values = new int[max];
		length = 0;
	}

private:
	void move(int*& from, int*& to, int times) {
		for (int i = 0; i < times; i++)
			to[i] = from[i];
	}
};

// Podwojna
struct Tablica2 {
	int* values;
	ulong length;
	ulong max;

	Tablica2() {
		max = 1000000;
		values = new int[max];
		length = 0;

		for (int i = 0; i < max; i++)
			values[i] = 0; // is empty
	}

	bool Find(int key) {
		int index = Hash(key);
		while (values[index] != 0 && values[index] != key) {
			index += HashPrim(key);
		}

		if (values[index] == 0) {
			return false;
		}

		return true;
	}

	void Print(int from, int to) {
		for (int i = from; i < to; i++)
			printf("%i: %i \n", i, values[i]);

		printf("\n\n");
	}

	void AddMany(int X) {
		for (int i = 0; i < X; i++)
		{
			int key = 0;
			do {
				key = static_cast<int>(GetRandom(bottom, top));
			} while (Find(key));

			Add(key);
			// printf("%i %i \n", i + 1, key);
		}
	}

	void Add(int key) { // key already hashed
		if (max < length + 1) {
			max *= 2; // new maximum
			int* temp = new int[max];

			move(values, temp, length);

			delete[] values;

			values = new int[max];

			move(temp, values, length);
		}

		// Adresowanie podwojne
		int index = Hash(key);
		while (values[index] != 0 && values[index] != key) {
			index += HashPrim(key);
		}

		values[index] = key;
		length++;
	}

	int Hash(int key) {
		return (((key mod 1000) + (int)(pow(2, (key mod 10))) + 1) mod 997);
	}

	int HashPrim(int key) {
		return ((3 * key) mod 19) + 1;
	}

	void Delete(int key) {
		if (!Find(key)) {
			printf("No key. %i \n", key);
			return;
		}

		int index = Hash(key);
		while (values[index] != 0 && values[index] != key) {
			index += HashPrim(key);
		}

		
		values[index] = -1;
		length--;
	}
	void Free() {
		delete[] values;

		max = 1000000;
		values = new int[max];
		length = 0;
	}

private:
	void move(int*& from, int*& to, int times) {
		for (int i = 0; i < times; i++)
			to[i] = from[i];
	}
};

int main() {

	int X, k1, k2, k3, k4;
	X = k1 = k2 = k3 = k4 = 0;

	fstream file{ "inlab07.txt" };
	if (file.is_open())
	{
		file >> X >> k1 >> k2 >> k3 >> k4;

		var start = clock();
		var tab = new Tablica();

		tab->Delete(k1);
		tab->Add(k1);
		tab->Print(0, 100);
		tab->AddMany(X);
		tab->Print(0, 100);
		tab->Add(k2);
		tab->Add(k3);
		tab->Add(k4);
		tab->Print(0, 100);
		tab->Print(500, 600);
		tab->Delete(k3);
		tab->Delete(k4);
		tab->Print(0, 100);
		tab->Print(500, 600);
		tab->Free();

		var end = clock();

		printf("Time: %0.3fs \n", (float)(end - start) / CLOCKS_PER_SEC);

		start = clock();
		var tab2 = new Tablica2();

		tab2->Delete(k1);
		tab2->Add(k1);
		tab2->Print(0, 100);
		tab2->AddMany(X);
		tab2->Print(0, 100);
		tab2->Add(k2);
		tab2->Add(k3);
		tab2->Add(k4);
		tab2->Print(0, 100);
		tab2->Print(500, 600);
		tab2->Delete(k3);
		tab2->Delete(k4);
		tab2->Print(0, 100);
		tab2->Print(500, 600);
		tab2->Free();

		end = clock();

		printf("Time: %0.3fs \n", (float)(end - start) / CLOCKS_PER_SEC);
	}
	else
		printf("No file. \n");
}