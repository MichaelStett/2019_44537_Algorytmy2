// ALGO2 IS1 212A LAB04
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
#define bottom -10000
#define top +10000

static long counter = 0;

struct Node {
	int key;
	char* keyString = new char[10];
	Node* left;
	Node* right;

	Node(int key) :
		key(key),
		left(nullptr),
		right(nullptr)
	{
		snprintf(keyString, 10, "%d", key); // niby działa
	};
};

#pragma region Helpers
double GetRandom(int min = INT_MIN, int max = INT_MAX) {
	vector<int> range{ { min, max } };
	auto RandNumber = mt19937();
	RandNumber.seed(random_device()());
	uniform_real_distribution<double> dist_(range.front(), range.back());
	return dist_(RandNumber);
}

Node* Exist(Node* root, int key) {
	while (root != nullptr)
	{
		if (key > root->key)
			root = root->right;
		else if (key < root->key)
			root = root->left;
		else
			return root;
	}
	return nullptr;
}
#pragma endregion

#pragma region Node_Functions
void Insert(Node*& root, int key) {
	if (Exist(root, key) != nullptr)
	{
		printf("Can't insert, key (%i) already exist. \n", key);
		return;
	}

	if (root == nullptr)
	{
		printf("Added new node (%i). \n", key);
		root = new Node(key);
		return;
	}

	Node* current = root;
	Node* prev = nullptr;

	while (current != nullptr) {
		prev = current;
		if (key < current->key)
			current = current->left;
		else
			current = current->right;
	}

	if (key < prev->key)
		prev->left = new Node(key);
	else
		prev->right = new Node(key);

	printf("Added new node (%i). \n", key);
}

void InsertMany(Node*& root, int X) {

	for (var i = 0; i < X; i++) {
		int key = 0;
		do {
			key = GetRandom(bottom, top);
		} while (Exist(root, key));

		Insert(root, key);
	}

	// printf("Added %i nodes. \n", X);
}

void Find(Node* root, int key) {
	if (root == nullptr)
	{
		printf("Root is nullptr! \n");
		return;
	}

	printf(Exist(root, key) != nullptr ? "Found key: %i. \n" : "Can't find key: %i. \n", key);
	return;
}

void Delete(Node*& root, int key) {
	Node* parent = nullptr,
		* child = root,
		* current = root;

	if (root == nullptr)
	{
		printf("Root is nullptr! \n");
		return;
	}

	while (current != nullptr && current->key != key) {
		parent = current;
		// no <= or >=, unique keys
		if (key < current->key)
			current = current->left;
		else if (key > current->key)
			current = current->right;
	}

	if (current == nullptr)
	{
		printf("Can't delete, key (%i) doesn't exist. \n", key);
		return;
	}

	printf("Deleted key (%i). \n", key);

	// no children
	if (current->left == nullptr && current->right == nullptr)
	{
		if (parent == nullptr)
			root = nullptr;
		else if (key < parent->key)
			parent->left = nullptr;
		else if (key > parent->key)
			parent->right = nullptr;

		return;
	}

	// left child is nullptr
	if (current->left == nullptr)
	{
		child = current->right;
		swap(current->key, child->key);
		swap(current->keyString, child->keyString);
		current->left = child->left;
		current->right = child->right;
		return;
	}

	// right child is nullptr
	if (current->right == nullptr)
	{
		child = current->left;
		swap(current->key, child->key);
		swap(current->keyString, child->keyString);
		current->left = child->left;
		current->right = child->right;
		return;
	}

	// two children
	if (current->left != nullptr && current->right != nullptr)
	{
		child = current->left;
		parent = nullptr;

		// poprzednik 
		while (child->right != nullptr) {
			parent = child;
			child = parent->right;
		}

		if (parent == nullptr) {
			swap(current->key, child->key);
			swap(current->keyString, child->keyString);
			current->left = child->left;
			return;
		}

		if (parent != nullptr) {
			swap(current->key, child->key);
			swap(current->keyString, child->keyString);
			parent->right = child->left;
			return;
		}
	}

}
#pragma endregion

#pragma region Printing
void Postorder(Node* root)
{
	if (root != NULL)
	{
		Postorder(root->left);
		Postorder(root->right);
		printf("%i ", root->key);
	}
	return;
}

void Preorder(Node* root)
{
	if (root != NULL)
	{
		printf("%i ", root->key);
		Preorder(root->left);
		Preorder(root->right);
	}
	return;
}

void Inorder(Node* root)
{
	if (root != NULL)
	{
		Inorder(root->left);
		printf("%i ", root->key);
		Inorder(root->right);
		counter++;
	}
	return;
}
#pragma endregion

int main() {
	var start = clock();

	Node* root = nullptr;
	int X, k1, k2, k3, k4;

	fstream file{ "inlab04.txt" };
	if (file.is_open())
	{
		file >> X >> k1 >> k2 >> k3 >> k4;

		Delete(root, k1);
		Insert(root, k1);

			InsertMany(root, X);

		printf("Inorder:");
		Inorder(root);
		printf("\nElements count: %i", counter);
		printf("\n \n");

		printf("Preorder: ");
		Preorder(root);
		printf("\n \n");

		Insert(root, k2);
		Insert(root, k3);
		Insert(root, k4);

		Delete(root, k1);

		printf("Preorder: ");
		Preorder(root);
		printf("\n \n");

		Find(root, k1);

		Delete(root, k2);

		printf("Inorder:");
		Inorder(root);
		printf("\nElements count: %i", counter);
		printf("\n \n");

		Delete(root, k3);
		Delete(root, k4);

		printf("\n");
	}
	else
		printf("No file. \n");

	var end = clock();

	printf("Time: %0.3fs", (float)(end - start) / CLOCKS_PER_SEC);
}
