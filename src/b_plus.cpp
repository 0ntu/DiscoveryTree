//
// Created by 19546 on 11/30/2024.
//

#include "b_plus.h"
#include <iostream>

    b_plus::~b_plus() {
        node* current_node = first_leaf;
        node* temp_node = nullptr;
        while(current_node != nullptr){
            temp_node = current_node->next;
            delete current_node;
            current_node = temp_node;
        }
    }

    void b_plus::createTree(vector<BooksFetcher::Book> books) {
        sortBooks(books);
        createLeafs(books);

    }

    // This is a simple bubble sort that compares the ISBN13 numbers of the books and sorts them in ascending order
    void b_plus::sortBooks(vector<BooksFetcher::Book>& books) {
        for(int i = 0; i < books.size() - 1; i++){
            bool swapped = false;
            for(int j = 0; j < books.size() - i - 1; j++){
                if(stoll(books[j].isbn13) > stoll(books[j+1].isbn13)){
                    swap(books[j], books[j+1]);
                    swapped = true;
                }
            }

            if(!swapped){
                break;
            }
        }
    }

    // This takes in a sorted vector of books and creates a linked list of leaf nodes
    // Each leaf node contains at least ceil(order/2) values and at most order-1 values
    void b_plus::createLeafs(vector<BooksFetcher::Book>& books) {
//        int books_added = 0;
        // This initializes our first node in the linked list
        node* start = new node;
        start->isLeaf = true;
        first_leaf = start;

        for(int i = 0; i < order-1; i++){
            start->keys.push_back(books[i]);
        }

//        books_added += order-1;

        node* previous_node = first_leaf;

        // This initializes every node after the first node
        for(int i = 1; i < books.size()/(order - 1); i++){
            node* next_node = new node;
            previous_node->next = next_node;
            next_node->isLeaf = true;

            // If we are at the last full node, we check if there are still more books at the end of the vector
            // If so, we split the remaining books into two smaller nodes
            if(i == books.size()/(order - 1) - 1 && books.size()%(order - 1) != 0){
                int remaining_books = (order - 1) + books.size()%(order - 1);

                // We insert the first half of the books into the first node
                for(int j = 0; j < remaining_books / 2; j++){
                    next_node->keys.push_back(books[j + (i * (order - 1))]);
//                    books_added += 1;
                }

                // We initialize the second node
                node* final_node = new node;
                next_node->next = final_node;
                final_node->isLeaf = true;

                // We insert the other half of the remaining books into the second node
                for(int k = 0; k < remaining_books - (remaining_books / 2); k++){
                    final_node->keys.push_back(books[k + (remaining_books / 2) + (i * (order - 1))]);
//                    books_added += 1;
                }
            }

            // Otherwise, we fill the current node with order - 1 books as usual
            else{
                for(int j = 0; j < order - 1; j++){
                    next_node->keys.push_back(books[j + (i * (order - 1))]);
//                    books_added += 1;
                }
            }

            previous_node = next_node;
        }
//        std::cout << books_added << std::endl;
    }

    void b_plus::createInternalNodes(vector<BooksFetcher::Book>& books) {

    }

    b_plus::node* b_plus::searchTree(int isbn) {

    }