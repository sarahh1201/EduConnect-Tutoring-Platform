#pragma once

#include <iostream>
#include <vector>
#include <climits>

using namespace std;

// This file contains functions for managing a min-heap data structure for requests

// Now for the very lovely Min-Heap algorithm
// Note this is ALL using a 0-based array architecture, since I don't wanna figure out the 1-based implementation

class MinHeap
{
private:
    vector<int> array; // Our heap should be based upon a vectored array that can be assigned nodes going down
    int size;
    int capacity;

public:
    MinHeap(int capacity) // Basic constructor where nothing exists
    {
        this->size = 0;
        this->capacity = capacity;
        this->array.resize(capacity); // Really sketchy function to resize an array, yeesh
    }

    void heapify(int i) // This enables a given value in the array to be organized into the 'heap' format, via recursion, woo!
    {
        int smallest = i; // For a given input 'i'
        int left = 2 * i + 1; // Left child is at (2i + 1) relative to index 'i'
        int right = 2 * i + 2; // Right child is at (2i + 2) instead

        if (left < size && array[left] < array[smallest]) // Case 1: the left child exists in bounds and has a smaller value than 'i'
        {
            smallest = left;
        }

        if (right < size && array[right] < array[smallest]) // Case 2: the right child is in bounds and is smaller than 'i' OR the left child subtree!
        {
            smallest = right;
        }

        if (smallest != i) // If 'i' gets swapped around, you then swap 'i' around with that given 'smallest' value, be it the left OR right side
        { 
            swap(array[i], array[smallest]);
            heapify(smallest); // And repeat this until 'i' is placed in a valid position. Simple!
        }
    }

    void buildHeap(vector<int>& arr) // How to build a heap from an array as an input (real sketchy stuff here)
    {
        capacity = arr.size(); // Get the array's size to be the capacity
        size = capacity; // And forcefully set the size equal to said capacity since everything's gonna be inserted in
        array = arr;

        // Wacky 'for' loop that runs the 'heapify' algorithm for every index of the inputted array until everything's in
        for (int i = (size - 1) / 2; i >= 0; i--) {
            heapify(i);
        }
    }

    void insert(int value) 
    {
        if (size == capacity) // If the heap is at the capacity limit, just variably change it, what's the worst that could happen?
        {
            capacity *= 2; // Double it when at capacity
            array.resize(capacity); // And resize the whole thing
        }

        size++; // Bump up the current size
        int i = size - 1; // Since we're using a 0-based array
        array[i] = value; // Assign this null node a value

        while (i != 0 && array[(i - 1) / 2] > array[i])  // So: if its parent node is GREATER than the node that has been inserted, swap it up an urgency level
        {
            swap(array[i], array[(i - 1) / 2]);
            i = (i - 1) / 2; // And check again for any other nodes above
        }
    }

    void pop() // How to 'pop out' the root node, which is effectively taking out the lowest-valued node in the entire tree
    {
        if (size <= 0) // Case 1: If no value exists in the heap, return an exception
        {
            cout << "Error: No value exists in the min heap, what are you doing?" << endl;
            return;
        }
        else if (size == 1) // Case 2: Exactly one value exists in the heap
        {
            size--;
            cout << "Popped out value " << array[0] << ", heap is now empty." << endl;
            return;
        }
        else
        {
            // If otherwise, set the root to array[0] temporarily, wipe the node from existence, and heapify the root to get the next lowest value into the root
            double root = array[0];
            array[0] = array[size - 1];
            size--;
            heapify(0);
            cout << "Popped out value " << root << endl;
            return;
        }
    }

    int pop_val()
    {
        if (size <= 0) // Case 1: If no value exists in the heap, return an exception
        {
            cout << "Error: No value exists in the min heap, what are you doing?" << endl;
            return -1;  // Indicates that the heap is empty
        }
        else if (size == 1) // Case 2: Exactly one value exists in the heap
        {
            size--;
            return array[0];
        }
        else
        {
            // If otherwise, set the root to array[0] temporarily, wipe the node from existence, and heapify the root to get the next lowest value into the root
            int root = array[0];
            array[0] = array[size - 1];
            size--;
            heapify(0);
            return root;
        }
    }

    int peek() 
    {
        
        if (size <= 0) // Case 1: The heap is empty, so you should check that first
        {
            cout << "Error: Heap is empty, what are you doing?" << endl;
            return -1;  // Indicates that the heap is empty
        }
        return array[0]; // If the root does exist, simply return that
    }

    void print() // And just a plain function to print out the heap, very simple for loop!
    {
        for (int i = 0; i < size; i++)
            cout << array[i] << " ";
        cout << endl;
    }

    bool isEmpty() const {
        return size == 0;
    }

    void remove() {
        pop();
    }
};
