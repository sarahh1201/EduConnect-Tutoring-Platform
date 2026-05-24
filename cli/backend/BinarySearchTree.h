#pragma once
#include <iostream>
using namespace std;


// This file contains functions for managing a Binary Search Tree (BST)

struct Node // Creating a base node definition the Binary Search Tree will follow
{
	int data;
	Node* left; // Pointers for either the left or right child node
	Node* right;
};

Node* createNode(int value) // How to create a new node
{
	Node* newNode = new Node(); // Create a new node associated with this node
	newNode->data = value; // Assign the given value to the node in question
	newNode->left = newNode->right = nullptr; // Set its left and right children to be null
	return newNode;
}

Node* insertNode(Node* root, int value) // How to insert the node into the tree
{
	if (root == nullptr) // Case 1: There is no node present
	{
		return createNode(value); // Set this point to be where the node will be placed
	}

	else if (value < root->data) // Case 2: A node exists, and the current value is less than the current node
	{
		root->left = insertNode(root->left, value); // Recursively bring the node down the left child and check again
	}
	else if (value > root->data) // Case 3: A node exists, but the current value is greater than the parent node
	{
		root->right = insertNode(root->right, value); // Same deal, but you're now going down the right child node
	}

	return root; // When done with any of the cases above, return the root
}

void inOrderTraversal(Node* root) // Now for the traversal algorithm to kick into gear
{
	if (root != nullptr) // So long as the current node isn't null
	{
		inOrderTraversal(root->left); // Check if you can get a value out of the left-most child
		cout << root->data << " "; // When done, get the current node you're sitting in
		inOrderTraversal(root->right); // And if possible, go down any right children afterwards.
	}
}

Node* searchNode(Node* root, int key) // How to find a particular node in the tree, should be order of O(log(n))
{
	if (root == nullptr || root->data == key) // Case 1: If nothing exists OR the value matches the key
	{
		return root; // Simply return it
	}

	else if (root->data < key) // Case 2: The node is greater than the key given
	{
		return searchNode(root->right, key); // Rerun the function to see the left child node
	}

	else if (root->data > key) // Case 3: The node is less than the key given
	{
		return searchNode(root->left, key); // Rerun the function to see the right child node
	}
	return nullptr; // If nothing matches up, return null
}

Node* minValueNode(Node* node) // To find the lowest valued node
{
	Node* current = node;
	// Basically, we go down every left child node until we hit and end point
	while (current && current->left != nullptr) // To keep going down the left child
	{
		current = current->left; // If a left child exists, keep going down
	}

	return current; // Once no more left children exist, just exit with that value
}

Node* deleteNode(Node* root, int value) // And how to delete a specific node
{
	if (root == nullptr) // Case 1: Nothing exists, dummy
	{
		cout << "Error: No node exists." << endl;
		return root;
	}

	// Case 2: If the given value is less than the parent node, go down the left child
	if (value < root->data) {
		root->left = deleteNode(root->left, value);
	}
	// Case 3: If the given value is greater than the parent node, go down the right child
	else if (value > root->data) {
		root->right = deleteNode(root->right, value);
	}
	// Case 4: A value does exist that matches the key, this is where we delete the node
	else {
		// Case 4a: If the left child is null, take the right child (if it exists) and delete the current root
		if (root->left == nullptr) {
			Node* temp = root->right;
			delete root;
			return temp;
		}
		// Case 4b: If the right child is null, do the vice-versa. If both don't exist, nothing will be returned.
		else if (root->right == nullptr) {
			Node* temp = root->left;
			delete root;
			return temp;
		}

		// Case 4c: If two children exist, we make the right child check for any more children
		Node* temp = minValueNode(root->right);

		// Now copy the successor from our temp values found (could be null here too!)
		root->data = temp->data;

		// And when done, recursively delete the nodes until it's all in order.
		root->right = deleteNode(root->right, temp->data);
	}
	return root;
}
