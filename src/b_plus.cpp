//
// Created by 19546 on 11/30/2024.
//

#include "b_plus.h"
#include <iostream>

    b_plus::~b_plus() {
        int num_deleted = 0;
        deleteTree(root, num_deleted);
        cout << num_deleted << endl;
    }

    void b_plus::deleteTree(b_plus::node *start_node, int& num_deleted) {
        if(start_node->isLeaf){
            delete start_node;
            num_deleted += 1;
            return;
        }
        for(int i = 0; i < start_node->children.size(); i++){
            deleteTree(start_node->children[i], num_deleted);
        }
        num_deleted += 1;
        delete start_node;
    }

    void b_plus::createTree(vector<BooksFetcher::Book> books) {
        sortBooks(books);
        createLeafs(books);
        createInternalNodes(books);
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

        num_nodes += 1;

        node* previous_node = first_leaf;

        // This initializes every node after the first node
        for(int i = 1; i < books.size()/(order - 1); i++){
            node* next_node = new node;
            previous_node->next = next_node;
            next_node->isLeaf = true;
            num_nodes += 1;

            // If we are at the last full node, we check if there are still more books at the end of the vector
            // If so, we split the remaining books into two smaller nodes
            if(i == books.size()/(order - 1) - 1 && books.size()%(order - 1) != 0){
                int remaining_books = (order - 1) + books.size()%(order - 1);

                // We insert the first half of the books into the first node
                for(int j = 0; j < remaining_books / 2; j++){
                    next_node->keys.push_back(books[j + (i * (order - 1))]);
                }

                // We initialize the second node
                node* final_node = new node;
                next_node->next = final_node;
                final_node->isLeaf = true;
                num_nodes += 1;

                // We insert the other half of the remaining books into the second node
                for(int k = 0; k < remaining_books - (remaining_books / 2); k++){
                    final_node->keys.push_back(books[k + (remaining_books / 2) + (i * (order - 1))]);
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
        vector<node*> first_layer;
        node* current_node = first_leaf;
        if(num_nodes / order == 0){
            node* new_internal = new node;
            for(int i = 0; i < num_nodes; i++){
                if(i != num_nodes - 1){
                    new_internal->keys.push_back(current_node->next->keys[0]);
                }
                new_internal->children.push_back(current_node);
                current_node = current_node->next;
            }
            root = new_internal;
        }
        else{
            for(int i = 0; i < num_nodes / order; i++){
                node* new_internal = new node;
                first_layer.push_back(new_internal);
                if(i == (num_nodes / order) - 1 && num_nodes % order != 0){
                    int remaining_nodes = order + (num_nodes % order);
                    for(int j = 0; j < remaining_nodes / 2; j++){
                        if(j != (remaining_nodes / 2) - 1){
                            new_internal->keys.push_back(current_node->next->keys[0]);
                        }
                        new_internal->children.push_back(current_node);
                        current_node = current_node->next;
                    }
                    node* final_internal = new node;
                    first_layer.push_back(final_internal);
                    for(int k = 0; k < remaining_nodes - (remaining_nodes / 2); k++){
                        if(k != remaining_nodes - (remaining_nodes / 2) - 1){
                            final_internal->keys.push_back(current_node->next->keys[0]);
                        }
                        final_internal->children.push_back(current_node);
                        current_node = current_node->next;
                    }
                }
                else{
                    for(int j = 0; j < order; j++){
                        if(j != order - 1){
                            new_internal->keys.push_back(current_node->next->keys[0]);
                        }
                        new_internal->children.push_back(current_node);
                        current_node = current_node->next;
                    }
                }
            }
            fillInternalNodes(first_layer);
        }
    }

    void b_plus::fillInternalNodes(vector<b_plus::node *>& start_layer) {
        vector<node*> current_layer;
        while(start_layer.size() != 1){
            if(start_layer.size() / order == 0){
                node* new_internal = new node;
                current_layer.push_back(new_internal);
                for(int i = 0; i < start_layer.size(); i++){
                    if(i != start_layer.size() - 1){
                        new_internal->keys.push_back(getSmallestBook(start_layer[i + 1]));
                    }
                    new_internal->children.push_back(start_layer[i]);
                }
            }
            else{
                for(int i = 0; i < start_layer.size() / order; i++){
                    node* new_internal = new node;
                    current_layer.push_back(new_internal);
                    if(i == (start_layer.size() / order) - 1 && start_layer.size() % order != 0){
                        int remaining_nodes = order + (start_layer.size() % order);
                        for(int j = 0; j < remaining_nodes / 2; j++){
                            if(j != (remaining_nodes / 2) - 1){
                                new_internal->keys.push_back(getSmallestBook(start_layer[(i * order) + j + 1]));
                            }
                            new_internal->children.push_back(start_layer[(i * order) + j]);
                        }
                        node* final_internal = new node;
                        current_layer.push_back(final_internal);
                        for(int k = 0; k < remaining_nodes - (remaining_nodes / 2); k++){
                            if(k != remaining_nodes - (remaining_nodes / 2) - 1){
                                final_internal->keys.push_back(getSmallestBook(start_layer[(i * order) + k + (remaining_nodes / 2) + 1]));
                            }
                            final_internal->children.push_back(start_layer[(i * order) + k + (remaining_nodes / 2)]);
                        }
                    }
                    else{
                        for(int j = 0; j < order; j++){
                            if(j != order - 1){
                                new_internal->keys.push_back(getSmallestBook(start_layer[(i * order) + j + 1]));
                            }
                            new_internal->children.push_back(start_layer[(i * order) + j]);
                        }
                    }
                }
            }
            start_layer = current_layer;
            current_layer.clear();
        }
        root = start_layer[0];
    }

    Book b_plus::getSmallestBook(b_plus::node *start_node) {
        while(!start_node->isLeaf){
            start_node = start_node->children[0];
        }
        return start_node->keys[0];
    }

    b_plus::node* b_plus::searchTree(int isbn) {

    }

//#include <string>
//#include <vector>
//#include <iostream>
//
//#include "fetcher.h"
////#include "ui.h"
//#include "b_plus.h"
//
////using namespace ftxui;
//using BooksFetcher::Book;
//
//int main() {
//    std::vector<Book> books = BooksFetcher::fetch("books.csv");
//    std::vector<Book> small_books;
//    for(int i = 0; i < 6; i++){
//        small_books.push_back(books[i]);
//    }
//    b_plus tree;
//    tree.order = 3;
//    tree.createTree(small_books);
//    cout << tree.root->keys[0].title << endl;
////  Ui& ui = Ui::getInstance(books);
////
////  auto screen = ScreenInteractive::Fullscreen();
////  screen.Loop(ui.render());
//}
