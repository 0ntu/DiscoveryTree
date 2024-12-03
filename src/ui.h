#pragma once
#include "fetcher.h"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
using namespace ftxui;

class Ui {
public:
  static Ui &getInstance(const std::vector<BooksFetcher::Book> &books);
  Component render();

private:
  // UI is a singleton, creation should be called from Ui::getInstance(books)
  Ui(const std::vector<BooksFetcher::Book> &books);

  // UI is a singleton, remove all copy & assignment operations
  Ui(Ui const &) = delete;
  void operator=(Ui const &) = delete;

  Element buildInterface();

  Element renderPrimary();
  Element renderTabline();

  int tab_index = 0;
  std::vector<std::string> tab_options;
  Component tab_selection;
  Component exit_button;
  const std::vector<BooksFetcher::Book> &books;
};
