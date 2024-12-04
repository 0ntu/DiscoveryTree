//
// Created by 19546 on 11/30/2024.
//

#include "b_plus.h"
#include <iostream>
#include <queue>

    // Destructor for the b+ tree
    b_plus::~b_plus() {
        int num_deleted = 0;

        // Calls the deleteTree function to recursively delete the nodes
        deleteTree(root, num_deleted);
        cout << num_deleted << endl;
    }

    // Function to recursively delete the nodes in the tree with postorder traversal
    void b_plus::deleteTree(b_plus::node *start_node, int& num_deleted) {
        // If our current node is a leaf node, delete it and return
        if(start_node->isLeaf){
            delete start_node;
            num_deleted += 1;
            return;
        }
        // If our current node is not a leaf node, visit all of its children first and delete them, then delete this node
        for(int i = 0; i < start_node->children.size(); i++){
            deleteTree(start_node->children[i], num_deleted);
        }
        num_deleted += 1;
        delete start_node;
    }

    // Function to create the tree given a vector of book objects
    void b_plus::createTree(vector<BooksFetcher::Book> books) {
        // Sorts the vector of book objects
        sortBooks(books);

        // Creates the leaves first
        createLeafs(books);

        // Creates the internal nodes and sets the root
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

    // This function creates the internal nodes of the b+ tree after the leaf nodes have been initialized
    void b_plus::createInternalNodes(vector<BooksFetcher::Book>& books) {
        //  We start with our first layer, which connects to the leaf nodes
        vector<node*> first_layer;
        node* current_node = first_leaf;

        // If there aren't enough leaves to create a full set of children for the internal node, we store what leaves
        // there are as children in a single internal node
        if(num_nodes / order == 0){
            node* new_internal = new node;
            for(int i = 0; i < num_nodes; i++){
                // If the current leaf is not the final leaf, we add the book with the smallest ISBN13 number in the
                // next leaf to the keys of the internal node
                if(i != num_nodes - 1){
                    new_internal->keys.push_back(current_node->next->keys[0]);
                }

                // We add the current leaf to the vector of children associated with the internal node
                new_internal->children.push_back(current_node);
                current_node = current_node->next;
            }
            // We set the root to be the internal node
            root = new_internal;
        }
        // Else, if there are enough leaves to create at least a single full set of children for an internal node,
        // we loop for the number of full sets that can be made
        else{
            for(int i = 0; i < num_nodes / order; i++){
                node* new_internal = new node;
                first_layer.push_back(new_internal);

                // If we are at the last full set of leaves, but there are additional leaves that cannot form a full set,
                // we split the remaining leaves in half and create two new internal nodes
                // This ensures that both internal nodes have at least ceil(order/2) children
                if(i == (num_nodes / order) - 1 && num_nodes % order != 0){
                    int remaining_nodes = order + (num_nodes % order);

                    for(int j = 0; j < remaining_nodes / 2; j++){
                        if(j != (remaining_nodes / 2) - 1){
                            new_internal->keys.push_back(current_node->next->keys[0]);
                        }

                        // We add the current leaf to the vector of children associated with the internal node
                        new_internal->children.push_back(current_node);
                        current_node = current_node->next;
                    }

                    // We initialize the final node and add the remaining leaves to its children and any relevant
                    // books to its keys
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

                // Else, if we can still make more full sets of leaves without any remainders following it,
                // we add a full internal node as usual
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

            // Once the first layer of internal nodes has been initialized, we call fillInternalNodes to create more
            // internal nodes leading to the root
            fillInternalNodes(first_layer);
        }
    }

    // This function takes in the first layer of internal nodes and builds up from it until a single root node is reached
    void b_plus::fillInternalNodes(vector<b_plus::node *>& start_layer) {
        // current_layer is the current layer that is being filled with nodes that point to the nodes in start_layer
        vector<node*> current_layer;

        // While there is still more than one node in the starting layer, we create an additional layer of
        // internal nodes to point to the starting layer
        while(start_layer.size() != 1){

            // If there is not enough nodes in the starting layer to create a full set of children, we place the
            // remainder within a single internal node
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

            // Else, if there are enough nodes to create at least one full set of children,
            // we create as many full internal nodes as possible
            else{
                for(int i = 0; i < start_layer.size() / order; i++){
                    node* new_internal = new node;
                    current_layer.push_back(new_internal);

                    // If we are at the final set of full nodes but there are still remaining nodes after it,
                    // we combine the remaining nodes and split them among two internal nodes
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

                    // Else, if there are still more full sets of nodes without any remainders after them,
                    // we create full internal nodes as usual
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
            // Once every node in the start_layer have become children to nodes in the current_layer,
            // we set the start_layer to the current_layer, clear the current_layer, and repeat
            start_layer = current_layer;
            current_layer.clear();
        }

        // Once the start_layer has only one node, we set that node to be the root
        root = start_layer[0];
    }

    // This function gets the book with the smallest ISBN13 number contained within the children of a given node
    Book b_plus::getSmallestBook(b_plus::node *start_node) {
        while(!start_node->isLeaf){
            start_node = start_node->children[0];
        }
        return start_node->keys[0];
    }

    Book b_plus::searchTree(int index) {
        int current_index = 0;
        node* current_node = first_leaf;
        while(current_index != index){
            for(int i = 0; i < current_node->keys.size(); i++){
                if(current_index == index){
                    return current_node->keys[i];
                }
                current_index += 1;
            }
        }
    }

    vector<vector<b_plus::node*>> b_plus::levelOrderTraverse() {
        vector<vector<node*>> traversal;
        vector<node*> first_level;
        traversal.push_back(first_level);
        recursiveLevelOrder(root, traversal, 0);
        return traversal;
    }

    void b_plus::recursiveLevelOrder(b_plus::node* root_node, vector<vector<node*>>& traversal, int level_num) {
        if(traversal.size() - 1 < level_num){
            vector<node*> curr_level;
            traversal.push_back(curr_level);
        }
        if(root_node->isLeaf){
            traversal[level_num].push_back(root_node);
            return;
        }
        traversal[level_num].push_back(root_node);
        for(int i = 0; i < root_node->children.size(); i++){
            recursiveLevelOrder(root_node->children[i], traversal, level_num + 1);
        }
        traversal[level_num + 1].push_back(nullptr);
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
