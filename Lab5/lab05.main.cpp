// ALGO2 IS1 212A LAB05
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
#define top	   +10000
int numberOfNodes = 0;

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

	~Node() {
		// printf("Destroyed node (%i) \n", key);
	}
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

#pragma region Printing
void Postorder(Node* root)
{
	if (root != nullptr)
	{
		Postorder(root->left);
		Postorder(root->right);
		printf("%i ", root->key);
	}
	return;
}

void Preorder(Node* root)
{
	if (root != nullptr)
	{
		printf("%i ", root->key);
		Preorder(root->left);
		Preorder(root->right);
	}
	return;
}

void Inorder(Node* root)
{
	if (root != nullptr)
	{
		Inorder(root->left);
		printf("%i ", root->key);
		Inorder(root->right);
	}
	return;
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
		// printf("Added new node (%i). \n", key);
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

	// printf("Added new node (%i). \n", key);
}

void InsertMany(Node*& root, int X) {
	for (var i = 0; i < X; i++) {
		int key = 0;
		do {
			key = static_cast<int>(GetRandom(bottom, top));
		} while (Exist(root, key));

		Insert(root, key);
		numberOfNodes++;
	}
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

	// printf("Deleted key (%i). \n", key);

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

void DeleteAll(Node* node)
{
	if (node != nullptr)
	{
		DeleteAll(node->left);
		DeleteAll(node->right);

		node->left = nullptr;
		node->right = nullptr;
		
		Delete(node, node->key);

		if (numberOfNodes > 0)
			numberOfNodes--;
	}
}

void swapValues(Node*& a, Node*& b) {
	swap(a->key, b->key);
	swap(a->keyString, b->keyString);
}

void RotateRight(Node* root) {
	// no rotate if empty or if nothing to rotate
	if (root == nullptr || root->left == nullptr) return;

	swapValues(root, root->left); // swaps key & keyString

	Node* temp = root->left;
	root->left = temp->left;
	temp->left = temp->right;
	temp->right = root->right;
	root->right = temp;

	return;
}

void RotateLeft(Node* root) {
	if (root == nullptr || root->right == nullptr) return;

	swapValues(root, root->right);

	Node* temp = root->right;
	root->right = temp->right;
	temp->right = temp->left;
	temp->left = root->left;
	root->left = temp;

	return;
}

void Backbone(Node* root) {
	while (root != nullptr)
	{
		while (root->left != nullptr)
		{
			RotateRight(root);
		}
		root = root->right;
	}
	printf("Backboned! \n");
}

void MakePerfectTree(Node* root) {
	unsigned int m = 1;
	while (m <= numberOfNodes) 
		m = 2 * m + 1; // num of rotations

	Node* temp = root->right;
	for (m = floor((numberOfNodes - 1) / 2); m >= 1; m = m / 2)
	{
		RotateLeft(root);
		temp = root;

		for (int i = 0; i < m; i++, temp = temp->right) {
			RotateLeft(temp->right);
		}
	}
	printf("DSW Completed! \n");
}

void DSW(Node* root) {

	if (root == nullptr) {
		printf("There is only root in tree, can't rotate. \n");
		return;
	}

	Backbone(root);
	MakePerfectTree(root);

	return;
}

int Height(Node* root)
{
	if (root == nullptr) // if leaf is empty it doesn't count
		return 0;

	int leftH = Height(root->left);
	int rightH = Height(root->right);

	return 1 + max(leftH, rightH);
}

void PrintHeight(Node* root) {
	
	printf("Height of tree: %i \n", root == nullptr ? 0 : Height(root));
}

#pragma endregion

int main()
{
	var start = clock();

	Node* root = nullptr;
	int X1, X2;

	fstream file { "inlab05.txt" };
	if (file.is_open())
	{
		file >> X1 >> X2;

		InsertMany(root, X1);

		PrintHeight(root);
		DSW(root);
		PrintHeight(root);
		DeleteAll(root);

		printf("\n");

		InsertMany(root, X2);

		PrintHeight(root);
		DSW(root);
		PrintHeight(root);

		printf("\n");
	}
	else
		printf("No file. \n");

	var end = clock();

	printf("Time: %0.3fs", (float)(end - start) / CLOCKS_PER_SEC);
}