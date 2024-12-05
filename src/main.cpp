#include <string>
#include <vector>

#include "fetcher.h"
#include "ui.h"
#include "max_heap.h"
#include "b_plus.h"

using namespace ftxui;
using BooksFetcher::Book;

int main(int argc, char** argv) {

  std::vector<Book> books;
  //prioritize dataset passed into the program
  //otherwise, search in default location: current dir w/ name books.csv
  if (argc == 2) {
    books = BooksFetcher::fetch(argv[1]);
  } else {
    books = BooksFetcher::fetch("books.csv");
  }

  //create data structures
  max_heap heap = max_heap(books);
  b_plus bptree = b_plus();
  bptree.order = 3;
  bptree.createTree(books);
  auto v = bptree.collect();

  // //run ui
  Ui& ui = Ui::getInstance(books, heap, bptree);
  ui.loop();
}
