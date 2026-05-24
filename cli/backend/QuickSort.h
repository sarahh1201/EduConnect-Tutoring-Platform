#include <iostream>
#include <vector>
#include "Decode.h"
using namespace std;

// This file contains functions for sorting tutors by rating using QuickSort

double partition(vector<Tutor>& tutors, int low, int high) // How we do the array 'split':
{
	double pivotPoint = tutors[high].rating; // Just assign the last variable of the array to be our 'pivot'
	int prev = (low - 1); // Assign 'prev' as the index BEFORE the current index
	
	for (int cur = low; cur <= high - 1; cur++)
	{
		if (tutors[cur].rating <= pivotPoint) // If the current value is equal or greater than the pivot value
		{
			prev++; // Count the previous value up
			swap(tutors[prev], tutors[cur]); // Real simple 'swap' operation between the two
		}
	}

	swap(tutors[prev + 1], tutors[high]); // When the loop is done, swap the ending two values
	return (prev + 1); // And return this new 'high' value
}

void quickSort(vector<Tutor> &tutors, int low, int high) // The real star of the show
{
	if (low < high) // Quite simply, so long as the 'low' value is still 'low', we rerun the quicksort over and over
	{
		int partInd = partition(tutors, low, high); // How to do the partitioning operations, there's a function for that!
		quickSort(tutors, low, partInd - 1); // Left partition goes from the low bounds
		quickSort(tutors, partInd + 1, high); // Right partition goes to the high bounds, partInd is our 'middle' value for both
	}
}
