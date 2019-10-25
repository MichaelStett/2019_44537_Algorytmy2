// ALGO2 IS1 212A LAB03	
// Michał Tymejczyk
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
#define LMAX 6
#define PROB 0.5
#define keyMax 9999
#define keyMin 99

double GetRandom(int min = INT_MIN, int max = INT_MAX) {
	vector<int> range{ { min, max } };
	auto RandNumber = mt19937();
	RandNumber.seed(random_device()());
	uniform_real_distribution<double> dist_(range.front(), range.back());
	return dist_(RandNumber);
}

int randomlevel()
{
	int level = 0;
	while (GetRandom(0, 100) < PROB * 100 && level < LMAX)
		level++;
	return level;
};

struct Node
{
	int key;
	char Char;
	double Double;
	Node** next;

	Node(int key, int level)
	{
		this->key = key;
		this->Char = 'T';
		this->Double = GetRandom();

		next = new Node * [level + 1];

		for (int i = 0; i <= level; i++) next[i] = nullptr;
	};
};

class List
{
private:
	int level = 0;
	Node* head = new Node(INT_MIN, LMAX);
public:
	void InsertMulti(int X);
	void InsertOne(int);
	void Delete(int key);
	void Delete();
	bool Exist(int key); // helper for search
	void Find(int key);
	void PrintFromToCount(int from, int to, int count);
	void NodeCountOnLevelsFrom(int N);
};

void List::InsertMulti(int X) {
	for (var i = 0; i <= X; i++) {
		int key = 0;
		do {
			key = static_cast<int>(GetRandom(keyMin, keyMax));
		} while (Exist(key));

		InsertOne(key);
	}
}

void List::InsertOne(int new_key)
{
	var current = head;
	var update = new Node * [LMAX];

	for (int i = 0; i <= LMAX; i++) update[i] = nullptr;

	for (int i = level; i >= 0; i--)
	{
		while (current->next[i] != nullptr &&
			current->next[i]->key < new_key)
			current = current->next[i];
		update[i] = current;
	}

	current = current->next[0];

	if (current == nullptr || current->key != new_key)
	{
		int new_height = randomlevel();

		if (new_height > level)
		{
			for (int i = level + 1; i <= new_height; i++)
				update[i] = head;

			level = new_height;
		}

		var new_node = new Node(new_key, new_height);

		for (int i = 0; i <= new_height; i++)
		{
			new_node->next[i] = update[i]->next[i];
			update[i]->next[i] = new_node;
		}
		printf("Inserted key (%i). \n", new_key);
		return;
	}
	else
	{
		printf("\n");
		printf("Key (%i) already in list. \n", new_key);
		return;
	}
};

void List::Delete(int key)
{
	var current = head;

	var update = new Node * [LMAX + 1];

	for (int i = 0; i <= LMAX; i++) update[i] = nullptr;

	for (int i = level; i >= 0; i--)
	{
		while (current->next[i] != nullptr && current->next[i]->key < key)
			current = current->next[i];

		update[i] = current;
	}

	current = current->next[0];

	if (current != nullptr and current->key == key)
	{
		for (int i = 0; i <= level; i++)
		{
			if (update[i]->next[i] != current)
				break;

			update[i]->next[i] = current->next[i];
		}

		delete current;

		printf("Deleted key (%i). \n", key);
	}
	else
	{
		printf("Can't delete key: (%i), key doesn't exist.", key);
	}
};

void List::Delete() {
	
	Node* current = head;
	Node* next = nullptr;

	while (current != nullptr)
	{
		next = current->next[0];
		delete current;
		current = next;
	}

	head = nullptr;
}

bool List::Exist(int key) {
	var current = head;

	for (int i = level; i >= 0; i--)
	{
		while (current->next[i] && current->next[i]->key < key)
			current = current->next[i];
	}

	current = current->next[0];

	return current != nullptr && current->key == key;
}

void List::Find(int key)
{
	printf("\n");
	printf(Exist(key) ? "Found key: %i. \n" : "Can't find key: %i. \n", key);
};

void List::PrintFromToCount(int from, int to, int count)
{
	printf("\n");
	if (head != nullptr) {
		Node* node = head->next[0];
		if (to - from == 1) { // only one height
			printf("Level %i: ", from);
			for (var j = 0; node != nullptr && j != count; j++)
			{
				printf("%i ", node->key);
				node = node->next[0];
			}
		}
		else
		{
			for (var i = from; i <= to; i++)
			{
				node = head->next[i];
				printf("Level %i: ", i);
				for (var j = 0; node != nullptr && j != count; j++)
				{
					printf("%i ", node->key);
					node = node->next[i];
				}
				printf("\n");
			}
		}
	}
	else
	{
		printf("List is empty.");
	}
	printf("\n");
};

void List::NodeCountOnLevelsFrom(int N) {
	printf("\n");
	if (head!= nullptr) {
		for (var i = N; i <= LMAX; i++)
		{
			var node = head->next[i];
			var j = 0;
			for (; node != nullptr; j++)
			{
				node = node->next[i];
			}
			printf("Count on level %i: %i. \n", i, j);
		}
	} 
	else
	{
		printf("List is empty.");
	}
	printf("\n");
}

int main()
{
	var list = new List();

	clock_t start, end;
	start = clock();

	int X, k1, k2, k3, k4, k5;

	X = k1 = k2 = k3 = k4 = k5 = 0;

	fstream file{ "inlab03.txt" };
	if (file.is_open())
	{
		file >> X >> k1 >> k2 >> k3 >> k4 >> k5;

		var list = new List();

		list->Find(k1);

		list->InsertMulti(X);

		list->NodeCountOnLevelsFrom(0);
		list->PrintFromToCount(0, LMAX, 20);

		list->InsertOne(k2);
		list->PrintFromToCount(0, 1, 20);

		list->InsertOne(k3);
		list->PrintFromToCount(0, 1, 20);

		list->InsertOne(k4);
		list->PrintFromToCount(0, 1, 20);

		list->InsertOne(k4);
		list->PrintFromToCount(0, 1, 20);

		list->InsertOne(k5);
		list->PrintFromToCount(0, LMAX, 20);

		list->Delete(k3);
		list->Delete(k2);
		list->Delete(k5);

		list->NodeCountOnLevelsFrom(0);

		list->PrintFromToCount(0, LMAX, 20);	

		list->Delete();

		list->NodeCountOnLevelsFrom(0);
	}
	else
		printf("No file. \n");

	end = clock();

	printf("Time: %0.3fs", (float)(end - start) / CLOCKS_PER_SEC);
}