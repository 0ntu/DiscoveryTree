#pragma once
#include "fetcher.h"
#include <vector>
#include <climits>
#include <iostream>

using BooksFetcher::Book;
using namespace std;

class max_heap {
    vector<BooksFetcher::Book> books;  
    int size;
    int capacity;

public:

    // Constructor to initialize the heap with a given capacity
    max_heap(int capacity);

    // Heapifies the max heap.
    void heapify(int i);

    // Function to build a max heap from a given books
    void buildHeap(const vector<BooksFetcher::Book>& arr);

    // Function to insert a new value into the heap
    void insert(BooksFetcher::Book value);

    // Function to get the value of the root node of the max heap
    BooksFetcher::Book top();

    // Function to remove and return the maximum value from the heap
    BooksFetcher::Book pop();

    // Function to delete a specific key from the heap
    void deleteKey(BooksFetcher::Book key);

    // Function to print the heap
    void print();
};