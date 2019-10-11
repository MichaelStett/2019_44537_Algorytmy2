// ALGO2 IS1 212A LAB01
// tm44537@zut.edu.pl

#include <fstream>
#include <string>
#include <ctime>

#define charB 66
#define charS 83
#define upperLimit 9000
#define lowerLimit -1000;

using std::fstream;
using std::string;
using std::getline;
using std::swap;
using std::stoi;

struct MyStruct {
	int MyInt;
	char MyChar;
	float MyFloat;

	static MyStruct** Rand(MyStruct** tab, int N) {
		srand((unsigned int)time(nullptr));
		for (int i = 0; i < N - 1; i++) {
			int temp = 0;

			do {
				temp = (rand() % upperLimit) + lowerLimit;
			} while (Exist(tab, N, temp));

			tab[i]->MyInt = temp;
			tab[i]->MyChar = rand() % (charS - charB) + charB;
			tab[i]->MyFloat = 1000.0f + (float)i + 1;
		}
		return tab;
	}

	static void Sort(MyStruct** tab, int N) {
		bool flag;
		for (int i = 0; i < N - 1; i++) {
			flag = false;
			for (int j = 0; j < N - i - 1; j++)
				if (tab[j]->MyInt > tab[j + 1]->MyInt) {
					Swap(tab[j], tab[j + 1]);
					flag = true;
				}
			if (!flag)
				return;
		}
	}

	static int FindChar(MyStruct** tab, int N, char X) {
		int count = 0;
		for (int i = 0; i < N - 1; i++)
			if (tab[i]->MyChar == X)
				count++;

		return count;
	}

	static void Print(MyStruct** tab, int num) {
		for (int i = 0; i < num; i++)
			printf("%i %c %.0f \n", tab[i]->MyInt, tab[i]->MyChar, tab[i]->MyFloat);
	}

	static void Delete(MyStruct** tab, int N) {
		for (int i = 0; i < N; i++)
			delete tab[i];

		delete tab;
	}

private:
	static void Swap(MyStruct* tab, MyStruct* tab2) {
		swap(tab->MyInt, tab2->MyInt);
		swap(tab->MyChar, tab2->MyChar);
		swap(tab->MyFloat, tab2->MyFloat);
	}

	static bool Exist(MyStruct** tab, int N, int check) {
		for (int i = 0; i < N; i++)
			if (tab[i]->MyInt == check)
				return true;

		return false;
	}
};

int main() {
	clock_t start, end;
	start = clock();

	int N;
	char X;
	string line;

	fstream file { "inlab01.txt" };

	if (file.is_open())
	{
		getline(file, line);

		N = stoi(line);
		X = line[N < 10 ? 2 : N < 100 ? 3 : N < 1000 ? 4 : N < 10000 ? 5 : -1];

		auto tab = new MyStruct * [N];

		for (int i = 0; i < N; i++)
			tab[i] = new MyStruct();

		MyStruct::Rand(tab, N);

		MyStruct::Sort(tab, N);

		MyStruct::Print(tab, 20);

		printf(" %c -> %i \n", X, MyStruct::FindChar(tab, N, X));

		MyStruct::Delete(tab, N);

		file.close();
	}
	else
		printf("No file. \n");

	end = clock();

	printf("Time: %0.3fs", (float)(end - start) / CLOCKS_PER_SEC);
}