//
// Created by 19546 on 11/30/2024.
//
#pragma once
#include "fetcher.h"
using BooksFetcher::Book;
using namespace std;

    class b_plus {
    public:
        struct node {
            bool isLeaf = false;
            // Our keys are a vector of Book structs defined in fetcher.h
            vector<Book> keys;
            vector<node *> children;
            node *next = nullptr;
        };
        node *root;
        node *first_leaf;
        int num_nodes = 0;

        // The order defines the maximum number of children each internal node can have
        // Each internal node has at least ceil(order/2) children and at most order-1 keys
        // Every leaf node will have at least ceil(order/2) keys and at most order-1 keys
        // For the sake of easier implementation and creating a denser tree, this b+ tree will assume all
        // leaf nodes are maximally filled

        ~b_plus();

        int order;

        //Function to create a tree using a vector of books
        void createTree(vector<Book> books);

        //Function to sort a vector of books according to ISBN13 number
        void sortBooks(vector<Book> &books);

        //Function to create the leaf nodes
        void createLeafs(vector<Book> &books);

        //Function to create internal nodes
        void createInternalNodes(vector<Book> &books);

        void fillInternalNodes(vector<node*>& start_layer);

        Book getSmallestBook(node* start_node);

        //Function to search tree according to ISBN13 number
        node *searchTree(int isbn);
    };