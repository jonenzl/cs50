/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>
#include <string.h>

#include "helpers.h"

bool linear_search(int value, int values[], int n);
bool binary_search(int value, int values[], int n);
void bubble_sort(int values[], int n);
void selection_sort(int values[], int n);
void counting_sort(int values[], int n);

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // return false if n is negative
    if (n < 0)
    {
        return false;
    }
    else
    {
        return binary_search(value, values, n);
    }
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    counting_sort(values, n);
    return;
}

/**
 * Searching and Sorting Functions
 */

// linear search function to be called in the search function
bool linear_search(int value, int values[], int n)
{
    // iterate through each element of the array
    for (int i = 0; i < n; i++)
    {
        // check whether the current element is the value we are looking for
        if (values[i] == value)
        {
            return true;
        }
    }
    return false;
}

// binary search function to be called in the search function
bool binary_search(int value, int values[], int n)
{
    // initialise variables for the limits of the search, and the middle of the array
    int beginning = 0;
    int ending = n - 1;
    int middle = 0;
    
    // implement binary search to look for value in array
    // return true if value is in values[]
    while (beginning <= ending)
    {
        // set the middle of the array
        middle = (beginning + ending) / 2;
        
        // check if the middle element of the array is the value we are searching for
        if (values[middle] == value)
        {
            return true;
        }
        
        // check if the value is greater than the middle of the array
        else if (values[middle] < value)
        {
            beginning = middle + 1;
        }
        
        // check if the value is less than the middle of the array
        else if (values[middle] > value)
        {
            ending = middle - 1;
        }
        
        else
        {
            break;
        }
    }
    return false;
}

// bubble sort function to be called in the sort function
void bubble_sort(int values[], int n)
{
    // declare temp and initialise swapped variable
    int temp, i, j;
    bool swapped = false;
    
    // iterate through all the elements of the array
    for (i = 0; i < n - 1; i++)
    {
        // iterate through the numbers falling ahead
        for (j = 0; j < n - i - 1; j++)
        {
            // check whether the current element of the array is greater than the following element of the array
            if (values[j] > values[j + 1])
            {
                // swap elements in the array
                temp = values[j];
                values[j] = values[j + 1];
                values[j + 1] = temp;
                swapped = true;
            }
        }
        // if no elements are being swapped, the array is now sorted, break loop
        if (!swapped)
        {
            break;
        }
    }
}

// selection sort function to be called in the sort function
void selection_sort(int values[], int n)
{
    // declare temp variable
    int temp;
    
    // iterate through all the elements of the array 
    for (int i = 0; i < n; i++)
    {
        int smallest_index = i;
        // iterate through the following element in the array
        for (int j = i + 1; j < n; j++)
        {
            // check whether the current element is less than the preceding element
            if (values[smallest_index] > values[j])
            {
                smallest_index = j;
            }
        }
        // swap elements in the array
        temp = values[smallest_index];
        values[smallest_index] = values[i];
        values[i] = temp;
    }
}

// counting sort function to be called in the sort function
void counting_sort(int values[], int n)
{
    // create output array that will have sorted array
    int output[n];
    
    // create a count array to store count of integers and initialise count array as 0
    int count[65536];
    memset(count, 0, sizeof(count));
    
    // store count of each integer
    for (int i = 0; values[i]; i++)
    {
        count[values[i]]++;
    }
    
    // change count[i] so that it contains actual position of integer in output array
    for (int i = 1; i <= 65536; i++)
    {
        count[i] += count[i - 1];
    }
    
    // build the output array
    for (int i = 0; values[i]; i++)
    {
        output[count[values[i]] - 1] = values[i];
        count[values[i]]--;
    }
    
    // copy the output array to the values array in sorted order
    for (int i = 0; values[i]; i++)
    {
        values[i] = output[i];
    }
}