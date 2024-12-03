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
            bool isLeaf;
            //Our keys are a vector of Book structs defined in fetcher.h
            vector<Book> keys;
            vector<node *> children;
            node *next;
        };
        node *root;
        int max_degree;

        //Function to create a tree using a vector of books
        void createTree(vector<Book> books);

        //Function to sort a vector of books according to ISBN number
        void sortBooks(vector<Book> &books);

        //Function to create the leaf nodes
        void createLeafs(vector<Book> &books);

        //Function to create internal nodes
        void createInternalNodes(vector<Book> &books);

        //Function to search tree according to ISBN number
        node *searchTree(int isbn);
    };