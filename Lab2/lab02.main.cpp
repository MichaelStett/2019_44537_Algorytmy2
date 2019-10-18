// ALGO2 IS1 212A LAB02	
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

#define keyMax 99999
#define keyMin 99

int length = 0;

struct Node
{
	long int Key;
	double MyDouble;
	char MyChar;

	Node* next;
};

// Radnom
double GetRandom(int min = INT_MIN, int max = INT_MAX) {
	vector<int> range{ { min, max } };
	auto RandNumber = mt19937();
	RandNumber.seed(random_device()());
	uniform_real_distribution<double> dist_(range.front(), range.back());
	return dist_(RandNumber);
}

// Helper
Node* Exist(Node* head, int key) {
	if (length <= 0) {
		// printf("List is empty. \n");
		return nullptr;
	}
	else
	{
		Node* current = head;
		for (int i = 0; i < length; i++) {
			if (current->Key == key)
				return current;

			current = current->next;
		}
	}
	return nullptr;
}

void FindKey(Node* head, int key)
{
	if (length <= 0) {
		printf("List is empty. \n");
	}
	else
	{
		if (Exist(head, key) != nullptr) {
			printf("Key (%i) found. \n", key);
		}
		else {
			printf("Key (%i) not found. \n", key);
		}
	}
}

// Print length
void PrintSize() {
	printf("List size: %i. \n", length);
}

// Print First X element
void PrintFirst(Node* head, int Y)
{
	if (head == nullptr || length <= 0) {
		printf("List empty or not initialized. \n");
	}
	else
	{
		printf("First %i nodes: ", Y);
		Node* temp = head;
		for (int i = 0; i < Y; i++)
		{
			printf("%i  -> ", temp->Key);
			temp = temp->next;
		}
	}

	printf("\n \n");
}

// Print Last X elements
void PrintLast(Node* head, int Z)
{
	if (head == nullptr || length <= 0) {
		printf("List empty or not initialized. \n");
	}
	else
	{
		Node* temp = head;
		for (int i = 0; i < length - Z; i++)
		{
			temp = temp->next;
		}

		printf("Last %i nodes: ", Z);
		for (int i = 0; i < Z; i++)
		{
			printf("%i -> ", temp->Key);
			temp = temp->next;
		}
	}

	printf("\n \n");
}

// Delete Node with specific Key 
void DeleteNode(Node** head, int key) {
	auto node = Exist(*head, key);

	if (node != nullptr) {
		if (node == *head) {
			(*head)->Key = (*head)->next->Key;

			node = (*head)->next;

			(*head)->next = (*head)->next->next;

			printf("Deleting node of key %i. \n", node->Key);
			delete node;
			length--;

			return;
		}
		else {
			Node* prev = *head;
			while (prev->next != node) {
				prev = prev->next;
			}

			prev->next = prev->next->next; // Move list 

			printf("Deleting node of key %i. \n", node->Key);
			delete node;
			length--;

			return;
		}
	}
	else {
		printf("Node (%i) doesn't exist. Can't delete. \n", key);
	}
}

// Delete all Nodes
void DeleteAll(Node* head)
{
	Node* current = head;
	Node* next;

	while (current != nullptr)
	{
		next = current->next;
		// printf("Deleting node of key %i. \n", current->Key);
		delete current;
		length--;

		current = next;
	}

	head = nullptr;
}

// Insert One Node w/ Key of specified value
void InsertOne(Node** head, int key)
{
	auto node = Exist(*head, key);

	if (node == nullptr) { // if Node w/ such key doesnt exist

		node = new Node;
		node->Key = key;
		node->MyDouble = GetRandom();
		node->MyChar = 'T';

		if (*head == nullptr || (*head)->Key > node->Key)
		{
			node->next = *head;
			*head = node;
			// printf("Added node (%i) as head. \n", key);
			length++;
			return;
		}
		else {
			Node* current;
			current = *head;
			// if node ahead exist and key of it is smaller than new node procced to next one
			while (current->next != NULL && current->next->Key < node->Key)
			{
				current = current->next;
			}

			node->next = current->next;
			current->next = node;

			// printf("Added node (%i). \n", key);
			length++;
			return;
		}
	}
	else {
		printf("Node of given key (%i) already exist. \n", key);
	}
}

// Insert Multiple Nodes
void InsertMulti(Node** head, int X)
{
	for (int i = 0; i < X; i++) {
		int key = 0;

		do {
			key = static_cast<int>(GetRandom(keyMin, keyMax));
		} while (Exist(*head, key) != nullptr);

		// printf("Generated: %i \n", key);
		InsertOne(head, key);
	}
}

int main()
{
	clock_t start, end;
	start = clock();

	int X, k1, k2, k3, k4, k5;

	X = k1 = k2 = k3 = k4 = k5 = 0;

	fstream file{ "inlab02.txt" };
	if (file.is_open())
	{
		file >> X >> k1 >> k2 >> k3 >> k4 >> k5;

		Node* head = nullptr;

		FindKey(head, k1);
		InsertMulti(&head, X);
		PrintSize();

		PrintFirst(head, 20);
		InsertOne(&head, k2);
		PrintFirst(head, 20);
		InsertOne(&head, k3);
		PrintFirst(head, 20);
		InsertOne(&head, k4);
		PrintFirst(head, 20);
		InsertOne(&head, k5);

		DeleteNode(&head, k3);
		PrintFirst(head, 20);
		DeleteNode(&head, k2);
		PrintFirst(head, 20);
		DeleteNode(&head, k5);

		PrintSize();
		FindKey(head, k5);
		PrintLast(head, 11);
		PrintSize();
		DeleteAll(head);
	}
	else
		printf("No file. \n");

	end = clock();

	printf("Time: %0.3fs", (float)(end - start) / CLOCKS_PER_SEC);
}