#include <string>
#include <vector>

#include "fetcher.h"
#include "ui.h"

using namespace ftxui;
using BooksFetcher::Book;

int main() {
  std::vector<Book> books = BooksFetcher::fetch("books.csv");

  Ui& ui = Ui::getInstance(books);

  auto screen = ScreenInteractive::Fullscreen();
  screen.Loop(ui.render());
}
