#include "max_heap.h"

max_heap::max_heap(const vector<BooksFetcher::Book>& arr) {
    buildHeap(arr);
}

void max_heap::heapify(int i) {
    int largest = i;           
    int left = 2 * i + 1;      
    int right = 2 * i + 2;     
    
    if (left < size && books[left].rating > books[largest].rating)
        largest = left;


    if (right < size && books[right].rating > books[largest].rating)
        largest = right;

    
    if (largest != i) {
        swap(books[i], books[largest]);  
        heapify(largest);               
    }
}


void max_heap::buildHeap(const vector<BooksFetcher::Book>& arr) {
    capacity = arr.size();
    size = capacity;
    books = arr;

    // Build heap (rearrange books)
    for (int i = (size - 1) / 2; i >= 0; i--) {
        heapify(i);
    }
}


void max_heap::insert(BooksFetcher::Book value) {
    if (size == capacity) {
        // Resize the heap if necessary
        capacity *= 2;
        books.resize(capacity);
    }

    size++;
    int i = size - 1;
    books[i] = value;

    // Fix the max heap property if it is violated
    while (i != 0 && books[(i - 1) / 2].rating < books[i].rating) {
        swap(books[i], books[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}


BooksFetcher::Book max_heap::top() {
      return books[0];
}


BooksFetcher::Book max_heap::pop() {
    if (size == 1) {
        size--;
        return books[0];
    }

    // Store the maximum value, and remove it
    BooksFetcher::Book root = books[0];
    books[0] = books[size - 1];
    size--;
    // Heapify the root node after deletion
    max_heap::heapify(0);  
    return root;
}


void max_heap::deleteKey(BooksFetcher::Book key) {
    // Find the index of the key
    int index = -1;
    for (int i = 0; i < size; ++i) {
        if (books[i].rating == key.rating) {
            index = i;
            break;
        }
    }
    // If key is not found, return
    if (index == -1) {
        cout << "Key not found" << endl;
        return;
    }

    // If the key is found, delete it
    // If it's the last element, simply reduce the size
    if (index == size - 1) {
        size--;
        return;
    }

    // Move the last element to the index of the key to be deleted
    books[index] = books[size - 1];
    size--;

    // Heapify down to maintain heap property
    heapify(index);
}


void max_heap::print() {
    cout << "Max Heap: ";
    for (int i = 0; i < size; ++i)
        cout << books[i].title << " ";
    cout << endl;
}
