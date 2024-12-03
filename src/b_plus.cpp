//
// Created by 19546 on 11/30/2024.
//

#include "b_plus.h"
    void b_plus::createTree(vector<BooksFetcher::Book> books) {
        sortBooks(books);
    }

    void b_plus::sortBooks(vector<BooksFetcher::Book>& books) {
        for(int i = 0; i < books.size() - 1; i++){
            bool swapped = false;
            for(int j = 0; j < books.size() - i - 1; j++){
                if(stol(books[j].isbn) > stol(books[j+1].isbn)){
                    swap(books[j], books[j+1]);
                    swapped = true;
                }
            }
            if(!swapped){
                break;
            }
        }
    }

    void b_plus::createLeafs(vector<BooksFetcher::Book>& books) {
        node* start = new node;
        start->isLeaf = true;
        first_leaf = start;
        for(int i = 0; i < order-1; i++){
            start->keys.push_back(books[i]);
        }
        node* previous_node = first_leaf;
        for(int i = 1; i < books.size()/(order - 1); i++){
            node* next_node = new node;
            previous_node->next = next_node;
            next_node->isLeaf = true;
            if(i == books.size()/(order - 1) - 1 && books.size()%(order - 1) != 0){
                int remaining_books = (order - 1) + books.size()%(order - 1);
                for(int j = 0; j < remaining_books / 2; j++){
                    next_node->keys.push_back(books[j + (i * (order - 1))]);
                }
                node* final_node = new node;
                next_node->next = final_node;
                final_node->isLeaf = true;
                for(int k = 0; k < remaining_books - (remaining_books / 2); k++){
                    final_node->keys.push_back(books[k + (remaining_books / 2) + (i * (order - 1))]);
                }
            }
            else{
                for(int j = 0; j < order - 1; j++){
                    next_node->keys.push_back(books[j + (i * (order - 1))]);
                }
            }
            previous_node = next_node;
        }
    }

    void b_plus::createInternalNodes(vector<BooksFetcher::Book>& books) {

    }

    b_plus::node* b_plus::searchTree(int isbn) {

    }