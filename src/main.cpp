#include <string>
#include <vector>

#include "fetcher.h"
#include "ui.h"

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

  Ui& ui = Ui::getInstance(books);
  ui.loop();
}
