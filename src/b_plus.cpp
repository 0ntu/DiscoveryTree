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

    }

    void b_plus::createInternalNodes(vector<BooksFetcher::Book>& books) {

    }

    b_plus::node* b_plus::searchTree(int isbn) {

    }